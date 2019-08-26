#include "key_task.h"
 /*************************************************
Function: void  KEY_TASK (void const * argument)
Description: 判断按键是短按还是长按
Return: 无返回值
*************************************************/

TaskHandle_t 	KEY_TASK_Handle = NULL;		/* 创建任务句柄 */
void  KEY_TASK (void* parameter)
{
    portTickType xLastWakeTime;
	  uint8_t n = 0;
    
    xLastWakeTime=xTaskGetTickCount();/* 使用当前时间 初始化xLastWakeTime变量 */
	
	while (1) 
	{	
		if(0 == g_setModelFlag)
		{
			if(KEY_ENING)
			{
				vTaskDelayUntil(&xLastWakeTime,5);	
				if(KEY_ENING)
				{
					vTaskDelayUntil(&xLastWakeTime,15);	
					if(KEY_ENING)
					{
							xEventGroupSetBits(Key_EventGroup,KEY_SHORT);
					} 
					
					while(KEY_ENING)
					{
						while(KEY_ENING)
						{
								vTaskDelayUntil(&xLastWakeTime,10);	
						}
						
						vTaskDelayUntil(&xLastWakeTime,60);	
					}
				} 
      }    
		}
		else
		{
			if(KEY_ENING)
			{
				vTaskDelayUntil(&xLastWakeTime,5);	
				if(KEY_ENING)
				{
					vTaskDelayUntil(&xLastWakeTime,15);	
					if(KEY_ENING)
					{              
							n = 0;
							while(KEY_ENING)
							{
								vTaskDelayUntil(&xLastWakeTime,10);	
								n++;
								
								if(100 <= n)
								{
									n = 210;
									break;
								}
							}
	
							while(KEY_ENING)
							{
								while(KEY_ENING)
								{
									 vTaskDelayUntil(&xLastWakeTime,10);	
								}
								vTaskDelayUntil(&xLastWakeTime,60);	
							}
													
							//--------------------------------------------------------------
							//放到后面 这样长按时 以松手后出现反应 这样人性化
							if(200 <= n)
							{
								xEventGroupSetBits(Key_EventGroup,KEY_LONG);
							}
							else
							{
								xEventGroupSetBits(Key_EventGroup,KEY_SHORT);
							}	
					}
				}
			}	
		}
        vTaskDelayUntil(&xLastWakeTime,10);        
	 }				
}


