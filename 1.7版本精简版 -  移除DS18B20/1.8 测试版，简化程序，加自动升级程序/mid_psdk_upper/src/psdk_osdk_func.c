/**
 ******************************************************************************
 * @file    psdk_osdk_func.c
 * @version V1.5.0
 * @date    2019/07/31
 * @brief   This file provides the API functions for handling the transparent data
 * transmission between OSDK and user's payload.
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
#include "psdk_osdk_func.h"
#include <string.h>

/** @addtogroup PSDK
 * @{
 */

/** @defgroup PSDK_Osdk_Function PSDK Osdk Function
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/** @defgroup O_Func_Private_Variables Private Variables
 * @{
 */

///callback function list
static T_PsdkOsdkFuncHandler s_osdkFuncHandler = {0};

/**
 * @}
 */

/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup A_Func_Private_Functions Private functions
 * @{
 */

/**
 * @brief Handle transparent data transmission from Osdk to payload end.
 * @param protHandle Pointer to PSDK protocol handle.
 * @param inProtInfo Pointer to PSDK protocol information of data package.
 * @param inProtData Pointer to data field of package.
 * @return PSDK function process state.
 */
static E_PsdkStat
OsdkFuncCmd_TT_OsdkToLoad(struct _PsdkProt *protHandle, const T_PsdkProtInfo *inProtInfo, const uint8_t *inProtData)
{
    PSDK_UNUSED(protHandle);

    if (s_osdkFuncHandler.ReceiveFromOsdkCallback != NULL) {
        s_osdkFuncHandler.ReceiveFromOsdkCallback(inProtData, inProtInfo->dataLen);
    }

    return PSDK_STAT_OK;
}

/**
 * @}
 */

/** @addtogroup O_Func_Private_Variables
 * @{
 */

/**
 * @brief Osdk function commands process function list.
 */
static const T_PsdkProtCmdItem s_osdkFuncProtCmdList[] =
        {
                PSDK_PROT_CMD_ITEM(PSDK_CMDSET_OSDK_FUNC, PSDK_OSDKFUNC_CMDID_TT_OSDK_TO_PSDK,
                                   OsdkFuncCmd_TT_OsdkToLoad),
        };

/**
 * @}
 */

/* Exported functions --------------------------------------------------------*/

/** @defgroup A_Func_Exported_Functions Exported Functions
 * @{
 */

/**
 * @brief Initialize the OSDK integration functionality.
 * @details Mount OSDK function command process functions.
 * @param psdkUpper Pointer to the top level PSDK structure.
 * @param osdkFuncHandler Pointer to OSDK related functions handling callback functions.
 * @return PSDK function process state.
 */
E_PsdkStat PsdkOsdkFunc_Init(T_PsdkUpper *psdkUpper, const T_PsdkOsdkFuncHandler *osdkFuncHandler)
{
    T_PsdkProtCmdSetHandle cmdSetHandle;

    s_osdkFuncHandler = *osdkFuncHandler;

    cmdSetHandle.cmdSet = PSDK_CMDSET_OSDK_FUNC;
    cmdSetHandle.cmdList = s_osdkFuncProtCmdList;
    cmdSetHandle.cmdCount = sizeof(s_osdkFuncProtCmdList) / sizeof(T_PsdkProtCmdItem);

    return PsdkProto_RegCmdSetHandler(&(psdkUpper->protHandle), &cmdSetHandle);
}

/**
 * @brief Transparently transmit data to OSDK.
 * @param psdkUpper pointer to the top level PSDK structure.
 * @param pSendData pointer to the data buffer to be sent.
 * @param needSendLen the length of the data to be sent.
 * @param pRealSendLen the length of the data that has been actually sent.
 * @return PSDK function process state.
 */
E_PsdkStat PsdkOsdkFunc_TransferToOsdk(T_PsdkUpper *psdkUpper, const uint8_t *pSendData, uint16_t needSendLen,
                                       uint16_t *pRealSendLen)
{
    T_PsdkProtInfo protInfo;
    uint16_t packCount;
    uint16_t lastPackLen;
    const uint8_t *pSendPtr;
    int i;

    lastPackLen = (uint16_t) (needSendLen % OSDKFUNC_PSDK_TO_OSDK_MAX_LEN);
    packCount = (uint16_t) (needSendLen / OSDKFUNC_PSDK_TO_OSDK_MAX_LEN);

    protInfo.packetType = PSDK_PROT_PACKET_TYPE_CMD;
    protInfo.ackReq = PSDK_PROT_ACK_REQ_NONEED;
    protInfo.encType = PSDK_PROT_ENC_AES;
    protInfo.cmdSet = PSDK_CMDSET_OSDK_FUNC;
    protInfo.cmdID = PSDK_OSDKFUNC_CMDID_TT_PSDK_TO_OSDK;

    pSendPtr = pSendData;

    //send pack
    protInfo.dataLen = OSDKFUNC_PSDK_TO_OSDK_MAX_LEN;
    for (i = 0; i < packCount; i++) {
        protInfo.seqNum = PsdkProt_GetCurrentSendSeq(&(psdkUpper->protHandle));
        if (PsdkProto_Send(&(psdkUpper->protHandle), &protInfo, pSendPtr) != PSDK_STAT_OK) {
            *pRealSendLen = (uint16_t) (pSendPtr - pSendData);
            return PSDK_STAT_ERR;
        }
        pSendPtr += OSDKFUNC_PSDK_TO_OSDK_MAX_LEN;
    }

    //send last pack
    if (lastPackLen != 0) {
        protInfo.dataLen = lastPackLen;
        protInfo.seqNum = PsdkProt_GetCurrentSendSeq(&(psdkUpper->protHandle));
        if (PsdkProto_Send(&(psdkUpper->protHandle), &protInfo, pSendPtr) != PSDK_STAT_OK) {
            *pRealSendLen = (uint16_t) (pSendPtr - pSendData);
            return PSDK_STAT_ERR;
        }
        pSendPtr += lastPackLen;
    }


    *pRealSendLen = (uint16_t) (pSendPtr - pSendData);
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
