/**
 ******************************************************************************
 * @file    led_button_switch.c
 * @version V1.0.0
 * @date    2017/11/10
 * @brief   The file defines macros related to LED and BUTTON of demo board, and some driver functions of LED and BUTTON.
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
#include "led_button_switch.h"
#include "myinclude.h"
#include "log_printf.h"
#include "flash.h"
#include <os_util.h>


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

//LED1
#define LED1_RCC    RCC_AHB1Periph_GPIOE
#define LED1_PORT   GPIOE
#define LED1_PIN    GPIO_Pin_0

//LED2
#define LED2_RCC    RCC_AHB1Periph_GPIOE
#define LED2_PORT   GPIOE
#define LED2_PIN    GPIO_Pin_1

//LED3
#define LED3_RCC    RCC_AHB1Periph_GPIOE
#define LED3_PORT   GPIOE
#define LED3_PIN    GPIO_Pin_2

//UART
#define UART_SW_RCC         RCC_AHB1Periph_GPIOE
#define UART_SW_PORT        GPIOE
#define UART_SW_PIN         GPIO_Pin_7

//CAN
#define CAN_SW_RCC          RCC_AHB1Periph_GPIOE
#define CAN_SW_PORT         GPIOE
#define CAN_SW_PIN          GPIO_Pin_10

//BUTTON1
#define BUTTON1_RCC         RCC_AHB1Periph_GPIOD
#define BUTTON1_PORT        GPIOD
#define BUTTON1_PIN         GPIO_Pin_5
#define BUTTON1_EXITPORT    EXTI_PortSourceGPIOD
#define BUTTON1_EXITPIN     EXTI_PinSource5
#define BUTTON1_EXITLINE    EXTI_Line5

//BUTTON2
#define BUTTON2_RCC         RCC_AHB1Periph_GPIOD
#define BUTTON2_PORT        GPIOD
#define BUTTON2_PIN         GPIO_Pin_6
#define BUTTON2_EXITPORT    EXTI_PortSourceGPIOD
#define BUTTON2_EXITPIN     EXTI_PinSource6
#define BUTTON2_EXITLINE    EXTI_Line6

//BUTTON external interrupt
#define BUTTTON_IRQn         EXTI9_5_IRQn
#define BUTTON_IRQHandler   EXTI9_5_IRQHandler

//default PSDK communication interface setting
#define DEFAULT_SETTINGS    0xFFFFFFFF

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

//button press callback function
static ButtonPressCallbackFunc s_btPressCallback = NULL;

/* Exported variables --------------------------------------------------------*/

//PSDK communication interface setting variable
extern uint32_t g_can_uart_sw_settings;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
 * @brief LED initialize.
 * @param None.
 * @return None.
 */
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* GPIO Periph clock enable */
    RCC_AHB1PeriphClockCmd(LED1_RCC, ENABLE);
    RCC_AHB1PeriphClockCmd(LED2_RCC, ENABLE);
    RCC_AHB1PeriphClockCmd(LED3_RCC, ENABLE);

    /* Configure GPIO_InitStructure */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

    GPIO_InitStructure.GPIO_Pin = LED1_PIN;
    GPIO_Init(LED1_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LED2_PIN;
    GPIO_Init(LED2_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LED3_PIN;
    GPIO_Init(LED3_PORT, &GPIO_InitStructure);
}

/**
 * @brief Control LED turn on, turn off or toggle.
 * @param led LED controlled.
 * @param eLedOption LED operating option.
 */
void LED_Control(E_LED led, E_LEDOption eLedOption)
{
    GPIO_TypeDef *ledPort;
    uint16_t ledPin;

    if (led == LED1_GREEN) {
        ledPort = LED1_PORT;
        ledPin = LED1_PIN;
    } else if (led == LED2_RED) {
        ledPort = LED2_PORT;
        ledPin = LED2_PIN;
    } else if (led == LED3_BLUE) {
        ledPort = LED3_PORT;
        ledPin = LED3_PIN;
    } else {
        return;
    }

    if (eLedOption == LED_OPT_ON) {
        GPIO_ResetBits(ledPort, ledPin);
    } else if (eLedOption == LED_OPT_OFF){
        GPIO_SetBits(ledPort, ledPin);
    } else if (eLedOption == LED_OPT_TOGGLE) {
        if (GPIO_ReadOutputDataBit(ledPort, ledPin) == 1) {
            GPIO_ResetBits(ledPort, ledPin);
        } else {
            GPIO_SetBits(ledPort, ledPin);
        }
    }
}

/**
 * @brief UART initialize.
 * @param None.
 * @return None.
 */
void UART_SW_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* GPIO Periph clock enable */
    RCC_AHB1PeriphClockCmd(UART_SW_RCC, ENABLE);

    /* Configure GPIO_InitStructure */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin = UART_SW_PIN;

    GPIO_Init(UART_SW_PORT, &GPIO_InitStructure);
}

