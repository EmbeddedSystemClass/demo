#include "temp_task.h"


/*************************************************
Function: void TEMP_TASK(void const * argument)
Description: �����¶ȼ��
Return: �޷���ֵ
*************************************************/

uint16_t   g_ds18b20Temp=0;          /*ds18b20 �¶��ݴ�*/
TaskHandle_t 	TEMP_TASK_Handle 	= NULL;		//����������


void  TEMP_TASK(void const * argument)
{

	 while(1)
	 {
			taskENTER_CRITICAL();           								    /*�����ٽ���*/
			g_ds18b20Temp=cocoguojia_ds18b20GetTemp();
			taskEXIT_CRITICAL();            								    /*�˳��ٽ���*/

		  vTaskDelay(1500);  
	 }

}


