/**
 ******************************************************************************
 * @file    test_positioning.c
 * @version V1.5.0
 * @date    2019/07/31
 * @brief   The file simulates positioning functions for demo board.
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
#include "test_positioning.h"
#include "string.h"
#include "os_util.h"
#include "pps.h"
#include "time.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct {
    T_UtcTimestampMsg utcTime;
    uint32_t localTimeMs;
} T_SyncedTime;

/* Private define ------------------------------------------------------------*/
#define TIME_SYNC_EFFECTIVE_TIME_INTERVAL   500     // unit: ms
#define PAYLOAD_TARGET_POINT_COUNT          1
#define TIME_OFFSET_MAX_VALUE               2000000 // unit: us
uint8_t sdsign=0; 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static T_SyncedTime syncedTime;
static bool syncedTimeExist = false;
static uint32_t eventIndex[PAYLOAD_TARGET_POINT_COUNT] = {0};
uint8_t timesigner=0;
double longitude=0;
double latitude=0;
double height=0;
T_PsdkPositioningGetPosAck writesdpos;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief Transfer local time to UTC time based on synced time base. 基于同步时基将本地时间转换为UTC时间。
 * @param syncedTimeVar: synced time base.
 * @param localTimeMs: local time to be transferred.
 * @param utcTime: Pointer to structure to store UTC time transferred.
 * @return Function process result.
 */
static E_PsdkStat Test_TransferTimeFromLocalToUtc(T_SyncedTime syncedTimeVar, uint32_t localTimeMs,
                                                  T_UtcTimestampMsg *utcTime)
{
	  uint8_t i=0,a=0;
    int32_t localTimeDiffUs = 0;
    int32_t localTimeDiffS = 0;
    int32_t tempUtcTimeUs = 0;
    struct tm syncedUtcTimeTm = {0};
    struct tm transferredUtcTimeTm = {0};
    struct tm *pTempTransferredUtcTimeTm = NULL;
    time_t syncedUtcTimeTimet = 0;
    time_t transferredUtcTimeTimet = 0;

    if (utcTime == NULL)
		{
      return PSDK_STAT_ERR;
			PSDK_LOG_DEBUG("PSDK_STAT_ERR1 %d\r\n");
		}
		a=localTimeMs-syncedTimeVar.localTimeMs;
    localTimeDiffUs = ((int32_t) localTimeMs - (int32_t) syncedTimeVar.localTimeMs) * 1000;
//	  PSDK_LOG_DEBUG("shijian  %dms  %dms  %dms",localTimeMs,syncedTimeVar.localTimeMs,localTimeMs-syncedTimeVar.localTimeMs);
    tempUtcTimeUs = (localTimeDiffUs + (int32_t) syncedTimeVar.utcTime.microsecond) % 1000000;
    utcTime->microsecond = tempUtcTimeUs < 0 ? (1000000 - tempUtcTimeUs) : tempUtcTimeUs;
    localTimeDiffS =
            ((localTimeDiffUs + (int32_t) syncedTimeVar.utcTime.microsecond) - (int32_t) utcTime->microsecond) /
            1000000;

    syncedUtcTimeTm.tm_year = syncedTimeVar.utcTime.year - 1900;
    syncedUtcTimeTm.tm_mon = syncedTimeVar.utcTime.month;
    syncedUtcTimeTm.tm_mday = syncedTimeVar.utcTime.day;
    syncedUtcTimeTm.tm_hour = syncedTimeVar.utcTime.hour;
    syncedUtcTimeTm.tm_min = syncedTimeVar.utcTime.minute;
    syncedUtcTimeTm.tm_sec = syncedTimeVar.utcTime.second;
    syncedUtcTimeTm.tm_isdst = -1;
    syncedUtcTimeTimet = mktime(&syncedUtcTimeTm);
    if (syncedUtcTimeTimet == (time_t) -1)
		{
        return PSDK_STAT_ERR;
				PSDK_LOG_DEBUG("PSDK_STAT_ERR2 %d\r\n");
		}
		 
    transferredUtcTimeTimet = syncedUtcTimeTimet + localTimeDiffS;
//		PSDK_LOG_DEBUG("geilocaltime  %dms",transferredUtcTimeTimet);
    pTempTransferredUtcTimeTm = localtime_r(&transferredUtcTimeTimet, &transferredUtcTimeTm);
    if (pTempTransferredUtcTimeTm == NULL)
		{
        return PSDK_STAT_ERR;
				PSDK_LOG_DEBUG("PSDK_STAT_ERR3 %d\r\n");
		}
    utcTime->year = transferredUtcTimeTm.tm_year + 1900;
    utcTime->month = transferredUtcTimeTm.tm_mon;
    utcTime->day = transferredUtcTimeTm.tm_mday;
    utcTime->hour = transferredUtcTimeTm.tm_hour;
    utcTime->minute = transferredUtcTimeTm.tm_min;
    utcTime->second = transferredUtcTimeTm.tm_sec;

		
//    PSDK_LOG_DEBUG("Transferred Utc Time is %d.%d.%d %d:%d %d.%d", utcTime->year, utcTime->month,
//                   utcTime->day, utcTime->hour, utcTime->minute, utcTime->second, utcTime->microsecond);  //微秒打印出来的只有六位

    return PSDK_STAT_OK;
}

