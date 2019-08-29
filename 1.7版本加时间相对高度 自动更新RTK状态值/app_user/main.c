

#include "myinclude.h"
 
#include <psdk.h>
#include "sys.h"
#include "application.h"



#define GPIO_Remap_SWJ_JTAGDisable  ((uint32_t)0x00300200)  /*!< JTAG-DP Disabled and SW-DP Enabled */ 

//--------------------------------------------------------------------------------
//线程声明
void APP_TASKCREATE(void const * argument);//用于创建其他任务的任务
osThreadDef(APP_TASKCREATE1,APP_TASKCREATE,osPriorityAboveNormal, 0, 128);  //定义线程结构体
osThreadId APP_TASKCREATE_Handle=NULL;//创建任务句柄

osThreadDef(OLED_TASK1,OLED_TASK,osPriorityLow, 0, 512);//定义线程结构体   //显示没有获取温度
osThreadId OLED_TASK_Handle=NULL;//创建任务句柄

osThreadDef(KEY_TASK1,KEY_TASK,osPriorityNormal, 0, 256);//定义线程结构体   //没有获取温度
osThreadId KEY_TASK_Handle=NULL;//创建任务句柄
										
osThreadDef(CAM_TASK1,CAM_TASK,osPriorityBelowNormal, 0, 512);//定义线程结构体    
osThreadId CAM_TASK_Handle=NULL;//创建任务句柄

osThreadDef(RGB_TASK1,RGB_TASK,osPriorityLow, 0, 128);//定义线程结构体
osThreadId RGB_TASK_Handle=NULL;//创建任务句柄

osThreadDef(RUNINIT_TASK1,RUNINIT_TASK,osPriorityAboveNormal, 0, 512);//定义线程结构体
osThreadId RUNINIT_TASK_Handle=NULL;//创建任务句柄

osThreadDef(U2RECE_TASK1,U2RECE_TASK,osPriorityAboveNormal, 0, 512);//定义线程结构体
osThreadId U2RECE_TASK_Handle=NULL;//创建任务句柄

osThreadDef(TRIGGER_TASK1,TRIGGER_TASK,osPriorityRealtime, 0, 1024);//包含SD卡 要大空间  //定义线程结构体  //获取温度的功能那个
osThreadId TRIGGER_TASK_Handle=NULL;//创建任务句柄

osThreadDef(U2SEND_TASK1,U2SEND_TASK,osPriorityHigh, 0, 1024);//定义线程结构体
osThreadId U2SEND_TASK_Handle=NULL;//创建任务句柄

osThreadDef(FAN_TASK1,FAN_TASK,osPriorityAboveNormal, 0, 256);//风扇控制任务         //定义线程结构体
osThreadId FAN_TASK_Handle=NULL;//创建任务句柄

osThreadDef(n_psdkRecTask, ProcessPsdkRecTask, osPriorityHigh, 0, 1800); //定义线程结构体
osThreadId processRecDataTaskHandle=NULL;

osThreadDef(n_psdkTestTask, PsdkTestTask, osPriorityRealtime, 0, 512);      //定义线程结构体   传给大疆显示屏的数据  温度 相机状态
osThreadId psdkTestTaskHandle=NULL;

osThreadDef(PPS_TASK1,PPS_TASK,osPriorityRealtime, 0, 1536);//PPS计时任务   定义线程结构体
osThreadId PPS_TASK_Handle=NULL;//创建任务句柄



//--------------------------------------------------------------------------------------------
//事件用FreeRTOS 内核函数 因为CMSIS RTOS封装的事件其实TMD是个任务事件通知 只能发给一个任务 真JB坑
EventGroupHandle_t Key_EventGroup=NULL;     //KEY事件


//消息队列
osMessageQId usart2SendMessageQHandle=NULL;   //声明一个信号量容器
osMessageQDef(usart2SendMessageQ,5,uint32_t);   //sheng'ming

osMessageQId oledShowMessageQHandle=NULL;			//声明一个信号量容器
osMessageQDef(oledShowMessageQ,2,uint32_t);

    

//--------------------------------------------------------------------
//主函数
int main(void)
{
  	
  Sys_Init();//系统初始化
    
  //第一步：开发板硬件初始化
  BSP_Init();
    
  application_init();//板级初始化
	
  //第二步：创建AppTaskCreate任务用任务
  APP_TASKCREATE_Handle = osThreadCreate(osThread(APP_TASKCREATE1), NULL);
  
  SCB->VTOR = FLASH_BASE | 0x35000; /* Vector Table Relocation in Internal FLASH. 给应用程序276K ROM空间*/
  //第三步：启动FreeRTOS，开始多任务调度 
  osKernelStart();//启动RTOS内核      就是一个开启任务调度
  
  //--------------------------------------------------------------------
  //正常不会执行到这里
  while(1)  
  {
		;
  }
}

