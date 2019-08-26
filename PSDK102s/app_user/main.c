#include "main.h"
#include <psdk.h>

void APP_TASKCREATE(void const * argument);                                 /*线程声明*/

osThreadDef(APP_TASKCREATE1,APP_TASKCREATE,osPriorityAboveNormal, 0, 128);  /*定义用于创建其他任务 线程结构体*/
osThreadId APP_TASKCREATE_Handle=NULL;                                      /*创建任务句柄*/

osThreadDef(OLED_TASK1,OLED_TASK,osPriorityLow, 0, 512);                    /*定义显示屏 线程结构体*/
osThreadId OLED_TASK_Handle=NULL;

osThreadDef(KEY_TASK1,KEY_TASK,osPriorityNormal, 0, 200);                   /*定义机械按键 线程结构体*/ 
osThreadId KEY_TASK_Handle=NULL;
										
osThreadDef(CAM_TASK1,CAM_TASK,osPriorityBelowNormal, 0, 512);              /*定义相机控制 线程结构体*/
osThreadId CAM_TASK_Handle=NULL;

osThreadDef(RGB_TASK1,RGB_TASK,osPriorityLow, 0, 128);                      /*定义RGB灯 线程结构体*/
osThreadId RGB_TASK_Handle=NULL;

osThreadDef(RUNINIT_TASK1,RUNINIT_TASK,osPriorityAboveNormal, 0, 512);      /*定义逻辑走向 线程结构体*/   
osThreadId RUNINIT_TASK_Handle=NULL;

osThreadDef(Cam_status_update_TASK1,Cam_status_update_TASK,osPriorityAboveNormal, 0, 512);        /*相机状态更新 线程结构体*/
osThreadId Cam_status_update_TASK_Handle=NULL;

osThreadDef(TRIGGER_TASK1,TRIGGER_TASK,osPriorityRealtime, 0, 1024);        /*定义控制相机拍照 线程结构体*/
osThreadId TRIGGER_TASK_Handle=NULL;

osThreadDef(FAN_TASK1,FAN_TASK,osPriorityAboveNormal, 0, 128);              /*定义风扇控制 线程结构体*/          
osThreadId FAN_TASK_Handle=NULL;

osThreadDef(TEMP_TASK1,TEMP_TASK,osPriorityAboveNormal, 0, 300);            /*定义温度读取 线程结构体*/          
osThreadId TEMP_TASK_Handle=NULL;

osThreadDef(SJ_TASK1,SJ_TASK,osPriorityAboveNormal, 0, 512);                /*定义后台升级程序 线程结构体*/
osThreadId SJ_TASK_Handle=NULL;

osThreadDef(n_psdkRecTask, ProcessPsdkRecTask, osPriorityHigh, 0, 1800);    /*定义接收转接环数据 线程结构体*/
osThreadId processRecDataTaskHandle=NULL;

osThreadDef(n_psdkTestTask, PsdkTestTask, osPriorityRealtime, 0, 512);      /*定义向显示屏传递信息 线程结构体*/ 
osThreadId psdkTestTaskHandle=NULL;

osThreadDef(PPS_TASK1,PPS_TASK,osPriorityRealtime, 0, 1536);                /*定义控制记录位置信息逻辑 线程结构体*/
osThreadId PPS_TASK_Handle=NULL;


EventGroupHandle_t Key_EventGroup=NULL;       /*定义KEY事件*/

/*消息队列*/
osMessageQId SendMessageQHandle=NULL;         /*声明一个信号量容器*/  
osMessageQDef(SendMessageQ,5,uint32_t);  

osMessageQId oledShowMessageQHandle=NULL;			/*声明一个OLED屏通信信号量容器*/
osMessageQDef(oledShowMessageQ,2,uint32_t);


int main(void)
{
  	
  Sys_Init();           /*系统初始化*/
  BSP_Init();           /*开发板硬件初始化*/   
  application_init();   /*转接环通信初始化*/
	
  APP_TASKCREATE_Handle = osThreadCreate(osThread(APP_TASKCREATE1), NULL);    /*创建AppTaskCreate任务用于创建其它任务*/
  SCB->VTOR = FLASH_BASE | 0x35000;                                           /* Vector Table Relocation in Internal FLASH. 给应用程序276K ROM空间*/
  
  osKernelStart();      /*启动RTOS内核 ,开始多任务调度  */   
    
  while(1)              /*正常不会执行到这里*/
  {
		;
  }
}

