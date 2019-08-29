/**
 ******************************************************************************
 * @file    psdk_arch_sys.c
 * @version V1.5.0
 * @date    2019/07/31
 * @brief   
 * This file provides the PSDK wrapper functions for inter-thread communication
 * and resource sharing with mutexes under FreeRTOS and CMSIS-RTOS.
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
#include "psdk_debug.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

#if PSDK_ARCH_USE_OS

/**
 * @brief Declare the mutex container, initialize the mutex, and
 * create mutex ID.
 * @param mutex:  pointer to the created mutex ID.
 * @return an enum that represents a status of PSDK
 */
E_PsdkStat PsdkSys_MutexNew(PSDK_MUTEX_T *mutex)
{
    osMutexDef(PSDK_MUTEX);

    *mutex = osMutexCreate(osMutex(PSDK_MUTEX));
    if (*mutex == NULL) {
        return PSDK_STAT_ERR;
    }

    return PSDK_STAT_OK;
}

/**
 * @brief Delete the created mutex.
 * @param mutex:  pointer to the created mutex ID.
 * @return an enum that represents a status of PSDK
 */
E_PsdkStat PsdkSys_MutexDestroy(PSDK_MUTEX_T *mutex)
{
    PSDK_ASSERT(mutex != NULL);

    if (osMutexDelete(*mutex) != osOK) {
        return PSDK_STAT_ERR;
    }

    return PSDK_STAT_OK;
}

/**
 * @brief Acquire and lock the mutex when peripheral access is required
 * @param mutex:  pointer to the created mutex ID.
 * @return an enum that represents a status of PSDK
 */
E_PsdkStat PsdkSys_MutexLock(PSDK_MUTEX_T *mutex)
{
    PSDK_ASSERT(mutex != NULL);

    if (osMutexWait(*mutex, osWaitForever) != osOK) {
        return PSDK_STAT_ERR;
    }

    return PSDK_STAT_OK;
}

/**
 * @brief Unlock and release the mutex, when done with the peripheral access.
 * @param mutex:  pointer to the created mutex ID.
 * @return an enum that represents a status of PSDK
 */
E_PsdkStat PsdkSys_MutexUnlock(PSDK_MUTEX_T *mutex)
{
    PSDK_ASSERT(mutex != NULL);

    if (osMutexRelease(*mutex) != osOK) {
        return PSDK_STAT_ERR;
    }

    return PSDK_STAT_OK;
}

/**
 * @brief Declare the semaphore container, initialize the semaphore, and
 * create semaphore ID.
 * @param semaphore: pointer to the created semaphore ID.
 * @param count: initial value of semaphore.
 * @return an enum that represents a status of PSDK
 */
E_PsdkStat PsdkSys_SemaphoreNew(PSDK_SEMAPHORE_T *semaphore, uint32_t count)
{
    osSemaphoreDef(PSDK_SEMAPHORE);

    *semaphore = osSemaphoreCreate(osSemaphore(PSDK_SEMAPHORE), (int32_t) count);
    if (*semaphore == NULL) {
        return PSDK_STAT_ERR;
    }

    return PSDK_STAT_OK;
}

/**
 * @brief Delete the created semaphore.
 * @param semaphore: pointer to the created semaphore ID.
 * @return an enum that represents a status of PSDK
 */
E_PsdkStat PsdkSys_SemaphoreDestroy(PSDK_SEMAPHORE_T *semaphore)
{
    PSDK_ASSERT(semaphore != NULL);

    if (osSemaphoreDelete(*semaphore) != osOK) {
        return PSDK_STAT_ERR;
    }

    return PSDK_STAT_OK;
}

/**
 * @brief Wait the semaphore until token becomes available.
 * @param semaphore: pointer to the created semaphore ID.
 * @param waitTime: timeout value of waiting semaphore, unit: millisecond.
 * @return an enum that represents a status of PSDK
 */
E_PsdkStat PsdkSys_SemaphoreTimedWait(PSDK_SEMAPHORE_T *semaphore, uint32_t waitTime)
{
    PSDK_ASSERT(semaphore != NULL);

    if (osSemaphoreWait(*semaphore, waitTime) != osOK) {
        return PSDK_STAT_ERR;
    }

    return PSDK_STAT_OK;
}

/**
 * @brief Release the semaphore token.
 * @param semaphore: pointer to the created semaphore ID.
 * @return an enum that represents a status of PSDK
 */
E_PsdkStat PsdkSys_SemaphorePost(PSDK_SEMAPHORE_T *semaphore)
{
    PSDK_ASSERT(semaphore != NULL);

    if (osSemaphoreRelease(*semaphore) != osOK) {
        return PSDK_STAT_ERR;
    }

    return PSDK_STAT_OK;
}

#endif

/************************ (C) COPYRIGHT DJI Innovations *******END OF FILE******/
