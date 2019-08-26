/**
 ******************************************************************************
 * @file    psdk_cmdset_osdk_func.h
 * @version V1.4.0
 * @date    2018/11/20
 * @brief   This file defines the OSDK function command set.
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
#ifndef PSDK_CMDSET_OSDK_FUNC_H
#define PSDK_CMDSET_OSDK_FUNC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <psdk_typedef.h>
#include <psdk_core.h>

/** @addtogroup PSDK
 * @{
 */

/** @addtogroup PSDK_Osdk_Function
 * @{
 */

/* Exported constants --------------------------------------------------------*/

/** @defgroup O_Func_Exported_Constants Exported constants
 * @{
 */

#define OSDKFUNC_OSDK_TO_PSDK_MAX_LEN   255 /*!< Max size of data from OSDK to payload end */
#define OSDKFUNC_PSDK_TO_OSDK_MAX_LEN   255 /*!< Max size of data from payload end to OSDK */

/**
 * @}
 */

/* Exported macros -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

/** @defgroup O_Func_Exported_Types Exported Types
 * @{
 */

/** @defgroup O_Func_Command_Related Command Related
 * @{
 */

/**
 * @brief OSDK Function Command ID
 */
typedef enum {
    PSDK_OSDKFUNC_CMDID_TT_OSDK_TO_PSDK = 0x01, /*!< Transparent data transmission from OSDK to payload end */
    PSDK_OSDKFUNC_CMDID_TT_PSDK_TO_OSDK = 0x02, /*!< Transparent data transmission from payload end to OSDK */
} E_PsdkOsdkFuncCmd;

#pragma pack(1)

/**
 * @brief Transparent data transmission from OSDK to payload end command request data structure.
 */
typedef struct {
    uint8_t data[OSDKFUNC_OSDK_TO_PSDK_MAX_LEN];  /*!< Specifies transmission data. */
} T_PsdkOsdkFuncTransOsdkToPsdkReq;

/**
 * @brief Transparent data transmission from OSDK to payload end command ack data structure.
 */
typedef struct {
    PSDK_EMPTY_STRUCT
} T_PsdkOsdkFuncTransOsdkToPsdkAck;

/**
 * @brief Transparent data transmission from payload end to OSDK command request data structure.
 */
typedef struct {
    uint8_t data[OSDKFUNC_PSDK_TO_OSDK_MAX_LEN];  /*!< Specifies transmission data. */
} T_PsdkOsdkFuncTransPsdkToOsdkReq;

/**
 * @brief Transparent data transmission from payload end to OSDK command ack data structure.
 */
typedef struct {
    PSDK_EMPTY_STRUCT
} T_PsdkOsdkFuncTransPsdkToOsdkAck;

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

#endif //PSDK_CMDSET_OSDK_FUNC_H

/****************** (C) COPYRIGHT DJI Innovations *****END OF FILE****/
