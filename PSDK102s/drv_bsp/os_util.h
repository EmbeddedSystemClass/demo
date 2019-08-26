/**
 ******************************************************************************
 * @file    os_util.h
 * @version V1.0.0
 * @date    2017/11/10
 * @brief   The file defines some operate related to OS.
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
#ifndef M4_TANSFERRING_OS_UTIL_H
#define M4_TANSFERRING_OS_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "psdk_arch_sys.h"

#if PSDK_ARCH_USE_OS
#include <cmsis_os.h>
#include <FreeRTOS.h>

/* Exported constants --------------------------------------------------------*/
#define OS_GET_TIME_MS  (osKernelSysTick() * 1000 / osKernelSysTickFrequency)
#define OS_MALLOC       pvPortMalloc
#define OS_FREE         vPortFree
#define OS_DELAY        osDelay
#else
#include "sys.h"
	
/* Exported constants --------------------------------------------------------*/	
#define OS_GET_TIME_MS Sys_GetTick()
#define OS_MALLOC       malloc
#define OS_FREE         free
#define OS_DELAY        Sys_Delay
#endif

/* Exported macros -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif //M4_TANSFERRING_OS_UTIL_H

/****************** (C) COPYRIGHT DJI Innovations *****END OF FILE****/
