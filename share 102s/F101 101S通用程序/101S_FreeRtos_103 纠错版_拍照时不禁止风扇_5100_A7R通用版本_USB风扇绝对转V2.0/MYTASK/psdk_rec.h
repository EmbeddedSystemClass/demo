#ifndef  __MY_TASK_PSDK_REC__
#define  __MY_TASK_PSDK_REC__

#include "mypsdkinclude.h"

///////////////////////////////////////////////////////////////////////////////
//外部函数
extern void PsdkRec_Task(void const *parameter);   //PsdkRec任务实现


///////////////////////////////////////////////////////////////////////////////
//外部变量
extern TaskHandle_t 	    PsdkRec_Task_Handle;                //PsdkRec_Task_Handle任务句柄
extern SemaphoreHandle_t    uart2Mutex_Handle;
extern xQueueHandle         gpsQueue_Handle;                    //定义传递GPS信息的队列句柄


#endif
