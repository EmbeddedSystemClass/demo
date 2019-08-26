/**
 ******************************************************************************
 * @file    test_app_func.c
 * @version V1.5.0
 * @date    2019/07/31
 * @brief   The file define PSDK APP function command callback functions for demo board.
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
#include "test_app_func.h"
#include "led_button_switch.h"
#include "app_utils.h"
#include <string.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

//unique index define
#define SWITCH1_INDEX   0
#define SWITCH2_INDEX   1
#define BUTTON1_INDEX   2
#define SCALE1_INDEX    3
#define INPUT1_INDEX    4
#define LIST1_INDEX     5
#define BUTTON2_INDEX   6
#define BUTTON3_INDEX   7
#define LIST3_INDEX     8
#define LIST4_INDEX     9
#define LIST2_INDEX     10
#define INPUT2_INDEX    11

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

//@formatter:off
//widget list, please keep there not exist several same widget index and keep that index of widget in list increase from 0 in order.
static const T_PsdkAppFuncWidgetListItem s_TestWidgetList[] = {
APPFUNC_DEF_SWITCH_WIDGET(SWITCH1_INDEX, "Red LED"),
APPFUNC_DEF_SWITCH_WIDGET(SWITCH2_INDEX, "Blue LED"),

APPFUNC_DEF_BUTTON_WIDGET(BUTTON1_INDEX, "Button_1"),
APPFUNC_DEF_SCALE_WIDGET(SCALE1_INDEX, "Scale_1"),
APPFUNC_DEF_INT_INPUT_BOX_WIDGET(INPUT1_INDEX, "InputBox_1", "Unit : m"),
APPFUNC_DEF_LIST_WIDGET(LIST1_INDEX, "List_1", 3, "CH1", "CH2", "CH3"),

APPFUNC_DEF_BUTTON_WIDGET(BUTTON2_INDEX, "Button_2"),

APPFUNC_DEF_BUTTON_WIDGET(BUTTON3_INDEX, "Button_3"),
APPFUNC_DEF_LIST_WIDGET(LIST3_INDEX, "List_3", 10, "CH1", "CH2", "CH3", "CH4", "CH5", "CH6", "CH7", "CH8", "CH9", "CH10"),
APPFUNC_DEF_LIST_WIDGET(LIST4_INDEX, "List_4", 2, "TEST1", "TEST2"),
APPFUNC_DEF_LIST_WIDGET(LIST2_INDEX, "List_5", 1, "TEST_CHOOSE1"),
APPFUNC_DEF_INT_INPUT_BOX_WIDGET(INPUT2_INDEX, "InputBox_2", "Unit : ms"),
};
//@formatter:on

//buffer used to store data received from mobile APP
static char receivePrint[1024];

//widget value
static U_AppFuncWidgetValue switch1Val = {.switchVal = PSDK_APPFUNC_SWITCH_VAL_OFF};
static U_AppFuncWidgetValue scale1Val = {.scaleVal = 50};
static U_AppFuncWidgetValue input1Val = {.intInputBoxVal = 200};
static U_AppFuncWidgetValue list1Val = {.listVal = 1};
static U_AppFuncWidgetValue switch2Val = {.switchVal = PSDK_APPFUNC_SWITCH_VAL_OFF};
static U_AppFuncWidgetValue list3Val = {.listVal = 9};
static U_AppFuncWidgetValue list4Val = {.listVal = 0};
static U_AppFuncWidgetValue list2Val = {.listVal = 0};
static U_AppFuncWidgetValue input2Val = {.intInputBoxVal = 2000};

//text input box
static E_PsdkAppFuncTextInputBoxDisplayControlFlag textInputBoxDisplayControlFlag = PSDK_APPFUNC_TEXT_INPUT_BOX_NOT_DISPLAY;
static uint8_t textInputBoxName[APPFUNC_TEXT_INPUT_BOX_NAME_MAX_SIZE] = "Text Input Box";
static uint8_t textInputBoxDescription[APPFUNC_TEXT_INPUT_BOX_DESCRIPTION_MAX_SIZE] = "Text Input Box Description";

/* Exported variables --------------------------------------------------------*/

