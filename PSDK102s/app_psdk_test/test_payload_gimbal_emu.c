/**
 ******************************************************************************
 * @file    test_payload_gimbal_emu.c
 * @version V1.5.0
 * @date    2019/07/31
 * @brief   The file define PSDK gimbal command callback functions for demo board.
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
#include <psdk_cmdset_define/psdk_cmdset_gimbal.h>
#include "test_payload_gimbal_emu.h"
#include "app_utils.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

//gimbal mode
static uint8_t gimbalMode = PSDK_GIMBAL_GIMBAL_MODE_FPV;

//reach yaw, roll, pitch axis limit angle
static uint8_t isYawLimit = 0;
static uint8_t isRollLimit = 0;
static uint8_t isPitchLimit = 0;

//pitch, roll, yaw axis angle (unit: 0.1degree)
static int16_t pitch = 0;
static int16_t roll = 0;
static int16_t yaw = 0;

//pitch, roll, yaw axis angle speed (unit: 0.1degree/s)
static int16_t pitchSpeed = 0;
static int16_t rollSpeed = 0;
static int16_t yawSpeed = 0;

//pitch, roll, yaw axis joint angle (unit: 0.1degree)
static int16_t pitchJoint = 0;
static int16_t rollJoint = 0;
static int16_t yawJoint = 0;

//pitch, roll, yaw axis angle limit (unit: 0.1degree)
static const int16_t pitchMaxLimit = 300;
static const int16_t pitchMinLimit = -900;
static const int16_t rollMaxLimit = 450;
static const int16_t rollMinLimit = -450;
static const int16_t yawMaxLimit = 900;
static const int16_t yawMinLimit = -900;

/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
 * @brief Get gimbal parameters callback function.
 * @param req Command request structure, please refer to ::T_PsdkGimbalGetGimbalStateReq for details.
 * @param ack Command ack structure, please refer to ::T_PsdkGimbalGetGimbalStateAck for details.
 * @return None.
 */
void GimbalEmu_GetGimbalParam(const T_PsdkGimbalGetGimbalStateReq *req, T_PsdkGimbalGetGimbalStateAck *ack)
{
//    PSDK_LOG_DEBUG("Get Gimbal Param");

    //get gimbal param
    ack->gimbalMode = gimbalMode;
    ack->isMountedUpwards = 0;

    pitch += (int16_t) (pitchSpeed * 0.1);
    roll += (int16_t) (rollSpeed * 0.1);
    yaw += (int16_t) (yawSpeed * 0.1);

    pitchSpeed = 0;
    rollSpeed = 0;
    yawSpeed = 0;

    pitch = pitch > pitchMaxLimit ? pitchMaxLimit : pitch;
    pitch = pitch < pitchMinLimit ? pitchMinLimit : pitch;
    roll = roll > rollMaxLimit ? rollMaxLimit : roll;
    roll = roll < rollMinLimit ? rollMinLimit : roll;
    yaw = yaw > yawMaxLimit ? yawMaxLimit : yaw;
    yaw = yaw < yawMinLimit ? yawMinLimit : yaw;

    if (pitch == pitchMaxLimit || pitch == pitchMinLimit) {
        isPitchLimit = 1;
    }

    if (roll == rollMaxLimit || roll == rollMinLimit) {
        isRollLimit = 1;
    }

    if (yaw == yawMaxLimit || yaw == yawMinLimit) {
        isYawLimit = 1;
    }

    ack->isPitchLimit = isPitchLimit;
    ack->isRollLimit = isRollLimit;
    ack->isYawLimit = isYawLimit;

    ack->pitch = pitch;
    ack->roll = roll;
    ack->yaw = yaw;

    //ack code
    ack->ackCode = PSDK_CMD_ACK_CODE_OK;
}

/**
 * @brief Set gimbal mode callback function.
 * @param req Command request structure, please refer to ::T_PsdkGimbalSetGimbalModeReq for details.
 * @param ack Command ack structure, please refer to ::T_PsdkGimbalSetGimbalModeAck for details.
 * @return None.
 */
void GimbalEmu_SetGimbalMode(const T_PsdkGimbalSetGimbalModeReq *req, T_PsdkGimbalSetGimbalModeAck *ack)
{
    PSDK_LOG_DEBUG("Set Gimbal Mode");

    gimbalMode = req->gimbalMode;

    ack->ackCode = PSDK_CMD_ACK_CODE_OK;
}

/**
 * @brief Control gimbal return head callback function.
 * @param req Command request structure, please refer to ::T_PsdkGimbalReturnHeadReq for details.
 * @param ack Command ack structure, please refer to ::T_PsdkGimbalReturnHeadAck for details.
 * @return None.
 */
