/**
 ******************************************************************************
 * @file    ms5607.c
 * @version V1.0.0
 * @date    2017/11/10
 * @brief   The file defines MS5607 sensor operating functions, including communication interface initialization,
 *           module initialization, resetting and getting data etc.
 *
 * @copyright (c) 2017-2018 DJI. All rights reserved.
 *
 * All information contained herein is, and remains, the property of DJI.
 * The intellectual and technical concepts contained herein are proprietary
 * to DJI and may be covered by U.S. and foreign patents, patents in process,
 * and protected by trade secret or copyright law.  Dissemination of this
 * information, including but not limited to data and other proprietary
 * material(s) incorporated within the information, in any form, is strictly
 * prohibited without the express written consent of DJI.
 *
 * If you receive this source code without DJI’s authorization, you may not
 * further disseminate the information, and you must immediately remove the
 * source code and notify DJI of its removal. DJI reserves the right to pursue
 * legal actions against you for any loss(es) or damage(s) caused by your
 * failure to do so.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "psdk_arch_sys.h"
#include <math.h>
#include <stm32f4xx_gpio.h>
#include <dji_typedef.h>
#include "ms5607.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static unsigned int s_Ms5607Calibra[8]; // calibration coefficients

/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Ms5607_Reset(void);
static unsigned long Ms5607_CmdAdc(char cmd);
static unsigned int Ms5607_CmdProm(char coef_num);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the peripherals
  * @param  None
  * @return None
  */
