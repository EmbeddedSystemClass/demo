/**
 ******************************************************************************
 * @file    test_msg_push.c
 * @version V1.5.0
 * @date    2019/07/31
 * @brief   The file define PSDK message push command callback functions for demo board.
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
#include <psdk.h>
#include <psdk_cmdset_define/psdk_cmdset_msg_push.h>
#include "test_positioning.h"
#include "os_util.h"
#include "pps.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
//static osSemaphoreId s_utcSem = NULL;
/**
 * @brief PSDK data transmission bandwidth message push command callback function.
 * @param bandWidthMsg data transmission bandwidth message structure.
 * @return None.
 */
static void bandWidthMsgCallback(const T_BandWidthMsg *bandWidthMsg)
{
//    PSDK_LOG_DEBUG("Receive bandwidth message");
}

/**
 * @brief PSDK UAV attitude message push command callback function.
 * @param uavAttitudeMsg UAV attitude message structure.
 * @return None.
 */
static void uavAttitudeMsgCallback(const T_UavAttitudeMsg *uavAttitudeMsg)
{
//    PSDK_LOG_DEBUG("Receive Uav AttitudeMsg");
}

/**
 * @brief PSDK battery information push command callback function.
 * @param batteryInfoMsg UAV battery information structure.
 * @param batteryInfoAck Battery information push command ack structure, specifies payload power-off ready state.
 * @return None.
 */
static void batteryInfoMsgCallback(const T_BatteryInfoMsg *batteryInfoMsg, T_BatteryInfoAck *batteryInfoAck)
{
//    PSDK_LOG_DEBUG("Receive Battery Info Msg");

    /*
     * Users can add power off preparation work here, but if execution time of this part is too long,
     * please put it to anther task.
     */
    batteryInfoAck->ackCode = PSDK_CMD_ACK_CODE_OK;
    batteryInfoAck->powerOffReady = PSDK_PAYLOAD_POWER_OFF_READY;
}

/**
 * @brief PSDK GPS information push command callback function.
 * @param gpsInfoMsg GPS information structure.
 * @return None.
 */
static void gpsInfoMsgCallback(const T_GPSInfoMsg *gpsInfoMsg)
{
//    PSDK_LOG_DEBUG("Receive gps info msg");
}

/**
 * @brief PSDK UAV state message push command callback function.
 * @param uavStateMsg UAV state message structure.
 * @return None.
 */
static void uavStateMsgCallback(const T_UavStateMsg *uavStateMsg)
{
//    PSDK_LOG_DEBUG("Receive Uav state Msg");
}

/**
 * @brief PSDK mobile APP date and time message push command callback function.
 * @param appDateTimeMsg Mobile APP date and time message structure.
 * @return None.
 */
static void appDateTimeMsgCallback(const T_AppDateTimeMsg *appDateTimeMsg)
{
//    PSDK_LOG_DEBUG("Receive AppdateTime Msg");
}

/**
 * @brief PSDK barometer height data message push command callback function.
 * @param baroHeightDataMsg Barometer height data message structure.
 * @return None.
 */
static void baroHeightDataMsgCallback(const T_baroHeightDataMsg *baroHeightDataMsg)
{
//    PSDK_LOG_DEBUG("Receive BaroHeight Msg");
}

/**
 * @brief PSDK GPS raw data push command callback function.
 * @param gpsRawDataMsg GPS raw data structure.
 * @return None.
 */
static void gpsRawDataMsgCallback(const T_GpsRawDataMsg *gpsRawDataMsg)
{
//    PSDK_LOG_DEBUG("Receive GpsRawData Msg");
}

/**
 * @brief PSDK RTK raw data push command callback function.
 * @param rtkRawDataMsg RTK raw data structure.
 * @return None.
 */
static void rtkRawDataMsgCallback(const T_RtkRawDataMsg *rtkRawDataMsg)
{
//    PSDK_LOG_DEBUG("Receive RtkRawData Msg");
}

/**
 * @brief PSDK UTC timestamp push command callback function.
 * @param utcTimestampMsg UTC timestamp data structure.
 * @return None.
 */