//Pointer to widget list
const T_PsdkAppFuncWidgetListItem *g_TestWidgetList = s_TestWidgetList;
//total widget count
const uint8_t g_TestWidgetCount = sizeof(s_TestWidgetList) / sizeof(T_PsdkAppFuncWidgetListItem);

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
 * @brief Receive data from mobile APP callback function.
 * @param pData Pointer to data received.
 * @param dataLen Length of data received.
 * @return None.
 */
void Test_ReceiveFromMobileSDKFunc(const uint8_t *pData, uint16_t dataLen)
{
    memcpy(receivePrint, pData, dataLen);
    receivePrint[dataLen] = 0;
    PSDK_LOG_DEBUG("Receive from App: %s", receivePrint);
}

/**
 * @brief Get widget value callback function.
 * @param widgetType Type of widget to be get.
 * @param widgetIndex Index of widget to be get.
 * @param pWidgetValue Pointer to widget value.
 * @return None.
 */
E_PsdkStat
Test_GetWidgetValueFunc(E_PsdkAppFuncWidgetType widgetType, uint8_t widgetIndex, U_AppFuncWidgetValue *pWidgetValue)
{
    UNUSED(widgetType);

    switch (widgetIndex) {
        case SWITCH1_INDEX:
            *pWidgetValue = switch1Val;
            break;
        case SWITCH2_INDEX:
            *pWidgetValue = switch2Val;
            break;

        case SCALE1_INDEX:
            *pWidgetValue = scale1Val;
            break;
        case INPUT1_INDEX:
            *pWidgetValue = input1Val;
            break;
        case INPUT2_INDEX:
            *pWidgetValue = input2Val;
            break;

        case LIST1_INDEX:
            *pWidgetValue = list1Val;
            break;
        case LIST2_INDEX:
            *pWidgetValue = list2Val;
            break;
        case LIST3_INDEX:
            *pWidgetValue = list3Val;
            break;
        case LIST4_INDEX:
            *pWidgetValue = list4Val;
            break;
        default:
            break;
    }

    return PSDK_STAT_OK;
}

/**
 * @brief Set widget value callback function.
 * @param widgetType Type of widget to be set.
 * @param widgetIndex Index of widget to be set.
 * @param pWidgetValue Pointer to widget value to be set.
 * @return None.
 */
