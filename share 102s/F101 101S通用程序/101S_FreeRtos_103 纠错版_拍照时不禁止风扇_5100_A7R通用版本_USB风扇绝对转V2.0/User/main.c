#include "myinclude.h"


static TaskHandle_t 	APP_TASKCREATE_Handle 	= NULL;		//创建任务句柄


//--------------------------------------------------------------------------------
//函数声明
static void APP_TASKCREATE	(void);						    //用于创建其他任务的任务

EventGroupHandle_t Key_EventGroup=NULL;           //KEY事件
QueueHandle_t usart2Send_Queue=NULL;              //消息队列


//--------------------------------------------------------------------------------
//主函数
int main(void)
{	
  BaseType_t xReturn = pdPASS;	//定义一个创建信息返回值，默认为pdPASS
  
  //第一步：开发板硬件初始化
  BSP_Init();
	
  //第二步：创建AppTaskCreate任务用任务
  xReturn = xTaskCreate((TaskFunction_t )	APP_TASKCREATE,  		/* 任务入口函数 */
                        (const char*    )	"APP_TASKCREATE",		/* 任务名字 */
                        (uint16_t       )	128,  					    /* 任务栈大小 */
                        (void*          )	NULL,					      /* 任务入口函数参数 */
                        (UBaseType_t    )	1, 						      /* 任务的优先级 */
                        (TaskHandle_t*  )	&APP_TASKCREATE_Handle);	/* 任务控制块指针 */ 
  
  //第三步：启动FreeRTOS，开始多任务调度           
  if(pdPASS == xReturn)
  {
		vTaskStartScheduler();   //启动任务，开启调度
  }
  else
  {
		return -1; 
  }	  
  
  //--------------------------------------------------------------------
  //正常不会执行到这里
  while(1)  
  {
		;
  }
}

