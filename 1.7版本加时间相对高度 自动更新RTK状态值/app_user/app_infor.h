/**
 ******************************************************************************
 * @file    app_infor.c
 * @version V1.3.0
 * @date    2018/09/19
 * @brief   The file specifies payload application information.
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
#ifndef __APP_INFO_H__
#define __APP_INFO_H__

/* Includes ------------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

// ATTENTION: User must goto developer.dji.com to create your own payload sdk application, get payload sdk application
// information then fill in the application information here.
//18554016766@126.com
//shareuav122333
//cocoguojia gai
//用户开发帐户
#define USER_APP_NAME               "SHARE-101S"
#define USER_APP_ID                 "65187"
#define USER_APP_KEY                "f37923a5b2f94477defd81783ef1005"
#define USER_DEVELOPER_ACCOUNT      "18554016766@126.com"

// ATTENTION: If need to upgrade SKYPORT adapter's firmware to a fixed version, please specify the version number here and
// SKYPORT_FW_FIX_VER_NEED macro below.
// When user use DJI Assistant to upgrade SKYPORT adapter firmware, DJI Assistant will suggest you upgrade to the fixed version
// if the fixed version here exists in server. Otherwise, DJI Assistant will suggest you upgrade to the latest version.

// Macro used to define whether upgrade SKYPORT adapter's firmware to fixed version is needed.
// 1 represents need, 0 represents not need.
#define SKYPORT_FW_FIX_VER_NEED 1

// For example, you can write like this representing upgrading SKYPORT adapter's firmware to version 01.00.08.91:
#if SKYPORT_FW_FIX_VER_NEED
// #define SKYPORT_FW_FIX_VER_MAJOR  1
// #define SKYPORT_FW_FIX_VER_MINOR  3
// #define SKYPORT_FW_FIX_VER_MODIFY 00
// #define SKYPORT_FW_FIX_VER_DEBUG  03
 
 #define SKYPORT_FW_FIX_VER_MAJOR  1
 #define SKYPORT_FW_FIX_VER_MINOR  2
 #define SKYPORT_FW_FIX_VER_MODIFY 03
 #define SKYPORT_FW_FIX_VER_DEBUG  01
#endif

// ATTENTION: If need to set alias for load application, please specify it here and PSDK_APP_ALIAS_NEED macro below.
// Note: Application alias length must be less than 32.

// Macro used to define whether PSDK application alias is needed.
// 1 represents need, 0 represents not need.
#define PSDK_APP_ALIAS_NEED     0

// For example, define an application alias as "PSDK_ALIAS_APPNAME":
#if PSDK_APP_ALIAS_NEED
 #define PSDK_APP_ALIAS  "PSDK_ALIAS_APPNAME"
#endif

/* Exported macros -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#endif