/*************************************************
Function: void APP_TASKCREATE(void const * argument)
Description: 用于创建其它任务
Return: 无返回值
*************************************************/
void APP_TASKCREATE(void const * argument) /*用于创建其他任务的任务*/
{

    taskENTER_CRITICAL();               /*进入临界区*/

    Key_EventGroup=xEventGroupCreate(); /*创建按键事件*/
    
    SendMessageQHandle=osMessageCreate(osMessageQ(SendMessageQ),NULL);      /*创建485通信消息队列用于大板与小板之间通信*/ 
    oledShowMessageQHandle=osMessageCreate(osMessageQ(oledShowMessageQ),NULL);          /*创建OLED屏消息队列用于其他任务与OLED屏进行通信*/

    
     
    OLED_TASK_Handle = osThreadCreate(osThread(OLED_TASK1), NULL); /*创建 AppTASKOled  任务*/
    if(NULL!= OLED_TASK_Handle)
    {
        UART_Printf("$$创建OLED_TASK任务成功\r\n");
    }
		
    
    KEY_TASK_Handle = osThreadCreate(osThread(KEY_TASK1), NULL);  /*创建 KEY_TASK 任务*/
    if(NULL!= KEY_TASK_Handle)
    {
        UART_Printf("$$创建KEY_TASK任务成功\r\n");
    }
                            
    
    CAM_TASK_Handle = osThreadCreate(osThread(CAM_TASK1), NULL);  /*创建 CAM_TASK 任务*/
    if(NULL!= CAM_TASK_Handle)
    {
        UART_Printf("$$创建CAM_TASK任务成功\r\n");
    }
                            
    
    RGB_TASK_Handle = osThreadCreate(osThread(RGB_TASK1), NULL);  /*创建 RGB_TASK 任务*/
    if(NULL!= RGB_TASK_Handle)
    {
        UART_Printf("$$创建RGB_TASK任务成功\r\n");
    }
    
    
    RUNINIT_TASK_Handle = osThreadCreate(osThread(RUNINIT_TASK1), NULL); /*创建 RUNINIT_TASK 任务*/
    if(NULL!= RUNINIT_TASK_Handle)
    {
        UART_Printf("$$创建RUN_TASK任务成功\r\n");
    }   
             
   
    Cam_status_update_TASK_Handle = osThreadCreate(osThread(Cam_status_update_TASK1), NULL);   /*创建 Cam_status_update_TASK 任务*/
    if(NULL!= Cam_status_update_TASK_Handle)
    {
        UART_Printf("$$Cam_status_update_TASK\r\n");
    }
    
    
    TRIGGER_TASK_Handle = osThreadCreate(osThread(TRIGGER_TASK1), NULL); /*创建 TRIGGER_TASK 任务*/
    if(NULL!= TRIGGER_TASK_Handle)
    {
        UART_Printf("$$创建TRIGGER_TASK任务成功\r\n");
    }
     
		
    FAN_TASK_Handle = osThreadCreate(osThread(FAN_TASK1), NULL);         /*创建 FAN_TASK 任务*/
    if(NULL!= FAN_TASK_Handle)
    {
        UART_Printf("$$创建FAN_TASK任务成功\r\n");
    } 

		
		TEMP_TASK_Handle = osThreadCreate(osThread(TEMP_TASK1), NULL);         /*创建 TEMP_TASK 任务*/
    if(NULL!= TEMP_TASK_Handle)
    {
        UART_Printf("$$创建TEMP_TASK任务成功\r\n");
		}
		
		
    SJ_TASK_Handle = osThreadCreate(osThread(SJ_TASK1), NULL);           /*创建 SJ_TASK 任务*/
    if(NULL!= SJ_TASK_Handle)
    {
        UART_Printf("$$创建SJ_TASK任务成功\r\n");
    } 
		
    
    PPS_TASK_Handle = osThreadCreate(osThread(PPS_TASK1), NULL);         /*创建 PPS_TASK 任务*/
    if(NULL!= PPS_TASK_Handle)
    {
        UART_Printf_cocoguojia("$$创建PPS_TASK任务成功\r\n");
    }        
    
		
    processRecDataTaskHandle = osThreadCreate(osThread(n_psdkRecTask), NULL); /*创建 ProcessPsdkRecTask 任务*/
    if(NULL!=processRecDataTaskHandle)
    {
        UART_Printf("ProcessPsdkRecTask is run\r\n");
    }
    
		
    psdkTestTaskHandle = osThreadCreate(osThread(n_psdkTestTask), NULL);      /*创建 PsdkTestTask 任务*/
    if(NULL!=psdkTestTaskHandle)
    {
        UART_Printf("PsdkTestTask is run\r\n");
    }
     			
		
    osThreadTerminate (APP_TASKCREATE_Handle); 							/*删除AppTASKCreate任务*/ 
    taskEXIT_CRITICAL();            								        /*退出临界区*/
}










