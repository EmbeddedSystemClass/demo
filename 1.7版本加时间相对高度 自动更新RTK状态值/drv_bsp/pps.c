/**
 ******************************************************************************
 * @file    pps.c
 * @version V1.4.0
 * @date    2019/1/15
 * @brief   The file defines PPS signal related bottom layer functions.
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
#include "pps.h"
#include "log_printf.h"
#include "flash.h"
#include <os_util.h>
#include "psdk_debug.h"
#include "myinclude.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define PPS_RCC          RCC_APB2Periph_GPIOB
#define PPS_PORT         GPIOB
#define PPS_PIN          GPIO_Pin_3
//#define PPS_EXTIPORT    EXTI_PortSourceGPIOB
//#define PPS_EXTIPIN     EXTI_PinSource3
#define PPS_EXTILINE    EXTI_Line3
#define PPS_IRQn        EXTI3_IRQn
uint32_t ppssignal=0;
uint32_t ppsNewestTriggerTimeMs = 0;
uint32_t ppsNewTimeMs = 0;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//precise survey PPS signal semaphore and global variables
#if PSDK_ARCH_USE_OS
//static osSemaphoreId s_ppsSem = NULL;
#else
static uint8_t s_ppsSig = 0;
#endif

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/





/**
 * @brief PPS line initialize.
 * @param None.
 * @return None.
 */
void PPS_Init(void)
{
	  EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		GPIO_InitTypeDef GPIO_InitStructure;
	

  
}

/**
 * @brief Check whether PPS signal be triggered once.
 * @param None.
 * @return None.
 */
bool PPS_isTriggered(void)
{
	  if(ppssignal==1)
		{
			ppssignal=0;
			return true;
		}else
		{
			return false;
		}
}
/**
 * @brief Get the newest PPS signal trigger time.
 * @param None.
 * @retval 0: ineffective value, specifies PPS signal have not been triggered.
 * not 0: the newest PPS signal trigger time.
 */
uint32_t PPS_GetNewestTriggerTime(void)
{
    return ppsNewestTriggerTimeMs;
}
/**
 * @brief PPS signal interrupt request handler function.
 * @param None.
 * @return None.
 */
#define PPS_IRQHandler  EXTI3_IRQHandler
void PPS_IRQHandler(void)
{
    if (EXTI_GetITStatus(PPS_EXTILINE) != RESET) 
		{ 
			  ppssignal=1;
        ppsNewestTriggerTimeMs = OS_GET_TIME_MS;
			 	ppsNewTimeMs= OS_GET_TIME_MS;
        /* Clear the EXTI line pending bit */
        EXTI_ClearITPendingBit(PPS_EXTILINE);
				
    }
}

/****************** (C) COPYRIGHT DJI Innovations *****END OF FILE****/

