/**
 ******************************************************************************
 * @file    psdk_positioning.h
 * @version V1.5.0
 * @date    2019/07/31
 * @brief   This is the header file for "psdk_positioning.c", defining the
 * structures and (exported) function prototypes.
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
#ifndef PSDK_POSITIONING_H
#define PSDK_POSITIONING_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "psdk_upper.h"
#include "psdk_cmdset.h"

/** @addtogroup PSDK
 * @{
 */

/** @addtogroup PSDK_Positioning
 * @{
 */

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/** @addtogroup P_Positioning_Exported_Functions
 * @{
 */

E_PsdkStat PsdkPositioning_GetPos(T_PsdkUpper *psdkUpper, T_PsdkPositioningGetPosReq *getPosReq,
                                  T_PsdkPositioningGetPosAck *getPosAck);

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

#endif //PSDK_POSITIONING_H

/****************** (C) COPYRIGHT DJI Innovations *****END OF FILE****/
