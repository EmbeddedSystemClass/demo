#include "key_task.h"
#include "psdk_debug.h"
 

/*************************************************
Function: void  KEY_TASK (void const * argument)
Description: 判断按键是短按还是长按
Return: 无返回值
*************************************************/
void  KEY_TASK (void const * argument)
{
	  uint8_t n=0;
    uint32_t   xLastWakeTime;
    xLastWakeTime=osKernelSysTick(); /* 初始化xLastWakeTime为当前滴答数注意第一次使用的时候 要获取当前时间 */
	while (1) 
	{	
	//	PSDK_LOG_DEBUG("KEY_TASK running  \r\n" );		
    osDelayUntil(&xLastWakeTime,10);        
		if(0==setModelFlag)
		{
			if(KEY_ENING)
      {
					osDelay(5);	
					if(KEY_ENING)
					{
							osDelay(15);	
							if(KEY_ENING)
							{
								 xEventGroupSetBits(Key_EventGroup,KEY_SHORT);
							} 
							while(KEY_ENING)
							{
									while(KEY_ENING)
									{
										osDelay(10);	
									}
									osDelay(60);	
							}
					} 
      }    
		}
		else
		{
			if(KEY_ENING)
			{
				osDelay(5);	
				if(KEY_ENING)
				{
						osDelay(15);	
						if(KEY_ENING)
						{              
								n=0;
								while(KEY_ENING)
								{
										osDelay(10);	
										n++;
										if(100<=n)
										{
											n=210;
											break;
										}
								}
								
								while(KEY_ENING)
								{
										while(KEY_ENING)
										{
											 osDelay(10);	
										}
										osDelay(60);	
								}
														
								//--------------------------------------------------------------
								//放到后面 这样长按时 以松手后出现反应 这样人性化
								if(200<=n)
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
	 }				
}


