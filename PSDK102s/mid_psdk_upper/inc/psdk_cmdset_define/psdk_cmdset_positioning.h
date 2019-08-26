/**
 ******************************************************************************
 * @file    psdk_cmdset_positioning.h
 * @version V1.5.0
 * @date    2019/07/31
 * @brief   This file defines positioning command set.
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
#ifndef PSDK_CMDSET_POSITIONING_H
#define PSDK_CMDSET_POSITIONING_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <psdk_typedef.h>
#include <psdk_core.h>

/** @addtogroup PSDK
 * @{
 */

/** @addtogroup PSDK_Positioning
 * @{
 */

/* Exported constants --------------------------------------------------------*/

/** @defgroup P_Positioning_Exported_Constants Exported Constants
 * @{
 */

/**
 * @brief Max count of target points.
 */
#define POSITIONING_TARGET_POINT_MAX_COUNT   5

/**
 * @}
 */

/* Exported macros -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/** @defgroup P_Positioning_Exported_Types Exported Types
 * @{
 */

/** @defgroup P_Positioning_Command_Related Command Related
 * @{
 */

/**
 * @brief Positioning Command Set Command ID
 */
typedef enum {
    PSDK_POSITIONING_CMD_ID_GET_POS = 0x01, /*!< get position command */
} E_PsdkPositioningCmd;

/**
 * @}
 */

/** @defgroup P_Positioning_Related Positioning Related
 * @{
 */

/**
 * @brief RTK position solution property.
 */
typedef enum {
    PSDK_POSITIONING_POSITION_SOLUTION_PROPERTY_NOT_AVAILABLE = 0,              /*!< Position solution is not available. */
    PSDK_POSITIONING_POSITION_SOLUTION_PROPERTY_SINGLE_PNT_SOLUTION = 16,       /*!< Single point position solution. */
    PSDK_POSITIONING_POSITION_SOLUTION_PROPERTY_FLOAT_SOLUTION = 34,            /*!< Float position solution. */
    PSDK_POSITIONING_POSITION_SOLUTION_PROPERTY_NARROW_INT_SOLUTION = 50,       /*!< Narrow fixed point position solution. */
} E_PsdkPositioningPositionSolutionProperty;

/**
 * @}
 */

/** @addtogroup P_Positioning_Command_Related
 * @{
 */

#pragma pack(1)

/**
 * @brief Time.
 */
typedef struct {
    uint16_t year;          /*!< Specifies year. */
    uint8_t month;          /*!< Specifies month. */
    uint8_t day;            /*!< Specifies day. */
    uint8_t hour;           /*!< Specifies hour. */
    uint8_t minute;         /*!< Specifies minute. */
    uint8_t second;         /*!< Specifies second. */
} T_PsdkPositioningTime;

/**
 * @brief Detail information of events what trigger position requesting.
 */
typedef struct {
    uint16_t eventId : 13;      /*!< Specifies event index. */
    uint16_t targetPointId : 3; /*!< Specifies index of target point of payload. */
    uint32_t timeOffsetUs;         /*!< Specifies time offset between base timestamp and time of event, unit: us, range from 0 to 2000000. */
} T_PsdkPositioningEventDetailInfo;

/**
 * @brief Attitude.
 */
typedef struct {
    int16_t pitchAttiAngle; /*!< Specifies attitude angle in pitch axis, range from -180 to 180, unit: degree. */
    int16_t rollAttiAngle;  /*!< Specifies attitude angle in roll axis, range from -180 to 180, unit: degree. */
    int16_t yawAttiAngle;   /*!< Specifies attitude angle in yaw axis, range from -180 to 180, unit: degree. */
} T_PsdkPositioningAtti;

/**
 * @brief Position offset.
 */
typedef struct {
    int16_t northOffset;       /*!< Specifies position offset in N direction of NED coordinate system, unit: mm. */
    int16_t earthOffset;       /*!< Specifies position offset in E direction of NED coordinate system, unit: mm. */
    int16_t downOffset;        /*!< Specifies position offset in D direction of NED coordinate system, unit: mm. */
} T_PsdkPositioningPositionOffset;

/**
 * @brief Position information.
 */
typedef struct {
    psdk_f64_t longitude;       /*!< Specifies longitude, unit: degree. */
    psdk_f64_t latitude;        /*!< Specifies latitude, unit: degree. */
    psdk_f64_t height;          /*!< Specifies height above sea level, unit: m. */
} T_PsdkPositioningPosition;

/**
 * @brief Position value standard deviation.
 */
typedef struct {
    psdk_f32_t longitudeStandardDeviation;    /*!< Specifies longitude standard deviation. */
    psdk_f32_t latitudeStandardDeviation;     /*!< Specifies latitude standard deviation. */
    psdk_f32_t heightStandardDeviation;       /*!< Specifies height standard deviation. */
} T_PsdkPositioningPositionStandardDeviation;

/**
 * @brief Detail position information.
 */
typedef struct {
    T_PsdkPositioningEventDetailInfo eventDetailInfo;                           /*!< Specifies detail information of events what trigger position requesting. */
    uint8_t positionSolutionProperty;                                           /*!< Specifies RTK positioning solution property, and this parameter can be any value of ::E_PsdkPositioningPositionSolutionProperty. */
    T_PsdkPositioningAtti uavAtti;                                              /*!< Specifies UAV attitude. */
    T_PsdkPositioningPositionOffset mainAntennaTargetPointPosOffset;            /*!< Specifies position offset from RTK main antenna to target points in NED coordinate system. */
    T_PsdkPositioningPosition targetPointPos;                                   /*!< Specifies position of target points. */
    T_PsdkPositioningPositionStandardDeviation targetPointPosStandardDeviation; /*!< Specifies position standard deviation of target points. */
} T_PsdkPositioningPositionDetailInfo;

/**
 * @brief Get position command req data structure..
 */
typedef struct {
    uint8_t targetPointCount;                                                               /*!< Specifies target point count. */
    uint8_t taskId;                                                                         /*!< Specifies task index. *///。指定任务指标。*
    T_PsdkPositioningTime baseTimestamp;                                                    /*!< Specifies base timestamp. */
    T_PsdkPositioningEventDetailInfo eventDetailInfo[POSITIONING_TARGET_POINT_MAX_COUNT];   /*!< Specifies detail event information. */
} T_PsdkPositioningGetPosReq;

/**
* @brief Get position command ack data structure..
 */
typedef struct {
    uint8_t ackCode;                                                                        /*!< Specifies PSDK command ack code. This parameter can be any value of ::E_PsdkCmdAckCode. */
    uint8_t targetPointCount;                                                               /*!< Specifies target point count. */
    uint8_t taskId;                                                                         /*!< Specifies task index. */
    T_PsdkPositioningTime baseTimestamp;                                                    /*!< Specifies base timestamp. */
    T_PsdkPositioningPositionDetailInfo posDetailInfo[POSITIONING_TARGET_POINT_MAX_COUNT];  /*!< Specifies detail position information. */
} T_PsdkPositioningGetPosAck;

#pragma pack()

/**
 * @}
 */

/**
 * @}
 */

/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif //PSDK_CMDSET_POSITIONING_H

/****************** (C) COPYRIGHT DJI Innovations *****END OF FILE****/