E_PsdkStat Test_SetWidgetValueFunc(E_PsdkAppFuncWidgetType widgetType, uint8_t widgetIndex,
                                   const U_AppFuncWidgetValue *pWidgetValue)
{
    uint8_t i = 0;

    UNUSED(widgetType);

    for (i = 0; i < g_TestWidgetCount; i++) {
        if (g_TestWidgetList[i].widgetIndex == widgetIndex) {
            break;
        }
    }

    switch (widgetIndex) {
        case BUTTON1_INDEX:
            if (pWidgetValue->buttonVal == PSDK_APPFUNC_BUTTON_VAL_PRESS_DOWN) {
                PSDK_LOG_DEBUG("Button1 Press Down");
            } else if (pWidgetValue->buttonVal == PSDK_APPFUNC_BUTTON_VAL_RELEASE_UP) {
                PSDK_LOG_DEBUG("Button1 Release Up");
            } else {
                goto err;
            }
            break;
        case BUTTON2_INDEX:
            if (pWidgetValue->buttonVal == PSDK_APPFUNC_BUTTON_VAL_PRESS_DOWN) {
                PSDK_LOG_DEBUG("Button2 Press Down");
            } else if (pWidgetValue->buttonVal == PSDK_APPFUNC_BUTTON_VAL_RELEASE_UP) {
                PSDK_LOG_DEBUG("Button2 Release Up");
            } else {
                goto err;
            }
            break;
        case BUTTON3_INDEX:
            if (pWidgetValue->buttonVal == PSDK_APPFUNC_BUTTON_VAL_PRESS_DOWN) {
                PSDK_LOG_DEBUG("Button3 Press Down");
            } else if (pWidgetValue->buttonVal == PSDK_APPFUNC_BUTTON_VAL_RELEASE_UP) {
                PSDK_LOG_DEBUG("Button3 Release Up");
            } else {
                goto err;
            }
            break;
        case SWITCH1_INDEX:
            PSDK_LOG_DEBUG("Switch1 Opt");
            if (pWidgetValue->switchVal == PSDK_APPFUNC_SWITCH_VAL_ON) {
               // LED_Control(LED2_RED, LED_OPT_ON);
            } else if (pWidgetValue->switchVal == PSDK_APPFUNC_SWITCH_VAL_OFF) {
                //LED_Control(LED2_RED, LED_OPT_OFF);
            } else {
                goto err;
            }
            switch1Val = *pWidgetValue;
            break;
        case SWITCH2_INDEX:
            PSDK_LOG_DEBUG("Switch2 Opt");
            if (pWidgetValue->switchVal == PSDK_APPFUNC_SWITCH_VAL_ON) {
              //  LED_Control(LED3_BLUE, LED_OPT_ON);
            } else if (pWidgetValue->switchVal == PSDK_APPFUNC_SWITCH_VAL_OFF) {
               // LED_Control(LED3_BLUE, LED_OPT_OFF);
            } else {
                goto err;
            }
            switch2Val = *pWidgetValue;
            break;

        case SCALE1_INDEX:
            PSDK_LOG_DEBUG("Scale1 Opt");
            if ((pWidgetValue->scaleVal < APPFUNC_WIDGET_SCALE_MIN_VALUE) || \
               (pWidgetValue->scaleVal > APPFUNC_WIDGET_SCALE_MAX_VALUE)) {
                goto err;
            }
            scale1Val = *pWidgetValue;
            break;
        case INPUT1_INDEX:
            PSDK_LOG_DEBUG("Input1 Opt");
            input1Val = *pWidgetValue;
            break;
        case INPUT2_INDEX:
            PSDK_LOG_DEBUG("Input2 Opt");
            input2Val = *pWidgetValue;
            break;

        case LIST1_INDEX:
            PSDK_LOG_DEBUG("List1 Opt");
            if ((pWidgetValue->listVal >= g_TestWidgetList[i].widgetAttribute.listAttribute.listItemNum) || \
               (pWidgetValue->listVal < 0)) {
                goto err;
            }
            list1Val = *pWidgetValue;
            break;
        case LIST2_INDEX:
            PSDK_LOG_DEBUG("List2 Opt");
            if ((pWidgetValue->listVal >= g_TestWidgetList[i].widgetAttribute.listAttribute.listItemNum) || \
               (pWidgetValue->listVal < 0)) {
                goto err;
            }
            list2Val = *pWidgetValue;
            break;
        case LIST3_INDEX:
            PSDK_LOG_DEBUG("List3 Opt");
            if ((pWidgetValue->listVal >= g_TestWidgetList[i].widgetAttribute.listAttribute.listItemNum) || \
               (pWidgetValue->listVal < 0)) {
                goto err;
            }
            list3Val = *pWidgetValue;
            break;
        case LIST4_INDEX:
            PSDK_LOG_DEBUG("List4 Opt");
            if ((pWidgetValue->listVal >= g_TestWidgetList[i].widgetAttribute.listAttribute.listItemNum) || \
               (pWidgetValue->listVal < 0)) {
                goto err;
            }
            list4Val = *pWidgetValue;
            break;
        default:
            break;
    }

    return PSDK_STAT_OK;

    err:
    return PSDK_STAT_ERR_OUT_OF_RANGE;
}

/**
 * @brief Get text input box parameters callback function.
 * @note Length of text input box name and description information must be less than APPFUNC_TEXT_INPUT_BOX_NAME_MAX_SIZE
 * and APPFUNC_TEXT_INPUT_BOX_DESCRIPTION_MAX_SIZE.
 * @param displayControlFlag Flag indicated whether text input box display in DJI Pilot.
 * @param boxName Name of text input box.
 * @param boxDescription Description information of text input box.
 * @return None.
 */
E_PsdkStat Test_GetTextInputBoxParamFunc(E_PsdkAppFuncTextInputBoxDisplayControlFlag *displayControlFlag,
                                         uint8_t *boxName, uint8_t *boxDescription)
{
    *displayControlFlag = textInputBoxDisplayControlFlag;
    strncpy((char *) boxName, (const char *) textInputBoxName, APPFUNC_TEXT_INPUT_BOX_NAME_MAX_SIZE);
    strncpy((char *) boxDescription, (const char *) textInputBoxDescription,
            APPFUNC_TEXT_INPUT_BOX_DESCRIPTION_MAX_SIZE);

    return PSDK_STAT_OK;
}

/****************** (C) COPYRIGHT DJI Innovations *****END OF FILE****/
