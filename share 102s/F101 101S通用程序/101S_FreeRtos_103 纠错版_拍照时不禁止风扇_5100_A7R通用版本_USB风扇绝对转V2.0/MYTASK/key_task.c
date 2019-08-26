#include "key_task.h"
 /*************************************************
Function: void  KEY_TASK (void const * argument)
Description: �жϰ����Ƕ̰����ǳ���
Return: �޷���ֵ
*************************************************/

TaskHandle_t 	KEY_TASK_Handle = NULL;		/* ���������� */
void  KEY_TASK (void* parameter)
{
    portTickType xLastWakeTime;
	  uint8_t n = 0;
    
    xLastWakeTime=xTaskGetTickCount();/* ʹ�õ�ǰʱ�� ��ʼ��xLastWakeTime���� */
	
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
							//�ŵ����� ��������ʱ �����ֺ���ַ�Ӧ �������Ի�
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


