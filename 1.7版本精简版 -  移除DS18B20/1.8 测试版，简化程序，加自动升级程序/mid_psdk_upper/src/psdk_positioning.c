/**
 ******************************************************************************
 * @file    psdk_positioning.c
 * @version V1.5.0
 * @date    2019/07/31
 * @brief   This file provides the API functions for handling those commands in positioning command set.
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
#include <psdk_positioning.h>

/** @addtogroup PSDK
 * @{
 */

/** @defgroup PSDK_Positioning PSDK Positioning
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/** @defgroup P_Positioning_Exported_Functions Exported Functions
 * @{
 */

/**
 * @brief The function is used to request position of target points.
 * @param psdkUpper Pointer to the top-level PSDK structure.
 * @param getPosReq Pointer to get position command request structure.
 * @param getPosAck Pointer to get position command ack structure.
 * @return PSDK function process state.
 */
E_PsdkStat PsdkPositioning_GetPos(T_PsdkUpper *psdkUpper, T_PsdkPositioningGetPosReq *getPosReq,
                                  T_PsdkPositioningGetPosAck *getPosAck)
{
    T_PsdkProtInfo reqProtInfo, ackProtInfo;

    if (getPosReq->targetPointCount > POSITIONING_TARGET_POINT_MAX_COUNT || getPosReq->targetPointCount < 1) {
        return PSDK_STAT_ERR;
    }

    reqProtInfo.packetType = PSDK_PROT_PACKET_TYPE_CMD;
    reqProtInfo.ackReq = PSDK_PROT_ACK_REQ_AFTER_EXE;
    reqProtInfo.encType = PSDK_PROT_ENC_AES;
    reqProtInfo.cmdSet = PSDK_CMDSET_POSITIONING;
    reqProtInfo.cmdID = PSDK_POSITIONING_CMD_ID_GET_POS;
    reqProtInfo.dataLen = sizeof(T_PsdkPositioningGetPosReq)
                          - (POSITIONING_TARGET_POINT_MAX_COUNT - getPosReq->targetPointCount) *
                            sizeof(T_PsdkPositioningEventDetailInfo);
    reqProtInfo.seqNum = PsdkProt_GetCurrentSendSeq(&(psdkUpper->protHandle));

    if (PsdkProto_SendGetAck(&(psdkUpper->protHandle), &reqProtInfo, (const uint8_t *) getPosReq, &ackProtInfo,
                             (uint8_t *) getPosAck, PSDK_PROT_DEFAULT_CMD_ACK_TIMEOUT) != PSDK_STAT_OK) {
        return PSDK_STAT_ERR;
    }

    return PSDK_STAT_OK;
}

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/****************** (C) COPYRIGHT DJI Innovations *****END OF FILE****/