//--------------------------------------------------------------------
//用于创建其他任务的任务
void APP_TASKCREATE(void const * argument)
{
	  
    //#################################
    taskENTER_CRITICAL();           								    //进入临界区

    //--------------------------------------------------------------------------------------------
    //事件用FreeRTOS 内核函数 因为CMSIS RTOS封装的事件其实TMD是个任务事件通知 只能发给一个任务 真JB坑
    Key_EventGroup=xEventGroupCreate();
    
    usart2SendMessageQHandle=osMessageCreate(osMessageQ(usart2SendMessageQ),NULL);
           
    oledShowMessageQHandle=osMessageCreate(osMessageQ(oledShowMessageQ),NULL);

    
     //创建 AppTASKOled  任务
    OLED_TASK_Handle = osThreadCreate(osThread(OLED_TASK1), NULL);
    if(NULL!= OLED_TASK_Handle)
    {
        UART_Printf_cocoguojia("$$创建OLED_TASK任务成功\r\n");
    }
    // uxHighWaterMark=uxTaskGetStackHighWaterMark( OLED_TASK_Handle );    //查询大小
		// PSDK_LOG_DEBUG("OLED_TASK  %d\r\n",uxHighWaterMark);
		// PSDK_LOG_DEBUG("OLED_TASK");
    //创建 KEY_TASK 任务
    KEY_TASK_Handle = osThreadCreate(osThread(KEY_TASK1), NULL);
    if(NULL!= KEY_TASK_Handle)
    {
        UART_Printf_cocoguojia("$$创建KEY_TASK任务成功\r\n");
    }
                            
    //创建 CAM_TASK 任务
    CAM_TASK_Handle = osThreadCreate(osThread(CAM_TASK1), NULL);
    if(NULL!= CAM_TASK_Handle)
    {
        UART_Printf_cocoguojia("$$创建CAM_TASK任务成功\r\n");
    }
                            
    //创建 RGB_TASK 任务
    RGB_TASK_Handle = osThreadCreate(osThread(RGB_TASK1), NULL);
    if(NULL!= RGB_TASK_Handle)
    {
        UART_Printf_cocoguojia("$$创建RGB_TASK任务成功\r\n");
    }
    
    //创建 RUNINIT_TASK 任务
    RUNINIT_TASK_Handle = osThreadCreate(osThread(RUNINIT_TASK1), NULL);
    if(NULL!= RUNINIT_TASK_Handle)
    {
        UART_Printf_cocoguojia("$$创建RUN_TASK任务成功\r\n");
    }   
             
    //创建 U2RECE_TASK 任务
    U2RECE_TASK_Handle = osThreadCreate(osThread(U2RECE_TASK1), NULL);
    if(NULL!= U2RECE_TASK_Handle)
    {
        UART_Printf_cocoguojia("$$创建U2RECE_TASK任务成功\r\n");
    }
    
    //创建 TRIGGER_TASK 任务
    TRIGGER_TASK_Handle = osThreadCreate(osThread(TRIGGER_TASK1), NULL);
    if(NULL!= TRIGGER_TASK_Handle)
    {
        UART_Printf_cocoguojia("$$创建TRIGGER_TASK任务成功\r\n");
    }
    
    //创建 U2SEND_TASK 任务
    U2SEND_TASK_Handle = osThreadCreate(osThread(U2SEND_TASK1), NULL);
    if(NULL!= U2SEND_TASK_Handle)
    {
        UART_Printf_cocoguojia("$$创建U2SEND_TASK任务成功\r\n");
    }   

    //创建 FAN_TASK 任务
    FAN_TASK_Handle = osThreadCreate(osThread(FAN_TASK1), NULL);
    if(NULL!= FAN_TASK_Handle)
    {
        UART_Printf_cocoguojia("$$创建FAN_TASK任务成功\r\n");
    } 

    //创建 PPS_TASK 任务
    PPS_TASK_Handle = osThreadCreate(osThread(PPS_TASK1), NULL);
    if(NULL!= PPS_TASK_Handle)
    {
        UART_Printf_cocoguojia("$$创建PPS_TASK任务成功\r\n");
    }        
    
    processRecDataTaskHandle = osThreadCreate(osThread(n_psdkRecTask), NULL);
    if(NULL!=processRecDataTaskHandle)
    {
        UART_Printf_cocoguojia("ProcessPsdkRecTask is run\r\n");
    }
    
    psdkTestTaskHandle = osThreadCreate(osThread(n_psdkTestTask), NULL);
    if(NULL!=psdkTestTaskHandle)
    {
        UART_Printf_cocoguojia("PsdkTestTask is run\r\n");
    }
                           
    osThreadTerminate (APP_TASKCREATE_Handle); 							//删除AppTASKCreate任务
  
  
    taskEXIT_CRITICAL();            								    //退出临界区
    //##################################
}