/**
 * @brief UART open.
 * @param None.
 * @return None.
 */
void UART_SW_On(void)
{
    GPIO_SetBits(UART_SW_PORT, UART_SW_PIN);
}

/**
 * @brief UART close.
 * @param None.
 * @return None.
 */
void UART_SW_Off(void)
{
    GPIO_ResetBits(UART_SW_PORT, UART_SW_PIN);
}

/**
 * @brief CAN initialize.
 * @param None.
 * @return None.
 */
void CAN_SW_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* GPIO Periph clock enable */
    RCC_AHB1PeriphClockCmd(CAN_SW_RCC, ENABLE);

    /* Configure GPIO_InitStructure */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin = CAN_SW_PIN;

    GPIO_Init(CAN_SW_PORT, &GPIO_InitStructure);
}

/**
 * @brief CAN open.
 * @param None.
 * @return None.
 */
void CAN_SW_On(void)
{
    GPIO_SetBits(CAN_SW_PORT, CAN_SW_PIN);
}

/**
 * @brief CAN close.
 * @param None.
 * @return None.
 */
void CAN_SW_Off(void)
{
    GPIO_ResetBits(CAN_SW_PORT, CAN_SW_PIN);
}

/**
 * @brief Control the UART and CAN open or close.
 * @param None.
 * @return None.
 */
void Button_StartUpdate(void)
{
    if (g_can_uart_sw_settings == DEFAULT_SETTINGS)
    {
        UART_SW_On();
        CAN_SW_On();
    }
    else
    {
        UART_SW_Off();
        CAN_SW_Off();
    }
}

/**
 * @brief BUTTON press callback function.
 * @details Change the PSDK communication interface setting.
 * @param button BUTTON be pressed.
 * @return None.
 */
void Button_PressCallback(E_Button button)
{
    static uint32_t lastButtion2Time = 0;
    
    if (button == BUTTON1)
    {
        //reserved
    }
    else if (button == BUTTON2)
    {
        if (OS_GET_TIME_MS - lastButtion2Time < 50) {
            return ;
        } else {
            lastButtion2Time = OS_GET_TIME_MS;
        }

        g_can_uart_sw_settings = ~g_can_uart_sw_settings;
    }
}

/**
 * @brief LED initialize.
 * @param None.
 * @return None.
 */
void Button_Init(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    s_btPressCallback = Button_PressCallback;

    /* GPIO Periph clock enable */
    RCC_AHB1PeriphClockCmd(BUTTON1_RCC, ENABLE);
    RCC_AHB1PeriphClockCmd(BUTTON2_RCC, ENABLE);
    /* Enable SYSCFG clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    /* Configure PA0 pin as input floating */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

    GPIO_InitStructure.GPIO_Pin = BUTTON1_PIN;
    GPIO_Init(BUTTON1_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = BUTTON2_PIN;
    GPIO_Init(BUTTON2_PORT, &GPIO_InitStructure);

    /* Connect EXTI pin */
    SYSCFG_EXTILineConfig(BUTTON1_EXITPORT, BUTTON1_EXITPIN);
    SYSCFG_EXTILineConfig(BUTTON2_EXITPORT, BUTTON2_EXITPIN);

    /* Configure EXTI Line */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;

    EXTI_InitStructure.EXTI_Line = BUTTON1_EXITLINE;
    EXTI_Init(&EXTI_InitStructure);

    EXTI_InitStructure.EXTI_Line = BUTTON2_EXITLINE;
    EXTI_Init(&EXTI_InitStructure);

    /* Enable and set EXTI Line0 Interrupt to the lowest priority */
    NVIC_InitStructure.NVIC_IRQChannel = BUTTTON_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief BUTTON press interrupt request handler function .
 * @param None.
 * @return None.
 */
void BUTTON_IRQHandler(void)
{
    if (EXTI_GetITStatus(BUTTON1_EXITLINE) != RESET)
    {
        /* button1 */
        if(NULL != s_btPressCallback)
            s_btPressCallback(BUTTON1);

        /* Clear the EXTI line pending bit */
        EXTI_ClearITPendingBit(BUTTON1_EXITLINE);
    }
    else if (EXTI_GetITStatus(BUTTON2_EXITLINE) != RESET)
    {
        /* button2 */
        if(NULL != s_btPressCallback)
            s_btPressCallback(BUTTON2);

        /* Clear the EXTI line pending bit */
        EXTI_ClearITPendingBit(BUTTON2_EXITLINE);
    }
}

/************************ (C) COPYRIGHT DJI Innovations *******END OF FILE******/
