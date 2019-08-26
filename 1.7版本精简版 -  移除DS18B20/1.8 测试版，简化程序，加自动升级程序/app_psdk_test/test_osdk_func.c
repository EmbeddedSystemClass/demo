/**
 ******************************************************************************
 * @file    test_osdk_func.c
 * @version V1.5.0
 * @date    2019/07/31
 * @brief   The file define PSDK OSDK function command callback functions.
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
#include "test_osdk_func.h"
#include "string.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief Receive data from OSDK callback function.
 * @param pData Pointer to data received.
 * @param dataLen Length of data received.
 * @return None.
 */
static void Test_ReceiveFromOnboardSDKFunc(const uint8_t *pData, uint16_t dataLen)
{
    uint8_t s_printBuffer[OSDKFUNC_OSDK_TO_PSDK_MAX_LEN + 1];

    if (dataLen <= OSDKFUNC_OSDK_TO_PSDK_MAX_LEN) {
        memcpy(s_printBuffer, pData, dataLen);
        s_printBuffer[dataLen] = 0;
        PSDK_LOG_DEBUG("Receive from OSDK: %s", s_printBuffer);
    }
}

//OSDK communication related command callback function list
const T_PsdkOsdkFuncHandler g_testOsdkFuncHandler = {
        .ReceiveFromOsdkCallback = Test_ReceiveFromOnboardSDKFunc,
};

/* Exported functions --------------------------------------------------------*/

/****************** (C) COPYRIGHT DJI Innovations *****END OF FILE****/
