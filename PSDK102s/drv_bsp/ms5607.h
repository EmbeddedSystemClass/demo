/**
 ******************************************************************************
 * @file    ms5607.h
 * @version V1.0.0
 * @date    2017/11/10
 * @brief   This is the header file for "ms5607.c".
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DEMO_RTOS_RTTHREAD_MS5607_H
#define DEMO_RTOS_RTTHREAD_MS5607_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <os_util.h>

/* Exported constants --------------------------------------------------------*/
#define MS5607_SPI                              SPI2
#define MS5607_SPI_CLK                          RCC_APB1Periph_SPI2
#define MS5607_SPI_CLK_INIT                     RCC_APB1PeriphClockCmd

#define MS5607_SPI_SCK_PIN                      GPIO_Pin_13
#define MS5607_SPI_SCK_GPIO_PORT                GPIOB
#define MS5607_SPI_SCK_GPIO_CLK                 RCC_AHB1Periph_GPIOB
#define MS5607_SPI_SCK_SOURCE                   GPIO_PinSource13
#define MS5607_SPI_SCK_AF                       GPIO_AF_SPI2

#define MS5607_SPI_MISO_PIN                     GPIO_Pin_14
#define MS5607_SPI_MISO_GPIO_PORT               GPIOB
#define MS5607_SPI_MISO_GPIO_CLK                RCC_AHB1Periph_GPIOB
#define MS5607_SPI_MISO_SOURCE                  GPIO_PinSource14
#define MS5607_SPI_MISO_AF                      GPIO_AF_SPI2

#define MS5607_SPI_MOSI_PIN                     GPIO_Pin_15
#define MS5607_SPI_MOSI_GPIO_PORT               GPIOB
#define MS5607_SPI_MOSI_GPIO_CLK                RCC_AHB1Periph_GPIOB
#define MS5607_SPI_MOSI_SOURCE                  GPIO_PinSource15
#define MS5607_SPI_MOSI_AF                      GPIO_AF_SPI2

#define MS5607_CS_GPIO_PORT                     GPIOB
#define MS5607_CS_PIN                           GPIO_Pin_12
#define MS5607_CS_GPIO_CLK                      RCC_AHB1Periph_GPIOB

#define MS5607_CMD_RESET                        0x1E // ADC reset command
#define MS5607_CMD_ADC_READ                     0x00 // ADC read command
#define MS5607_CMD_ADC_CONV                     0x40 // ADC conversion command
#define MS5607_CMD_ADC_D1                       0x00 // ADC D1 conversion
#define MS5607_CMD_ADC_D2                       0x10 // ADC D2 conversion
#define MS5607_CMD_ADC_256                      0x00 // ADC OSR=256
#define MS5607_CMD_ADC_512                      0x02 // ADC OSR=512
#define MS5607_CMD_ADC_1024                     0x04 // ADC OSR=1024
#define MS5607_CMD_ADC_2048                     0x06 // ADC OSR=2056
#define MS5607_CMD_ADC_4096                     0x08 // ADC OSR=4096
#define MS5607_CMD_PROM_RD                      0xA0 // Prom read command

/* Exported macros -----------------------------------------------------------*/
#define MS5607_CSB_HIGH()                       (GPIO_SetBits(MS5607_CS_GPIO_PORT, MS5607_CS_PIN)) // setting CSB high
#define MS5607_CSB_LOW()                        (GPIO_ResetBits(MS5607_CS_GPIO_PORT, MS5607_CS_PIN)) // setting CSB low

#define MS5607_DELAY(msT)                       OS_DELAY(msT)

/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void Ms5607Spi_Init(void);
int Ms5607_Init(void);
int Ms5607_GetSenorData(double *temperature, double *pressure);
int testMs5607(void);

/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif //DEMO_RTOS_RTTHREAD_MS5607_H

/************************ (C) COPYRIGHT DJI Innovations *******END OF FILE******/
