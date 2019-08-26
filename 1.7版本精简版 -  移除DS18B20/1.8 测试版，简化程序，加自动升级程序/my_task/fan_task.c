#include "fan_task.h"
#include "psdk_debug.h"  

uint8_t g_fanDs18b20Flag = 0;         /*DS18B20逻辑的  0=不转 1=转*/


void  FAN_TASK (void const * argument)
{
  osDelay(10);	
   
	while (1) 
	{
         if(READ_ENING)
         {
              SWITCH_READ;                              
              RGBLEDB_ON;           /*蓝色灯单独常亮*/              
              g_readingFlag = 1;
         }
         else
         {
             LEDB_OFF;              /*蓝色灯灭*/
             SWITCH_WRITE;
             g_readingFlag = 0;
         }         
         
				if(1 == g_readingFlag)
        {
            FAN_ON;
        }
        else
        {
						if(1 == g_fanDs18b20Flag)
						{
								FAN_ON;
						}
						else
						{
								FAN_OFF;
						}    
		    }             
     osDelay(1);            
    }		
}


