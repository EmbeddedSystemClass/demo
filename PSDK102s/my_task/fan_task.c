#include "fan_task.h"
#include "psdk_debug.h"  


/*************************************************
Function: void  FAN_TASK (void const * argument)
Description: 控制风扇什么时候转动，及读写数据切换
Return: 无返回值
*************************************************/
uint8_t g_fanDs18b20Flag = 0;         /*DS18B20逻辑的  0=不转 1=转*/
void  FAN_TASK (void const * argument)
{
  osDelay(10);	
   
	while (1) 
	{
	  	PSDK_LOG_DEBUG("FAN_TASK running  \r\n" );	
         if(READ_ENING)
         {
              SWITCH_READ;                              
              RGBLEDB_ON;           /*蓝色灯单独常亮*/              
              readingFlag = 1;
         }
         else
         {
             LEDB_OFF;              /*蓝色灯灭*/
             SWITCH_WRITE;
             readingFlag = 0;
         }         
         
				if(1 == readingFlag)
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


