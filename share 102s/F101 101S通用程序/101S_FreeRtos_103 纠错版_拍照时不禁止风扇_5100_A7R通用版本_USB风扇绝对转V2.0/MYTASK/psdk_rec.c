 
#include "psdk_rec.h"

////////////////////////////////////////////////////////////////////////////////////////
//全局变量
TaskHandle_t 	PsdkRec_Task_Handle 	= NULL;     //PsdkRec_Task_Handle任务句柄
SemaphoreHandle_t uart2Mutex_Handle=NULL;           //定义串口2的接收数据缓冲区的 互斥信号量
xQueueHandle gpsQueue_Handle=NULL;                  //定义传递GPS信息的队列句柄



//----------------------------------------------------
//PsdkRec任务实现
void PsdkRec_Task(void const *parameter)
{
   uint8_t res = 0;
   BaseType_t xReturn = pdPASS;    //定义一个创建信息返回值，默认为pdPASS 
    
    while (1) 
	{	
        //-----------------------------------------------------------------------------------    
        //xReturn = xSemaphoreTake(uart2Mutex_Handle, 0);                           //获取互斥量 在操作缓冲区时 线获取互斥量uart2Mutex_Handle 如果没获到则一直等待
        res=Bsp_usart2ReadChar(&Usart2RingRecBufDoStruct,&PsdkProtFrameAllStruct);  //串口2接收数据并处理并正确回复
        //xReturn = xSemaphoreGive( uart2Mutex_Handle );                            //释放互斥量
        //-----------------------------------------------------------------------------------
        res=res;
        xReturn=xReturn;
        //延时1个tick(1);
        vTaskDelay(1);   
    }
}





