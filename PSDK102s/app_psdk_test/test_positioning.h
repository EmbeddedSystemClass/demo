/**
 ******************************************************************************
 * @file    test_positioning.h
 * @version V1.5.0
 * @date    2019/07/31
 * @brief   This is the header file for "test_positioning.c".
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

#ifndef TEST_POSITIONING_H
#define TEST_POSITIONING_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <psdk.h>

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

E_PsdkStat
Test_VerifyUtcTimestamp(uint32_t utcTimestampComeTimeMs, uint32_t ppsNewestTriggerTimeMs, uint8_t *effectiveFlag);
E_PsdkStat Test_SyncTime(T_UtcTimestampMsg utcTimestamp, uint32_t ppsNewestTriggerTimeMs);
E_PsdkStat Test_RequestPosition(T_PsdkUpper *psdkUpper,uint32_t localTime);
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
extern  T_PsdkPositioningGetPosReq utc_time;
extern  T_PsdkPositioningGetPosAck writesdpos;
#ifdef __cplusplus
}
#endif

#endif //TEST_POSITIONING_H

/****************** (C) COPYRIGHT DJI Innovations *****END OF FILE****/
