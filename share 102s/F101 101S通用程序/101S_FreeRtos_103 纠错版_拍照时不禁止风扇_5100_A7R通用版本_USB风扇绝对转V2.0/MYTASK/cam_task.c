#include "cam_task.h"
 
/*************************************************
Function: void  CAM_TASK (void const * argument)
Description: 判断相机处于什么样的状态，从而进行界面的切换和开关机动作
Return: 无返回值
*************************************************/

TaskHandle_t 	CAM_TASK_Handle = NULL;		//创建任务句柄

void  CAM_TASK (void* parameter)
{  
  	uint8_t n = 0,i = 0,j = 0;
    EventBits_t eventTemp=0;

  

    /* 等待标志g_camTaskCanTakeOverFlag置位 cam任务可以接管标志置位 */
    while (0 == g_camTaskCanTakeOverFlag) 
    {
        vTaskDelay(10);
    }
	
    vTaskDelay(100);    
	  POWER_NO;            /* 关机@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
    vTaskDelay(100);
    
    
    /* 检测是否真正关机 */
    for(i = 1; i <= 5; i++)
    {
        if(0 != g_camPowerFlagTable[i])
        {
            i = 210;
        }
    }
    if(200 <= i)
    {
        g_offOrOnTureStateFlag = SYSTEMSTATE_SHUTFAIL;    /* 关机失败 */  	
    }
    else
    {
        g_offOrOnTureStateFlag = SYSTEMSTATE_OFFTRUE;     /* 关机成功 */
    }
    xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite); /* 任务的句柄，发送的数据 32位，覆盖当前通知 */   
    FMQ_ON;
    vTaskDelay(500);
    FMQ_OFF;
    vTaskDelay(100);
    n = 0;    
   
	while (1) 
	{  
    /*   真实关机了   */
    if(SYSTEMSTATE_OFFTRUE == g_offOrOnTureStateFlag)        
		{             
            /* 检测问5个相机的开关机状态 */         
            for(i = 1; i <= 5; i++)
            {
                if(0 != g_camPowerFlagTable[i])
                {
                    i = 210;
                }
            }

            /* 如果有相机没关机或者突然开机 */
            if(200 <= i)
            {
                POWER_NO;           /* 关机@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
                vTaskDelay(300);  
                g_offOrOnTureStateFlag=SYSTEMSTATE_SHUTFAIL;
                xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite);            /* 任务的句柄，发送的数据 32位，覆盖当前通知 */
                FMQ_ON;
                vTaskDelay(500);
                FMQ_OFF;
                vTaskDelay(100);
                n = 0;                
            }
            
            /* 检测是否有按键被按下 按下则进入 开机中 */            
            eventTemp = xEventGroupWaitBits(Key_EventGroup,KEY_SHORT,pdTRUE,pdFALSE,0);/* 事件对象句柄，接受任务感兴趣的事件，退出时清除事件位，满足感兴趣的任意事件,不等待  */         
            if(KEY_SHORT == (eventTemp&KEY_SHORT))//按键短按了
            {
                g_offOrOnTureStateFlag = SYSTEMSTATE_GOTOOPEN;
                xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite);   /* 任务的句柄，发送的数据 32位，覆盖当前通知 */
                FMQ_ON;
                vTaskDelay(500);
                FMQ_OFF;
                vTaskDelay(100);
                n = 0;
            }        
     }
		
		
		/*  关机失败  */   
    if(SYSTEMSTATE_SHUTFAIL == g_offOrOnTureStateFlag)	         
		{	
            n++;
            if((1 == n) || (0 == (n % 3)))
            {
                FMQ_ON;
            }
            else
            {
                FMQ_OFF;
            }    
            if(8 <= n)
            {
								POWER_NO;       /* 关机@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
                vTaskDelay(100);                
                n=0;
            }
           
			/* 分别检测5个相机的开关机状态  */
            vTaskDelay(300);       
            for(i = 1; i <= 5; i++)
            {
                if(0 != g_camPowerFlagTable[i])
                {
                    i = 210;
                }
            }
            if(100 > i)
            {
                g_offOrOnTureStateFlag = SYSTEMSTATE_OFFTRUE;
                xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite);  /* 任务的句柄，发送的数据 32位，覆盖当前通知 */
                FMQ_ON;
                vTaskDelay(500);
                FMQ_OFF;
                vTaskDelay(100);
                n = 0;   	
            }
            else
            {                
                eventTemp = xEventGroupWaitBits(Key_EventGroup,KEY_SHORT,pdTRUE,pdFALSE,0);/* 事件对象句柄，接受任务感兴趣的事件，退出时清除事件位，满足感兴趣的任意事件,不等待 */         
                if(KEY_SHORT==(eventTemp&KEY_SHORT))/* 按键短按了 */
                {
                    g_offOrOnTureStateFlag = SYSTEMSTATE_GOTOOPEN;
                    xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite);       /* 任务的句柄，发送的数据 32位，覆盖当前通知 */
                    FMQ_ON;
                    vTaskDelay(500);
                    FMQ_OFF;
                    vTaskDelay(100);
                    n = 0;
                } 
            }                
     } 
		
		 
		/* 开机失败 */    
    if(SYSTEMSTATE_OPENFAIL == g_offOrOnTureStateFlag)			    
		{	
            n++;
            if((1 == n) || (0 == (n % 3)))
            {
                FMQ_ON;
            }
            else
            {
                FMQ_OFF;
            }    
            if(8 <= n)
            {
								POWER_EN;       /* 开机@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
                vTaskDelay(100);
                n = 0;
            }
            
                   
           /* 分别检测5个相机的开关机状态  */
            vTaskDelay(300);         
            for(i = 1; i <= 5; i++)
            {
                if(1 != g_camPowerFlagTable[i])
                {
                    i = 210;
                }
            }
            if(100 > i)
            {
								vTaskDelay(3000);
								vTaskDelay(3000);
								vTaskDelay(3000);
								PHOTOALL_EN;               /* 拍照@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */  
								vTaskDelay(100);
								PHOTOALL_NO;							
								vTaskDelay(3000);		
                g_offOrOnTureStateFlag = SYSTEMSTATE_ONTRUE;
                xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite);  /* 任务的句柄，发送的数据 32位，覆盖当前通知 */
                FMQ_ON;
                vTaskDelay(500);
                FMQ_OFF;
                vTaskDelay(100);
                n = 0;   	
            }
            else
            {                
                eventTemp = xEventGroupWaitBits(Key_EventGroup,KEY_SHORT,pdTRUE,pdFALSE,0);/* 事件对象句柄，接受任务感兴趣的事件，退出时清除事件位，满足感兴趣的任意事件,不等待 */         
                if(KEY_SHORT == (eventTemp&KEY_SHORT))//按键短按了
                {
                    g_offOrOnTureStateFlag = SYSTEMSTATE_GOTOSHUT;
                    xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite);   /* 任务的句柄，发送的数据 32位，覆盖当前通知 */
                    FMQ_ON;
                    vTaskDelay(500);
                    FMQ_OFF;
                    vTaskDelay(100);
                } 
            }                
     } 
		
		 
		/* 关机中 */  
     if(SYSTEMSTATE_GOTOSHUT == g_offOrOnTureStateFlag)	       
		{	
		    POWER_NO;       /* 关机@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
            vTaskDelay(100);
            
            for(j = 0; j < 50; j++)   //15s
            {              
				/* 分别检测5个相机的开关机状态  */
                vTaskDelay(300);
                
                for(i = 1; i <= 5; i++)
                {
                    if(1 == g_camPowerFlagTable[i])
                    {
                        i = 210;
                    }
                }
                if(100 > i)
                {
                    j = 220;
                }   
            }
            if(200 <= j)
            {
                g_offOrOnTureStateFlag = SYSTEMSTATE_OFFTRUE;
                xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite);   /* 任务的句柄，发送的数据 32位，覆盖当前通知 */
                FMQ_ON;
                vTaskDelay(500);
                FMQ_OFF;
                vTaskDelay(100);
                n = 0;   	
            }
            else
            {
			        	POWER_NO;         /* 关机@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
                vTaskDelay(100); 
                
                g_offOrOnTureStateFlag = SYSTEMSTATE_SHUTFAIL;     	
                xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite);    /* 任务的句柄，发送的数据 32位，覆盖当前通知 */
                FMQ_ON;
                vTaskDelay(500);
                FMQ_OFF;
                vTaskDelay(100);
                n = 0;    
            } 
     }
		
		 
		/* 开机中   */
    if(SYSTEMSTATE_GOTOOPEN == g_offOrOnTureStateFlag)								       
		{	  
			      POWER_EN;       /* 开机@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
            vTaskDelay(100);      
            for(j = 0; j < 50; j++)//15s
            {
				/* 分别检测5个相机的开关机状态  */
                vTaskDelay(300);      
                for(i = 1; i <= 5; i++)
                {
                    if(0 == g_camPowerFlagTable[i])
                    {
                        i = 210;
                    }
                }
                if(100 > i)
                {
                    j = 220;
                }   
            }
            //j=230;//调试//////////////////////////////
            if(200 <= j)
            {  
				
								vTaskDelay(3000);
								vTaskDelay(3000);
								vTaskDelay(3000);
								PHOTOALL_EN;            /* 拍照 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  */
								vTaskDelay(100);		
								PHOTOALL_NO;							
								vTaskDelay(3000);	
                g_offOrOnTureStateFlag = SYSTEMSTATE_ONTRUE;                
                xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite);         /* 任务的句柄，发送的数据 32位，覆盖当前通知 */
                FMQ_ON;
                vTaskDelay(500);
                FMQ_OFF;
                vTaskDelay(100);
                n = 0; 
                                
            }
            else
            {
                vTaskDelay(300);
                g_offOrOnTureStateFlag = SYSTEMSTATE_OPENFAIL;     	
                xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite);         /* 任务的句柄，发送的数据 32位，覆盖当前通知 */
                FMQ_ON;
                vTaskDelay(500);
                FMQ_OFF;
                vTaskDelay(100);
                n = 0;    
            } 
     }                               
        vTaskDelay(1);
    }
}




