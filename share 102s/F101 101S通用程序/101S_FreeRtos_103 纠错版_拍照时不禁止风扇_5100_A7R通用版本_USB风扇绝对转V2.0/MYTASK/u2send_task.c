 
 
 
#include "u2send_task.h"
  

static void comFor5CamOn(void);
static void comFor5CamOff(void);
static void comFor5CamPhoto(void);
static void askFor5Cam(void);
static void comForKey(void);

TaskHandle_t 	U2SEND_TASK_Handle=NULL;

//-------------------------------------------------------------------------
//������
void  U2SEND_TASK (void* parameter)
{
    BaseType_t xReturn=pdFAIL;
	uint8_t p_msg;
    uint8_t n=0;

	while (1) 
	{	     
        xReturn=xQueueReceive(usart2Send_Queue,&p_msg,portMAX_DELAY);
         
		if(pdTRUE==xReturn)
		{
			n=p_msg;
            switch(n)
            {
                case USART2COM_OPEN:
                comFor5CamOn();
                break;  

                case USART2COM_SHUT:
                comFor5CamOff();
                break;

                case USART2COM_PHOTO:
                comFor5CamPhoto();
                break;

                case USART2COM_ASK:
                askFor5Cam();
                break;

                case USART2COM_KEY:
                comForKey();
                break;

                default:
                break;
            }                
		}
    }
}
    
//------------------------------------------
//���� ����23ms+8��ʱ
static void comFor5CamOn(void)
{
    portTickType xLastWakeTime;
    xLastWakeTime=xTaskGetTickCount();//ʹ�õ�ǰʱ�� ��ʼ��xLastWakeTime����
	
    RS485_TXD_EN;
    vTaskDelayUntil(&xLastWakeTime,3);
    cocoguojia_usartNPutStr(2,"@@@");//ǰ��@
    cocoguojia_usartNPutStr(2,"@X1#");//���͹ػ�����
    vTaskDelayUntil(&xLastWakeTime,20);
}

//------------------------------------------
//�ػ� ����23ms+8��ʱ
static void comFor5CamOff(void)
{
    portTickType xLastWakeTime;
    xLastWakeTime=xTaskGetTickCount();//ʹ�õ�ǰʱ�� ��ʼ��xLastWakeTime����
    
    RS485_TXD_EN;
    vTaskDelayUntil(&xLastWakeTime,3);
    cocoguojia_usartNPutStr(2,"@@@");//ǰ��@
    cocoguojia_usartNPutStr(2,"@X2#");//���͹ػ�����
    vTaskDelayUntil(&xLastWakeTime,20);
}

//------------------------------------------
//���� ����23ms+8��ʱ
static void comFor5CamPhoto(void)
{
    portTickType xLastWakeTime;
    xLastWakeTime=xTaskGetTickCount();//ʹ�õ�ǰʱ�� ��ʼ��xLastWakeTime����
    
    RS485_TXD_EN;
    vTaskDelayUntil(&xLastWakeTime,3);
    cocoguojia_usartNPutStr(2,"@@@");//ǰ��@
    cocoguojia_usartNPutStr(2,"@X3#");//���͹ػ�����
    vTaskDelayUntil(&xLastWakeTime,20);
    
}

extern uint8_t  g_usart2forSendRunFlag;     //����2�ܷ������ڽ��б�־

//------------------------------------------
//��ѯ״̬ ����180ms+20ms��ʱ
static void askFor5Cam(void)
{
    portTickType xLastWakeTime;
    xLastWakeTime=xTaskGetTickCount();//ʹ�õ�ǰʱ�� ��ʼ��xLastWakeTime����
    
    RS485_TXD_EN;
    vTaskDelayUntil(&xLastWakeTime,3);
    cocoguojia_usartNPutStr(2,"@14#");//���Ͳ�ѯ����   
    while(1==g_usart2forSendRunFlag){vTaskDelayUntil(&xLastWakeTime,1);}//��ʹ��RS485����֮ǰ Ҫ�ж�һ���Ƿ�����ȫ��־ 
    vTaskDelayUntil(&xLastWakeTime,2);
    RS485_RXD_EN;
    vTaskDelayUntil(&xLastWakeTime,30);
    
    RS485_TXD_EN;
    vTaskDelayUntil(&xLastWakeTime,3);
    cocoguojia_usartNPutStr(2,"@24#");//���Ͳ�ѯ����
    while(1==g_usart2forSendRunFlag){vTaskDelayUntil(&xLastWakeTime,1);}//��ʹ��RS485����֮ǰ Ҫ�ж�һ���Ƿ�����ȫ��־
    vTaskDelayUntil(&xLastWakeTime,2);
    RS485_RXD_EN;
    vTaskDelayUntil(&xLastWakeTime,30);
    
    RS485_TXD_EN;
    vTaskDelayUntil(&xLastWakeTime,3);
    cocoguojia_usartNPutStr(2,"@34#");//���Ͳ�ѯ����
    while(1==g_usart2forSendRunFlag){vTaskDelayUntil(&xLastWakeTime,1);}//��ʹ��RS485����֮ǰ Ҫ�ж�һ���Ƿ�����ȫ��־
    vTaskDelayUntil(&xLastWakeTime,2);
    RS485_RXD_EN;
    vTaskDelayUntil(&xLastWakeTime,30);
   
    RS485_TXD_EN;
    vTaskDelayUntil(&xLastWakeTime,3);
    cocoguojia_usartNPutStr(2,"@44#");//���Ͳ�ѯ����
    while(1==g_usart2forSendRunFlag){vTaskDelayUntil(&xLastWakeTime,1);}//��ʹ��RS485����֮ǰ Ҫ�ж�һ���Ƿ�����ȫ��־
    vTaskDelayUntil(&xLastWakeTime,2);
    RS485_RXD_EN;
    vTaskDelayUntil(&xLastWakeTime,30);
    
    RS485_TXD_EN;
    vTaskDelayUntil(&xLastWakeTime,3);
    cocoguojia_usartNPutStr(2,"@54#");//���Ͳ�ѯ����
    while(1==g_usart2forSendRunFlag){vTaskDelayUntil(&xLastWakeTime,1);}//��ʹ��RS485����֮ǰ Ҫ�ж�һ���Ƿ�����ȫ��־
    vTaskDelayUntil(&xLastWakeTime,2);
    RS485_RXD_EN;
    vTaskDelayUntil(&xLastWakeTime,30);
    
    RS485_TXD_EN;   
}


//------------------------------------------
//OK���� ����23ms+5��ʱ
static void comForKey(void)
{
    portTickType xLastWakeTime;
    xLastWakeTime=xTaskGetTickCount();//ʹ�õ�ǰʱ�� ��ʼ��xLastWakeTime����
    
    RS485_TXD_EN;
    vTaskDelayUntil(&xLastWakeTime,3);
    cocoguojia_usartNPutStr(2,"@@@");//ǰ��@
    cocoguojia_usartNPutStr(2,"@X5#");//���͹ػ�����
    vTaskDelayUntil(&xLastWakeTime,20);
}








