#ifndef __MAIN__H
#define __MAIN__H

#include "myinclude.h"
#include "application.h"
#include "sys.h"

#define GPIO_Remap_SWJ_JTAGDisable  ((uint32_t)0x00300200)  /*!< JTAG-DP Disabled and SW-DP Enabled */ 

extern EventGroupHandle_t Key_EventGroup;     /*KEY事件*/
extern osMessageQId usart2SendMessageQHandle; /*消息队列*/
extern osMessageQId oledShowMessageQHandle;   /*消息队列*/


extern osThreadId OLED_TASK_Handle;           /*创建任务句柄*/
extern osThreadId KEY_TASK_Handle;            /*创建任务句柄*/
extern osThreadId CAM_TASK_Handle;            /*创建任务句柄*/
extern osThreadId RGB_TASK_Handle;            /*创建任务句柄*/
extern osThreadId RUNINIT_TASK_Handle;        /*创建任务句柄*/
extern osThreadId U2RECE_TASK_Handle;         /*创建任务句柄*/
extern osThreadId TRIGGER_TASK_Handle;        /*创建任务句柄*/
extern osThreadId U2SEND_TASK_Handle;         /*创建任务句柄*/
extern osThreadId PSDKU3REC_TASK_Handle;      /*创建任务句柄*/
extern osThreadId FAN_TASK_Handle;            /*创建任务句柄*/
extern osThreadId processRecDataTaskHandle;   /*创建任务句柄*/
extern osThreadId psdkTestTaskHandle;         /*创建任务句柄*/
extern osThreadId PPS_TASK_Handle;            /*创建任务句柄*/
extern osThreadId SJ_TASK_Handle;             /*创建任务句柄*/
#endif

