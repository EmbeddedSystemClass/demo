/**
 ******************************************************************************
 * @file    psdk_arch_sys.h
 * @version V1.5.0
 * @date    2019/07/31
 * @brief   This file contains PSDK system wrapper functions prototypes for FreeRTOS and Cmsis wrapper,
 *          and define printf function for log, define memory functions.
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
#ifndef PSDK_ARCH_SYS_H
#define PSDK_ARCH_SYS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "psdk_typedef.h"
#include <cmsis_os.h>
#include <FreeRTOS.h>

// Note: log_printf.h are self-defined, rather than FreeRTOS library.
#include <log_printf.h>

/* Exported constants --------------------------------------------------------*/

//set use operating system or not, this setting can be value 1 or 0.
#define PSDK_ARCH_USE_OS            1       //use operating system

/* Exported macros -----------------------------------------------------------*/

//set printf function for log; if use operating system, must be thread safe
#define PSDK_DBG_PRINTF             LOG

//set malloc & free function; if use operating system, must be thread safe
#define PSDK_MEM_MALLOC             pvPortMalloc
#define PSDK_MEM_FREE               vPortFree

#if PSDK_ARCH_USE_OS
//define mutex type
#define PSDK_MUTEX_T                osMutexId
//define semaphore type
#define PSDK_SEMAPHORE_T            osSemaphoreId
#else
//define get system time in milliseconds interface
#define PSDK_GET_TIME_MS
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

#if PSDK_ARCH_USE_OS
E_PsdkStat PsdkSys_MutexNew(PSDK_MUTEX_T *mutex);
E_PsdkStat PsdkSys_MutexDestroy(PSDK_MUTEX_T *mutex);
E_PsdkStat PsdkSys_MutexLock(PSDK_MUTEX_T *mutex);
E_PsdkStat PsdkSys_MutexUnlock(PSDK_MUTEX_T *mutex);
E_PsdkStat PsdkSys_SemaphoreNew(PSDK_SEMAPHORE_T *semaphore, uint32_t count);
E_PsdkStat PsdkSys_SemaphoreDestroy(PSDK_SEMAPHORE_T *semaphore);
E_PsdkStat PsdkSys_SemaphoreTimedWait(PSDK_SEMAPHORE_T *semaphore, uint32_t waitTime);
E_PsdkStat PsdkSys_SemaphorePost(PSDK_SEMAPHORE_T *semaphore);
#endif

/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif //PSDK_ARCH_SYS_H

/************************ (C) COPYRIGHT DJI Innovations *******END OF FILE******/
