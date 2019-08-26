#include "temp_task.h"
#include "psdk_debug.h"


/*************************************************
Function: void TEMP_TASK(void const * argument)
Description: 进行温度检测
Return: 无返回值
*************************************************/
uint16_t   g_ds18b20Temp=0;          /*ds18b20 温度暂存*/

void  TEMP_TASK(void const * argument)
{

	 while(1)
	 {
			taskENTER_CRITICAL();           								    /*进入临界区*/
			g_ds18b20Temp=cocoguojia_ds18b20GetTemp();
			taskEXIT_CRITICAL();            								    /*退出临界区*/
		// 	PSDK_LOG_DEBUG("TEMP_TASK running  \r\n" );
		 osDelay(1500);  
	 }

}