/* Exported functions --------------------------------------------------------*/

/**
 * @brief Verify whether UTC timestamp package is effective.
 * @param utcTimestampComeTimeMs Time when UTC timestamp package comes.
 * @param ppsNewestTriggerTimeMs Time when the newest PPS signal be triggered.
 * @param effectiveFlag Flag specifies whether UTC timestamp package is effective.
 * @return Function process result.
 */
E_PsdkStat
Test_VerifyUtcTimestamp(uint32_t utcTimestampComeTimeMs, uint32_t ppsNewestTriggerTimeMs, uint8_t *effectiveFlag)
{
    if (effectiveFlag == NULL)
        return PSDK_STAT_ERR_PARAM;

    if (utcTimestampComeTimeMs > ppsNewestTriggerTimeMs &&
        (utcTimestampComeTimeMs - ppsNewestTriggerTimeMs) < TIME_SYNC_EFFECTIVE_TIME_INTERVAL)
        *effectiveFlag = true;
    else
        *effectiveFlag = false;

    return PSDK_STAT_OK;
}

/**
 * @brief Sync UTC time and local time when PPS signal be triggered and write to syncedTime.
 * @param utcTimestamp UTC time stamp.
 * @param ppsNewestTriggerTimeMs Time when the newest PPS signal be triggered.
 * @return Function process result.
 */
E_PsdkStat Test_SyncTime(T_UtcTimestampMsg utcTimestamp, uint32_t ppsNewestTriggerTimeMs)  //外部函数调用 将本地时间传递给它
{
    syncedTime.utcTime = utcTimestamp;  
    syncedTime.localTimeMs = ppsNewestTriggerTimeMs;
    syncedTimeExist = true;

    return PSDK_STAT_OK;
}

/**
 * @brief Request position of target points.
 * @param psdkUpper Pointer to the top level PSDK structure.
 * @return Function process result.
 */
 T_PsdkPositioningGetPosReq utc_time = {0};
