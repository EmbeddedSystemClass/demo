#ifndef  __MY_TASK_PSDK_REC__
#define  __MY_TASK_PSDK_REC__

#include "mypsdkinclude.h"

///////////////////////////////////////////////////////////////////////////////
//�ⲿ����
extern void PsdkRec_Task(void const *parameter);   //PsdkRec����ʵ��


///////////////////////////////////////////////////////////////////////////////
//�ⲿ����
extern TaskHandle_t 	    PsdkRec_Task_Handle;                //PsdkRec_Task_Handle������
extern SemaphoreHandle_t    uart2Mutex_Handle;
extern xQueueHandle         gpsQueue_Handle;                    //���崫��GPS��Ϣ�Ķ��о��


#endif
