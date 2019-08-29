/**
 ******************************************************************************
 * @file    test_app_func.h
 * @version V1.5.0
 * @date    2019/07/31
 * @brief   This is the header file for "test_app_func.c".
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

#ifndef TEST_APP_FUNC_H
#define TEST_APP_FUNC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <psdk.h>

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern const T_PsdkAppFuncWidgetListItem *g_TestWidgetList;
extern const uint8_t g_TestWidgetCount;

/* Exported functions --------------------------------------------------------*/
void Test_ReceiveFromMobileSDKFunc(const uint8_t *pData, uint16_t dataLen);
E_PsdkStat Test_GetWidgetValueFunc(E_PsdkAppFuncWidgetType widgetType, uint8_t widgetIndex,
                                   U_AppFuncWidgetValue *pWidgetValue);
E_PsdkStat Test_SetWidgetValueFunc(E_PsdkAppFuncWidgetType widgetType, uint8_t widgetIndex,
                                   const U_AppFuncWidgetValue *pWidgetValue);
E_PsdkStat Test_GetTextInputBoxParamFunc(E_PsdkAppFuncTextInputBoxDisplayControlFlag *displayControlFlag,
                                         uint8_t *boxName, uint8_t *boxDescription);

/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif //TEST_APP_FUNC_H

/****************** (C) COPYRIGHT DJI Innovations *****END OF FILE****/