E_PsdkStat Test_RequestPosition(T_PsdkUpper *psdkUpper,uint32_t localTime)
{
    int32_t i = 0;
    uint32_t currentLocalTime = 0;
    uint32_t tempTimeOfsUs = 0;
    T_UtcTimestampMsg currentUtcTime = {0};
    T_PsdkPositioningGetPosReq getPosReq = {0};
    T_PsdkPositioningGetPosAck getPosAck = {0};
    E_PsdkStat psdkStat = PSDK_STAT_ERR;
				
						if (syncedTimeExist != true)  
						{			
//								PSDK_LOG_DEBUG("PSDK_eist %d\r\n");
								return PSDK_STAT_ERR;
						}
						currentLocalTime = syncedTime.localTimeMs;
						Test_TransferTimeFromLocalToUtc(syncedTime, localTime, &currentUtcTime);     //本地时间转utc
						getPosReq.targetPointCount = PAYLOAD_TARGET_POINT_COUNT;
						getPosReq.taskId = 0;
						getPosReq.baseTimestamp.year = currentUtcTime.year;
						getPosReq.baseTimestamp.month = currentUtcTime.month;
						getPosReq.baseTimestamp.day = currentUtcTime.day;
						getPosReq.baseTimestamp.hour = currentUtcTime.hour;
						getPosReq.baseTimestamp.minute = currentUtcTime.minute;
						getPosReq.baseTimestamp.second = currentUtcTime.second;    // psdk时间比飞机时间快了几百毫秒  
						
					 do {
						    tempTimeOfsUs = currentUtcTime.microsecond;
								if(tempTimeOfsUs>250000){
								tempTimeOfsUs = currentUtcTime.microsecond-250000;
								}else{
											    if(getPosReq.baseTimestamp.second==0){
														getPosReq.baseTimestamp.minute-=1;
													}
													getPosReq.baseTimestamp.second-=1;
									    		tempTimeOfsUs+=750000;								}
//								PSDK_LOG_DEBUG("******************Transferred Utc Time is %d.%d.%d %d:%d %d.%d",  getPosReq.baseTimestamp.year, getPosReq.baseTimestamp.month,
//															 getPosReq.baseTimestamp.day ,getPosReq.baseTimestamp.hour,getPosReq.baseTimestamp.minute, getPosReq.baseTimestamp.second, tempTimeOfsUs);  //微秒打印出来的只有六位
								if(tempTimeOfsUs > TIME_OFFSET_MAX_VALUE)
										continue;
								
								getPosReq.eventDetailInfo[i].eventId = eventIndex[i]++;    
								getPosReq.eventDetailInfo[i].targetPointId = i;
								getPosReq.eventDetailInfo[i].timeOffsetUs = tempTimeOfsUs; //赋值微妙
//								PSDK_LOG_DEBUG("Req Second is %d, Microsecond is %d.", getPosReq.baseTimestamp.second,
//															getPosReq.eventDetailInfo[i].timeOffsetUs);
								i++;
							} while (i < PAYLOAD_TARGET_POINT_COUNT);
					 
					utc_time.baseTimestamp.year	 = getPosReq.baseTimestamp.year ;
					utc_time.baseTimestamp.month  = 	getPosReq.baseTimestamp.month ;
					utc_time.baseTimestamp.day = 	getPosReq.baseTimestamp.day ;
					utc_time.baseTimestamp.hour = 	getPosReq.baseTimestamp.hour ;
					utc_time.baseTimestamp.minute = 	getPosReq.baseTimestamp.minute ;
					utc_time.baseTimestamp.second = 	getPosReq.baseTimestamp.second ;    
					utc_time.eventDetailInfo[0].timeOffsetUs = tempTimeOfsUs; 
							
							psdkStat = PsdkPositioning_GetPos(psdkUpper, &getPosReq, &getPosAck);
							if (psdkStat != PSDK_STAT_OK || getPosAck.ackCode != PSDK_CMD_ACK_CODE_OK) {   //如果获取失败 则打印
//									PSDK_LOG_WARN("Request Position of Target Points Fail.");
									return PSDK_STAT_ERR;
							}

//							PSDK_LOG_DEBUG("Request Position of Target Points Success.");
//							PSDK_LOG_DEBUG("Detail Position Information:");
							for (i = 0; i < PAYLOAD_TARGET_POINT_COUNT; ++i) {                                
										g_ppsRtkHealthNum=getPosAck.posDetailInfo[i].positionSolutionProperty;

										PSDK_LOG_DEBUG("Longitude: %f\tLatitude: %f\tHeight: %f", getPosAck.posDetailInfo[i].targetPointPos.longitude,
																	 getPosAck.posDetailInfo[i].targetPointPos.latitude,
																	 getPosAck.posDetailInfo[i].targetPointPos.height);
										writesdpos.posDetailInfo[1].targetPointPos.longitude = getPosAck.posDetailInfo[i].targetPointPos.longitude;
										writesdpos.posDetailInfo[1].targetPointPos.latitude = getPosAck.posDetailInfo[i].targetPointPos.latitude;
										writesdpos.posDetailInfo[1].targetPointPos.height = getPosAck.posDetailInfo[i].targetPointPos.height;
										writesdpos.posDetailInfo[1].uavAtti.pitchAttiAngle = getPosAck.posDetailInfo[i].uavAtti.pitchAttiAngle;
                    writesdpos.posDetailInfo[1].uavAtti.rollAttiAngle = getPosAck.posDetailInfo[i].uavAtti.rollAttiAngle;
                    writesdpos.posDetailInfo[1].uavAtti.yawAttiAngle = getPosAck.posDetailInfo[i].uavAtti.yawAttiAngle;

						   }   			
						 return 1;
}
/****************** (C) COPYRIGHT DJI Innovations *****END OF FILE****/
