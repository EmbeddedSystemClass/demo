/**
 ******************************************************************************
 * @file    psdk_osdk_func.h
 * @version V1.4.0
 * @date    2018/11/20
 * @brief
 * This is the header file for "psdk_osdk_func.c", defining the structure and
 * (exported) function prototypes.
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
#ifndef PSDK_OSDK_FUNC_H
#define PSDK_OSDK_FUNC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "psdk_upper.h"
#include "psdk_cmdset.h"

/** @addtogroup PSDK
 * @{
 */

/** @addtogroup PSDK_Osdk_Function
 * @{
 */

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

/** @defgroup O_Func_Exported_Types Exported Types
 * @{
 */

/** @defgroup O_Func_Callback_Related Callback Related
 * @{
 */

/**
 * @brief OSDK related function list and configure parameters.
 * @details This structure type is used to define a list structure that includes all
 * the callback functions and parameters that you can design for OSDK data transmission:
 * 1. Transparent data transmission from OSDK to payload end
 */
typedef struct {
    void (*ReceiveFromOsdkCallback) (const uint8_t *pData, uint16_t dataLen);   /*!< Receive data from OSDK callback function. */
} T_PsdkOsdkFuncHandler;

/**
 * @}
 */

/**
 * @}
 */

/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/** @addtogroup A_Func_Exported_Functions
 * @{
 */

E_PsdkStat PsdkOsdkFunc_Init(T_PsdkUpper *psdkUpper, const T_PsdkOsdkFuncHandler *osdkFuncHandler);
E_PsdkStat PsdkOsdkFunc_TransferToOsdk(T_PsdkUpper *psdkUpper, const uint8_t *pSendData, uint16_t needSendLen, uint16_t *pRealSendLen);

/**
 * @}
 */

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

#endif //PSDK_OSDK_FUNC_H

/****************** (C) COPYRIGHT DJI Innovations *****END OF FILE****/
