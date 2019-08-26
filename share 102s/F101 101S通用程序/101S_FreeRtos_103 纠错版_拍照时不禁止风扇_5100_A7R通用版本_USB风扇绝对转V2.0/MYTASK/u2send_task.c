 
 
 
#include "u2send_task.h"
  

static void comFor5CamOn(void);
static void comFor5CamOff(void);
static void comFor5CamPhoto(void);
static void askFor5Cam(void);
static void comForKey(void);

TaskHandle_t 	U2SEND_TASK_Handle=NULL;

//-------------------------------------------------------------------------
//任务函数
void  U2SEND_TASK (void* parameter)
{
    BaseType_t xReturn=pdFAIL;
	uint8_t p_msg;
    uint8_t n=0;

	while (1) 
	{	     
        xReturn=xQueueReceive(usart2Send_Queue,&p_msg,portMAX_DELAY);
         
		if(pdTRUE==xReturn)
		{
			n=p_msg;
            switch(n)
            {
                case USART2COM_OPEN:
                comFor5CamOn();
                break;  

                case USART2COM_SHUT:
                comFor5CamOff();
                break;

                case USART2COM_PHOTO:
                comFor5CamPhoto();
                break;

                case USART2COM_ASK:
                askFor5Cam();
                break;

                case USART2COM_KEY:
                comForKey();
                break;

                default:
                break;
            }                
		}
    }
}
    
//------------------------------------------
//开机 自身23ms+8延时
static void comFor5CamOn(void)
{
    portTickType xLastWakeTime;
    xLastWakeTime=xTaskGetTickCount();//使用当前时间 初始化xLastWakeTime变量
	
    RS485_TXD_EN;
    vTaskDelayUntil(&xLastWakeTime,3);
    cocoguojia_usartNPutStr(2,"@@@");//前导@
    cocoguojia_usartNPutStr(2,"@X1#");//发送关机命令
    vTaskDelayUntil(&xLastWakeTime,20);
}

//------------------------------------------
//关机 自身23ms+8延时
static void comFor5CamOff(void)
{
    portTickType xLastWakeTime;
    xLastWakeTime=xTaskGetTickCount();//使用当前时间 初始化xLastWakeTime变量
    
    RS485_TXD_EN;
    vTaskDelayUntil(&xLastWakeTime,3);
    cocoguojia_usartNPutStr(2,"@@@");//前导@
    cocoguojia_usartNPutStr(2,"@X2#");//发送关机命令
    vTaskDelayUntil(&xLastWakeTime,20);
}

//------------------------------------------
//拍照 自身23ms+8延时
static void comFor5CamPhoto(void)
{
    portTickType xLastWakeTime;
    xLastWakeTime=xTaskGetTickCount();//使用当前时间 初始化xLastWakeTime变量
    
    RS485_TXD_EN;
    vTaskDelayUntil(&xLastWakeTime,3);
    cocoguojia_usartNPutStr(2,"@@@");//前导@
    cocoguojia_usartNPutStr(2,"@X3#");//发送关机命令
    vTaskDelayUntil(&xLastWakeTime,20);
    
}

extern uint8_t  g_usart2forSendRunFlag;     //串口2总发送正在进行标志

//------------------------------------------
//查询状态 自身180ms+20ms延时
static void askFor5Cam(void)
{
    portTickType xLastWakeTime;
    xLastWakeTime=xTaskGetTickCount();//使用当前时间 初始化xLastWakeTime变量
    
    RS485_TXD_EN;
    vTaskDelayUntil(&xLastWakeTime,3);
    cocoguojia_usartNPutStr(2,"@14#");//发送查询命令   
    while(1==g_usart2forSendRunFlag){vTaskDelayUntil(&xLastWakeTime,1);}//在使能RS485接收之前 要判断一下是否发送完全标志 
    vTaskDelayUntil(&xLastWakeTime,2);
    RS485_RXD_EN;
    vTaskDelayUntil(&xLastWakeTime,30);
    
    RS485_TXD_EN;
    vTaskDelayUntil(&xLastWakeTime,3);
    cocoguojia_usartNPutStr(2,"@24#");//发送查询命令
    while(1==g_usart2forSendRunFlag){vTaskDelayUntil(&xLastWakeTime,1);}//在使能RS485接收之前 要判断一下是否发送完全标志
    vTaskDelayUntil(&xLastWakeTime,2);
    RS485_RXD_EN;
    vTaskDelayUntil(&xLastWakeTime,30);
    
    RS485_TXD_EN;
    vTaskDelayUntil(&xLastWakeTime,3);
    cocoguojia_usartNPutStr(2,"@34#");//发送查询命令
    while(1==g_usart2forSendRunFlag){vTaskDelayUntil(&xLastWakeTime,1);}//在使能RS485接收之前 要判断一下是否发送完全标志
    vTaskDelayUntil(&xLastWakeTime,2);
    RS485_RXD_EN;
    vTaskDelayUntil(&xLastWakeTime,30);
   
    RS485_TXD_EN;
    vTaskDelayUntil(&xLastWakeTime,3);
    cocoguojia_usartNPutStr(2,"@44#");//发送查询命令
    while(1==g_usart2forSendRunFlag){vTaskDelayUntil(&xLastWakeTime,1);}//在使能RS485接收之前 要判断一下是否发送完全标志
    vTaskDelayUntil(&xLastWakeTime,2);
    RS485_RXD_EN;
    vTaskDelayUntil(&xLastWakeTime,30);
    
    RS485_TXD_EN;
    vTaskDelayUntil(&xLastWakeTime,3);
    cocoguojia_usartNPutStr(2,"@54#");//发送查询命令
    while(1==g_usart2forSendRunFlag){vTaskDelayUntil(&xLastWakeTime,1);}//在使能RS485接收之前 要判断一下是否发送完全标志
    vTaskDelayUntil(&xLastWakeTime,2);
    RS485_RXD_EN;
    vTaskDelayUntil(&xLastWakeTime,30);
    
    RS485_TXD_EN;   
}


//------------------------------------------
//OK按键 自身23ms+5延时
static void comForKey(void)
{
    portTickType xLastWakeTime;
    xLastWakeTime=xTaskGetTickCount();//使用当前时间 初始化xLastWakeTime变量
    
    RS485_TXD_EN;
    vTaskDelayUntil(&xLastWakeTime,3);
    cocoguojia_usartNPutStr(2,"@@@");//前导@
    cocoguojia_usartNPutStr(2,"@X5#");//发送关机命令
    vTaskDelayUntil(&xLastWakeTime,20);
}








