#include "temp_task.h"
#include "psdk_debug.h"

uint16_t   g_ds18b20Temp=0;          /*ds18b20 �¶��ݴ�*/
 
 
void  TEMP_TASK(void const * argument)
{
	 while(1)
	 {		 
			taskENTER_CRITICAL();           								    /*�����ٽ���*/
			g_ds18b20Temp=cocoguojia_ds18b20GetTemp();
			taskEXIT_CRITICAL();            								    /*�˳��ٽ���*/
		 osDelay(2000);  
	 }
 
}