static void utcTimestampMsgCallback(const T_UtcTimestampMsg *utcTimestampMsg)
{
    E_PsdkStat psdkStat;
    uint32_t currentLocalTime = OS_GET_TIME_MS;
    uint32_t ppsNewestTriggerTimeMs = 0;
    uint8_t utcTimestampEffectFlag = false;

 //   PSDK_LOG_DEBUG("Receive UtcTimestamp Msg");
 //   PSDK_LOG_DEBUG("Second is %d, Microsecond is %d.", utcTimestampMsg->second, utcTimestampMsg->microsecond);

    ppsNewestTriggerTimeMs = PPS_GetNewestTriggerTime();
		
    if (ppsNewestTriggerTimeMs == 0) {
 //       PSDK_LOG_DEBUG("PPS Signal have not been Triggered.");
        return;
    }

    psdkStat = Test_VerifyUtcTimestamp(currentLocalTime, ppsNewestTriggerTimeMs, &utcTimestampEffectFlag);   //此处很有可能
    if (psdkStat != PSDK_STAT_OK || utcTimestampEffectFlag != true) {
 //       PSDK_LOG_DEBUG("UTC Timestamp Package is Ineffective.");
        return;
    }

    Test_SyncTime(*utcTimestampMsg, ppsNewestTriggerTimeMs);  //将GET TIM MS传递给这个函数
}

/**
 * @brief PSDK type of other payload mounted on UAV push command callback function.
 * @param otherPayloadTypeMsg other payload type message structure.
 * @return None.
 */
static void otherPayloadTypeMsgCallback(const T_OtherPayloadTypeMsg *otherPayloadTypeMsg)
{
//    PSDK_LOG_DEBUG("Receive OtherPayloadType Msg");
}

/**
 * @brief PSDK focal length of other payload mounted on UAV push command callback function.
 * @param otherPayloadFocalLengthMsg other payload focal length message structure.
 * @return None.
 */
static void otherPayloadFocalLengthMsgCallback(const T_OtherPayloadFocalLengthMsg *otherPayloadFocalLengthMsg)
{
//    PSDK_LOG_DEBUG("Receive OtherPayloadFocalLength Msg.");
}

//message subscribe function list
const T_PsdkPushDataMsgCallbackList g_PushDataMsgCallbackList = {
        .bandWidthMsgCallback = bandWidthMsgCallback,
        .uavAttitudeMsgCallback = uavAttitudeMsgCallback,
        .batteryInfoMsgCallback = batteryInfoMsgCallback,
        .gpsInfoMsgCallback = gpsInfoMsgCallback,
        .uavStateMsgCallback = uavStateMsgCallback,
        .appDateTimeMsgCallback = appDateTimeMsgCallback,
        .baroHeightDataMsgCallback = baroHeightDataMsgCallback,
        .gpsRawDataMsgCallback = gpsRawDataMsgCallback,
        .rtkRawDataMsgCallback = rtkRawDataMsgCallback,
        .utcTimestampMsgCallback = utcTimestampMsgCallback,
        .otherPayloadTypeMsgCallback = otherPayloadTypeMsgCallback,
        .otherPayloadFocalLengthMsgCallback = otherPayloadFocalLengthMsgCallback,
};
//static void utcTsMsgCallback(const T_UtcTimestampMsg *utcTsMsg)
//{
////    //PSDK_LOG_DEBUG("Receive UtcTimestamp Msg");//COCOGUOJIA
////    memcpy(&s_utcTime, utcTsMsg, sizeof(T_UtcTsMsg));
//#if PSDK_ARCH_USE_OS
//    osSemaphoreRelease(s_utcSem);
//#else
//    s_utcSig = 1;
//#endif
//}
//void pceSurveyInit(void)
//{
//#if PSDK_ARCH_USE_OS
//    osSemaphoreDef(utc);
//    s_utcSem = osSemaphoreCreate(osSemaphore(utc), 1);
//    osSemaphoreWait(s_utcSem, 0);
//#endif
//}

//bool pceIsUtcPushed(void)    //外加此函数
//{
//#if PSDK_ARCH_USE_OS //设置是否使用操作系统，此设置可以为值1或0
//    if(osSemaphoreWait(s_utcSem, 0) == osOK)//等待信号量令牌可用 
//		{      
//        return true;
//    }
//#else
//        if(s_utcSig == 1) {
//        s_utcSig = 0;
//        return true;
//    }
//#endif
//    else {
//        return false;
//    }
//}
/****************** (C) COPYRIGHT DJI Innovations *****END OF FILE****/
