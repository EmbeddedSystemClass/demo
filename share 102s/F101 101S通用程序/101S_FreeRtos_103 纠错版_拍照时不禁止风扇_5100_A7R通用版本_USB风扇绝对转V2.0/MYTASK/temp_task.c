#include "temp_task.h"


/*************************************************
Function: void TEMP_TASK(void const * argument)
Description: 进行温度检测
Return: 无返回值
*************************************************/

uint16_t   g_ds18b20Temp=0;          /*ds18b20 温度暂存*/
TaskHandle_t 	TEMP_TASK_Handle 	= NULL;		//创建任务句柄


void  TEMP_TASK(void const * argument)
{

	 while(1)
	 {
			taskENTER_CRITICAL();           								    /*进入临界区*/
			g_ds18b20Temp=cocoguojia_ds18b20GetTemp();
			taskEXIT_CRITICAL();            								    /*退出临界区*/

		  vTaskDelay(1500);  
	 }

}