//--------------------------------------------------------------------
//用于创建其他任务的任务
static void APP_TASKCREATE(void)
{
    BaseType_t xReturn = pdPASS;		           /* 定义一个创建信息返回值，默认为pdPASS */

    taskENTER_CRITICAL();           					 /* 进入临界区 */

    Key_EventGroup=xEventGroupCreate();
    usart2Send_Queue=xQueueCreate(5,1);
    
    
    //创建 AppTASKOled 任务
    xReturn = xTaskCreate(  (TaskFunction_t )	OLED_TASK, 			    /* 任务入口函数*/
                            (const char*    )	"OLED_TASK",			  /* 任务名字*/
                            (uint16_t       )	1024,   				    /* 任务栈大小*/
                            (void*          )	NULL,				        /* 任务入口函数参数*/
                            (UBaseType_t    )	2,	    			      /* 任务的优先级*/
                            (TaskHandle_t*  )	&OLED_TASK_Handle);	/* 任务控制块指针*/
    if(pdPASS == xReturn)
    {
        PRINTF_BEFORE printf("$$创建OLED_TASK任务成功\r\n");
    }
                            
    //创建 KEY_TASK 任务
    xReturn = xTaskCreate(  (TaskFunction_t )	KEY_TASK, 			    /* 任务入口函数*/
                            (const char*    )	"KEY_TASK",			    /* 任务名字*/
                            (uint16_t       )	512,   				      /* 任务栈大小*/
                            (void*          )	NULL,				        /* 任务入口函数参数*/
                            (UBaseType_t    )	3,	    			      /* 任务的优先级*/
                            (TaskHandle_t*  )	&KEY_TASK_Handle);  /* 任务控制块指针*/
    if(pdPASS == xReturn)
    {
        PRINTF_BEFORE printf("$$创建KEY_TASK任务成功\r\n");
    }
                            
    //创建 CAM_TASK 任务
    xReturn = xTaskCreate(  (TaskFunction_t )	CAM_TASK, 			    /* 任务入口函数*/
                            (const char*    )	"CAM_TASK",			    /* 任务名字*/
                            (uint16_t       )	1024,   				    /* 任务栈大小*/
                            (void*          )	NULL,				        /* 任务入口函数参数*/
                            (UBaseType_t    )	4,	    			      /* 任务的优先级*/
                            (TaskHandle_t*  )	&CAM_TASK_Handle);	/* 任务控制块指针*/
    if(pdPASS == xReturn)
    {
        PRINTF_BEFORE printf("$$创建CAM_TASK任务成功\r\n");
    }
                            
    //创建 RGB_TASK 任务
    xReturn = xTaskCreate(  (TaskFunction_t )	RGB_TASK, 			    /* 任务入口函数*/
                            (const char*    )	"RGB_TASK",			    /* 任务名字*/
                            (uint16_t       )	512,   				      /* 任务栈大小*/
                            (void*          )	NULL,				        /* 任务入口函数参数*/
                            (UBaseType_t    )	5,	    			      /* 任务的优先级*/
                            (TaskHandle_t*  )	&RGB_TASK_Handle);  /* 任务控制块指针*/
    if(pdPASS == xReturn)
    {
        PRINTF_BEFORE printf("$$创建RGB_TASK任务成功\r\n");
    }
    
    //创建 RUNINIT_TASK 任务
    xReturn = xTaskCreate(  (TaskFunction_t )	RUNINIT_TASK, 			    /* 任务入口函数*/
                            (const char*    )	"RUNINIT_TASK",			    /* 任务名字*/
                            (uint16_t       )	512,   				          /* 任务栈大小*/
                            (void*          )	NULL,				            /* 任务入口函数参数*/
                            (UBaseType_t    )	6,	    			          /* 任务的优先级*/
                            (TaskHandle_t*  )	&RUNINIT_TASK_Handle);	/* 任务控制块指针*/
    if(pdPASS == xReturn)
    {
        PRINTF_BEFORE printf("$$创建RUN_TASK任务成功\r\n");
    }
          
             
    //创建 U2RECE_TASK 任务
    xReturn = xTaskCreate(  (TaskFunction_t )	Cam_status_update_TASK, 			    /* 任务入口函数*/
                            (const char*    )	"Cam_status_update_TASK",			    /* 任务名字*/
                            (uint16_t       )	512,   				                    /* 任务栈大小*/
                            (void*          )	NULL,				                      /* 任务入口函数参数*/
                            (UBaseType_t    )	7,	    			                    /* 任务的优先级*/														
                            (TaskHandle_t*  )	&Cam_status_update_TASK_Handle);	/* 任务控制块指针*/
    if(pdPASS == xReturn)
    {
        PRINTF_BEFORE printf("$$创建Cam_status_update_TASK 成功\r\n");
    }
    
    //创建 TRIGGER_TASK 任务
    xReturn = xTaskCreate(  (TaskFunction_t )	TRIGGER_TASK, 			    /* 任务入口函数*/
                            (const char*    )	"TRIGGER_TASK",			    /* 任务名字*/
                            (uint16_t       )	1024,   				        /* 任务栈大小*/
                            (void*          )	NULL,				            /* 任务入口函数参数*/
                            (UBaseType_t    )	8,	    			          /* 任务的优先级*/
                            (TaskHandle_t*  )	&TRIGGER_TASK_Handle);	/* 任务控制块指针*/
    if(pdPASS == xReturn)
    {
        PRINTF_BEFORE printf("$$创建TRIGGER_TASK任务成功\r\n");
    }

    //创建 TRIGGER_TASK 任务
    xReturn = xTaskCreate(  (TaskFunction_t )	TEMP_TASK, 			        /* 任务入口函数*/
                            (const char*    )	"TEMP_TASK",			      /* 任务名字*/
                            (uint16_t       )	1024,   				        /* 任务栈大小*/
                            (void*          )	NULL,				            /* 任务入口函数参数*/
                            (UBaseType_t    )	8,	    			          /* 任务的优先级*/
                            (TaskHandle_t*  )	&TEMP_TASK_Handle);   	/* 任务控制块指针*/
    if(pdPASS == xReturn)
    {
        PRINTF_BEFORE printf("$$创建TEMP_TASK成功\r\n");
    }		
    
    #ifdef CPU_TASK_EN 
    /* 创建CPU_TASK任务 */
    xReturn = xTaskCreate((TaskFunction_t )CPU_TASK,       /* 任务入口函数 */
                        (const char*    )"CPU_TASK",       /* 任务名字 */
                        (uint16_t       )512,              /* 任务栈大小 */
                        (void*          )NULL,	           /* 任务入口函数参数 */
                        (UBaseType_t    )12,	             /* 任务的优先级 */
                        (TaskHandle_t*  )&CPU_TASK_Handle);/* 任务控制块指针 */
    if(pdPASS == xReturn)
    PRINTF_BEFORE printf("创建CPU_Task任务成功!\r\n");
    #endif
        

		
    vTaskDelete(APP_TASKCREATE_Handle); 							    /* 删除AppTASKCreate任务 */
  
  
    taskEXIT_CRITICAL();            								      /* 退出临界区 */

}











