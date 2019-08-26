/**
 ******************************************************************************
 * @file    psdk_ack_code.h
 * @version V0.0.0
 * @date    2017/11/9
 * @brief   PSDK protocol ack code define.
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
#ifndef PSDK_ACK_CODE_H
#define PSDK_ACK_CODE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/** @addtogroup PSDK
* @{
*/

/** @addtogroup PSDK_Core
* @{
*/

/** @defgroup PSDK_Ack_Code PSDK Ack Code
 * @{
 */

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

/** @defgroup A_Code_Exported_Types Exported Types
 * @{
 */

/**
 * @brief PSDK protocol ack code.
 */
typedef enum {
    PSDK_CMD_ACK_CODE_OK = 0x00,
    PSDK_CMD_ACK_CODE_ERROR = 0x01,
    PSDK_CMD_ACK_CODE_UNSUPPORT = 0x02,
} E_PsdkCmdAckCode;

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

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif //PSDK_ACK_CODE_H

/****************** (C) COPYRIGHT DJI Innovations *****END OF FILE****/