void GimbalEmu_ReturnHead(const T_PsdkGimbalReturnHeadReq *req, T_PsdkGimbalReturnHeadAck *ack)
{
    PSDK_LOG_DEBUG("Return Head");

    pitch = 0;
    roll = 0;
    yaw = 0;

    ack->ackCode = PSDK_CMD_ACK_CODE_OK;
}

/**
 * @brief Control gimbal speed callback function.
 * @param req Command request structure, please refer to ::T_PsdkGimbalControlSpeedReq for details.
 * @param ack Command ack structure, please refer to ::T_PsdkGimbalControlSpeedAck for details.
 * @return None.
 */
void GimbalEmu_ControlSpeed(const T_PsdkGimbalControlSpeedReq *req, T_PsdkGimbalControlSpeedAck *ack)
{
    PSDK_LOG_DEBUG("Control Speed");

    pitchSpeed = req->pitchSpeed;
    rollSpeed = req->rollSpeed;
    yawSpeed = req->yawSpeed;

    ack->ackCode = PSDK_CMD_ACK_CODE_OK;
}

/**
 * @brief Control gimbal angle callback function.
 * @param req Command request structure, please refer to ::T_PsdkGimbalControlAngleReq for details.
 * @param ack Command ack structure, please refer to ::T_PsdkGimbalControlAngleAck for details.
 * @return None.
 */
void GimbalEmu_ControlAngle(const T_PsdkGimbalControlAngleReq *req, T_PsdkGimbalControlAngleAck *ack)
{
    PSDK_LOG_DEBUG("Control Gimbal Angle");

    pitch = (int16_t) (req->pitchAngle * 10);
    roll = (int16_t) (req->rollAngle * 10);
    yaw = (int16_t) (req->yawAngle * 10);

    ack->ackCode = PSDK_CMD_ACK_CODE_OK;
}

/**
 * @brief Control gimbal joint angle callback function.
 * @param req Command request structure, please refer to ::T_PsdkGimbalControlJointReq for details.
 * @param ack Command ack structure, please refer to ::T_PsdkGimbalControlJointAck for details.
 * @return None.
 */
void GimbalEmu_ControlJointAngle(const T_PsdkGimbalControlJointAngleReq *req, T_PsdkGimbalControlJointAngleAck *ack)
{
    PSDK_LOG_DEBUG("Control Gimbal Joint Angle");

    pitchJoint = req->pitchJointAngle;
    rollJoint = req->rollJointAngle;
    yawJoint = req->yawJointAngle;

    UNUSED(pitchJoint);
    UNUSED(rollJoint);
    UNUSED(yawJoint);

    ack->ackCode = PSDK_CMD_ACK_CODE_OK;
}

/**
 * @brief Start gimbal calibration callback function.
 * @param req Command request structure, please refer to ::T_PsdkGimbalStartCalibrationReq for details.
 * @param ack Command ack structure, please refer to ::T_PsdkGimbalStartCalibrationAck for details.
 * @return None.
 */
void GimbalEmu_StartCalibration(const T_PsdkGimbalStartCalibrationReq *req, T_PsdkGimbalStartCalibrationAck *ack)
{
    PSDK_LOG_DEBUG("Gimbal Start Calibration");

    ack->ackCode = PSDK_CMD_ACK_CODE_OK;
}

/**
 * @brief Get gimbal calibration progress callback function.
 * @param req Command request structure, please refer to ::T_PsdkGimbalGetCalibrationProgressReq for details.
 * @param ack Command ack structure, please refer to ::T_PsdkGimbalGetCalibrationProgressAck for details.
 * @return None.
 */
void GimbalEmu_GetCalibrationProgress(const T_PsdkGimbalGetCaliProgressReq *req, T_PsdkGimbalGetCaliProgressAck *ack)
{
    PSDK_LOG_DEBUG("Gimbal Get Calibration Progress");

    ack->ackCode = PSDK_CMD_ACK_CODE_OK;
    ack->progress = 100;
    ack->status = PSDK_GIMBAL_CALI_CMPL;
}

//gimbal command callback function list
const T_PsdkGimbalHandler g_testGimbalHandler = {
        .GetGimbalState = GimbalEmu_GetGimbalParam,
        .SetGimbalMode = GimbalEmu_SetGimbalMode,
        .ReturnHead = GimbalEmu_ReturnHead,
        .ControlSpeed = GimbalEmu_ControlSpeed,
        .ControlAngle = GimbalEmu_ControlAngle,
        .ControlJointAngle = GimbalEmu_ControlJointAngle,
        .StartCalibration = GimbalEmu_StartCalibration,
        .GetCaliProgress = GimbalEmu_GetCalibrationProgress,
};

/****************** (C) COPYRIGHT DJI Innovations *****END OF FILE****/
