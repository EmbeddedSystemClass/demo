 
#include "psdk_rec.h"

////////////////////////////////////////////////////////////////////////////////////////
//ȫ�ֱ���
TaskHandle_t 	PsdkRec_Task_Handle 	= NULL;     //PsdkRec_Task_Handle������
SemaphoreHandle_t uart2Mutex_Handle=NULL;           //���崮��2�Ľ������ݻ������� �����ź���
xQueueHandle gpsQueue_Handle=NULL;                  //���崫��GPS��Ϣ�Ķ��о��



//----------------------------------------------------
//PsdkRec����ʵ��
void PsdkRec_Task(void const *parameter)
{
   uint8_t res = 0;
   BaseType_t xReturn = pdPASS;    //����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS 
    
    while (1) 
	{	
        //-----------------------------------------------------------------------------------    
        //xReturn = xSemaphoreTake(uart2Mutex_Handle, 0);                           //��ȡ������ �ڲ���������ʱ �߻�ȡ������uart2Mutex_Handle ���û����һֱ�ȴ�
        res=Bsp_usart2ReadChar(&Usart2RingRecBufDoStruct,&PsdkProtFrameAllStruct);  //����2�������ݲ�������ȷ�ظ�
        //xReturn = xSemaphoreGive( uart2Mutex_Handle );                            //�ͷŻ�����
        //-----------------------------------------------------------------------------------
        res=res;
        xReturn=xReturn;
        //��ʱ1��tick(1);
        vTaskDelay(1);   
    }
}





