#ifndef  __MY_TASK_PSDKUSART3_REC__
#define  __MY_TASK_PSDKUSART3_REC__

#include "myinclude.h"

///////////////////////////////////////////////////////////////////////////////
//外部函数
extern void PSDKU3REC_TASK(void const * argument);   //PsdkRec任务实现


///////////////////////////////////////////////////////////////////////////////
//外部变量
extern TaskHandle_t 	    PsdkRec_Task_Handle;                //PsdkRec_Task_Handle任务句柄
extern SemaphoreHandle_t    uart3Mutex_Handle;
extern xQueueHandle         gpsQueue_Handle;                    //定义传递GPS信息的队列句柄


#endif