static void sFLASH_LowLevel_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*!< Enable the SPI clock */
    MS5607_SPI_CLK_INIT(MS5607_SPI_CLK, ENABLE);

    /*!< Enable GPIO clocks */
    RCC_AHB1PeriphClockCmd(MS5607_SPI_SCK_GPIO_CLK | MS5607_SPI_MISO_GPIO_CLK |
                           MS5607_SPI_MOSI_GPIO_CLK | MS5607_CS_GPIO_CLK, ENABLE);

    /*!< SPI pins configuration *************************************************/

    /*!< Connect SPI pins to AF5 */
    GPIO_PinAFConfig(MS5607_SPI_SCK_GPIO_PORT, MS5607_SPI_SCK_SOURCE, MS5607_SPI_SCK_AF);
    GPIO_PinAFConfig(MS5607_SPI_MISO_GPIO_PORT, MS5607_SPI_MISO_SOURCE, MS5607_SPI_MISO_AF);
    GPIO_PinAFConfig(MS5607_SPI_MOSI_GPIO_PORT, MS5607_SPI_MOSI_SOURCE, MS5607_SPI_MOSI_AF);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

    /*!< SPI SCK pin configuration */
    GPIO_InitStructure.GPIO_Pin = MS5607_SPI_SCK_PIN;
    GPIO_Init(MS5607_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

    /*!< SPI MOSI pin configuration */
    GPIO_InitStructure.GPIO_Pin = MS5607_SPI_MOSI_PIN;
    GPIO_Init(MS5607_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

    /*!< SPI MISO pin configuration */
    GPIO_InitStructure.GPIO_Pin = MS5607_SPI_MISO_PIN;
    GPIO_Init(MS5607_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

    /*!< Configure sFLASH Card CS pin in output pushpull mode ********************/
    GPIO_InitStructure.GPIO_Pin = MS5607_CS_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(MS5607_CS_GPIO_PORT, &GPIO_InitStructure);
}

/**
  * @brief  Sends a byte through the SPI interface and return the byte received
  *         from the SPI bus.
  * @param  byte byte to send.
  * @return The value of the received byte.
  */
static uint8_t SPIWrap_SendByte(uint8_t byte)
{
    /*!< Loop while DR register in not empty */
    while (SPI_I2S_GetFlagStatus(MS5607_SPI, SPI_I2S_FLAG_TXE) == RESET);

    /*!< Send byte through the SPI1 peripheral */
    SPI_I2S_SendData(MS5607_SPI, byte);

    /*!< Wait to receive a byte */
    while (SPI_I2S_GetFlagStatus(MS5607_SPI, SPI_I2S_FLAG_RXNE) == RESET);

    /*!< Return the byte read from the SPI bus */
    return (uint8_t) SPI_I2S_ReceiveData(MS5607_SPI);
}

/**
 * @brief MS5607 module reset.
 * @param  None
 * @return None
 */
static void Ms5607_Reset(void)
{
    MS5607_CSB_LOW(); // pull CSB low to start the command
    SPIWrap_SendByte(MS5607_CMD_RESET); // send reset sequence
    MS5607_DELAY(3); // wait for the reset sequence timing
    MS5607_CSB_HIGH(); // pull CSB high to finish the command
}

/**
 * @brief Read MS5607 sensor data.
 * @param cmd Command of reading sensor data.
 * @return Temperature value.
 */
static unsigned long Ms5607_CmdAdc(char cmd)
{
    unsigned int ret;
    unsigned long temp = 0;

    MS5607_CSB_LOW(); // pull CSB low
    SPIWrap_SendByte((uint8_t) (MS5607_CMD_ADC_CONV + cmd)); // send conversion command
    switch (cmd & 0x0f) // wait necessary conversion time
    {
        case MS5607_CMD_ADC_256 :
            MS5607_DELAY(1);
            break;
        case MS5607_CMD_ADC_512 :
            MS5607_DELAY(3);
            break;
        case MS5607_CMD_ADC_1024:
            MS5607_DELAY(4);
            break;
        case MS5607_CMD_ADC_2048:
            MS5607_DELAY(6);
            break;
        case MS5607_CMD_ADC_4096:
            MS5607_DELAY(10);
            break;
        default:
            break;
    }
    MS5607_CSB_HIGH(); // pull CSB high to finish the conversion
    MS5607_CSB_LOW(); // pull CSB low to start new command

    SPIWrap_SendByte(MS5607_CMD_ADC_READ); // send ADC read command
    ret = SPIWrap_SendByte(0x00); // send 0 to read 1st byte (MSB)
    temp = 65536 * ret;
    ret = SPIWrap_SendByte(0x00); // send 0 to read 2nd byte
    temp = temp + 256 * ret;
    ret = SPIWrap_SendByte(0x00); // send 0 to read 3rd byte (LSB)
    temp = temp + ret;

    MS5607_CSB_HIGH(); // pull CSB high to finish the read command
    return temp;
}

/**
 * @brief PROM read functions.
 * @param coef_num PROM address.
 * @return Value of PROM in corresponding address.
 */
static unsigned int Ms5607_CmdProm(char coef_num)
{
    unsigned int ret;
    unsigned int rC = 0;

    MS5607_CSB_LOW(); // pull CSB low
    SPIWrap_SendByte((uint8_t) (MS5607_CMD_PROM_RD + coef_num * 2)); // send PROM READ command
    ret = SPIWrap_SendByte(0x00); // send 0 to read the MSB
    rC = 256 * ret;
    ret = SPIWrap_SendByte(0x00); // send 0 to read the LSB
    rC = rC + ret;
    MS5607_CSB_HIGH(); // pull CSB high
    return rC;
}

/* Exported functions --------------------------------------------------------*/

/**
  * @brief  Initializes the peripherals used by the SPI FLASH driver.
  * @param  None
  * @return None
  */
void Ms5607Spi_Init(void)
{
    SPI_InitTypeDef SPI_InitStructure;

    sFLASH_LowLevel_Init();

    /*!< Deselect : Chip Select high */
    MS5607_CSB_HIGH();

    /*!< SPI configuration */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;

    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(MS5607_SPI, &SPI_InitStructure);

    /*!< Enable the sFLASH_SPI  */
    SPI_Cmd(MS5607_SPI, ENABLE);
}

/**
 * @brief MS5607 initialization function.
  * @param  None
  * @return Module initialization state.
 */
int Ms5607_Init(void)
{
    int i;

    Ms5607_Reset(); // reset the module after powerup
    MS5607_DELAY(1);
    for (i = 0; i < 8; i++) {
        s_Ms5607Calibra[i] = Ms5607_CmdProm((char) i);
    } // read calibration coefficients

    return RTN_SUCCESS;
}

/**
 * @brief Get ms5607 sensor data.
 * @note Temperature unit is oc and pressure unit is mbar.
 * @param temperature Pointer to temperature value to be read.
 * @param pressure Pointer to pressure value to be read.
 * @return Function execute state.
 */
int Ms5607_GetSenorData(double *temperature, double *pressure)
{
    unsigned long adc1; // ADC value of the pressure conversion
    unsigned long adc2; // ADC value of the temperature conversion
    double dT; // difference between actual and measured temperature
    double offsetT; // offset at actual temperature
    double senT; // sensitivity at actual temperature

    adc1 = Ms5607_CmdAdc(MS5607_CMD_ADC_D1 + MS5607_CMD_ADC_256); // read uncompensated pressure
    adc2 = Ms5607_CmdAdc(MS5607_CMD_ADC_D2 + MS5607_CMD_ADC_4096); // read uncompensated temperature

    // cal 1st order pressure and temperature (MS5607 1st order algorithm)
    dT = adc2 - s_Ms5607Calibra[5] * pow(2, 8);
    offsetT = s_Ms5607Calibra[2] * pow(2, 17) + dT * s_Ms5607Calibra[4] / pow(2, 6);
    senT = s_Ms5607Calibra[1] * pow(2, 16) + dT * s_Ms5607Calibra[3] / pow(2, 7);

    *temperature = (2000 + (dT * s_Ms5607Calibra[6]) / pow(2, 23)) / 100;
    *pressure = (((adc1 * senT) / pow(2, 21) - offsetT) / pow(2, 15)) / 100;

    return RTN_SUCCESS;
}

/************************ (C) COPYRIGHT DJI Innovations *******END OF FILE******/
