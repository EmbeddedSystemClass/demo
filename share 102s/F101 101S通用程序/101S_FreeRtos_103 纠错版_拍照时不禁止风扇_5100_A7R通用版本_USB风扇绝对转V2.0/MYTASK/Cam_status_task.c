#include "Cam_status_task.h"
 
 //cocoguojia say:
 //RUN_TASK �������������߼� ���һЩ�ؼ���״̬��־ ����ϵͳ������
 //��ϵͳ״̬���� ���ܸı�ϵͳ״̬��־

 //---------------------------------------------------------------------------
 //����ȫ�ֱ���
TaskHandle_t 	Cam_status_update_TASK_Handle	= NULL;		//����������
 


uint8_t 	  g_triggerFlag=0;							  //1=PWM���������� Ϊ1ʱҪ��ʾ��һҳ(���ڵ�2ҳ��ʱ��Ҫ�л��ɵ�һҳ ���ڵ�һҳ��ʱ�� ����һҳ��ʾ������ʱ��)  ��OLED��������
uint32_t 	  g_oledStateFlag=0;							  //0��ʾ������
uint16_t    g_tim5PlauseUs=0;							  //��⵽��PWM������
uint16_t    g_tim5TriggerPlauseUs=0;				      //����ʱ ��⵽��PWM������
uint16_t    g_tim5CatchNum=0;							  //�����ұ���������Ч�ܴ���
uint8_t 	  g_capture_FinishAndStartPhotoFlag=0;		  //��׽��������Ҫ�����������̱�־ 1=��ʼ��������
uint8_t  	  g_camPowerFlagTable[6]={0,0,0,0,0,0};         //5������Ŀ��ػ���־
uint16_t    g_photoPicNum[6]={0,0,0,0,0,0};         	  //5������ֱ����Ƭ��
uint8_t    	g_readingFlag=0;							  //1=���ڶ���  0=��������״̬
uint8_t  	  g_lastReadingFlag=0;						  //�ϴ�״̬ 1=���ڶ���  0=��������״̬
uint8_t     g_setModelFlag=0;							  //1=���뵽����ģʽ
uint8_t     g_pwmOrTtlModelFlag=0;				          //0=pwm  1=ttl-H   2=ttl-L
uint16_t    g_ttlCurrentNum=0;							  //��ǰ��������
uint16_t    g_ttlTriggerNum=0;							  //�����ǵĴ�������
uint8_t     g_currentRowNum=2;						      //��ǰ�� Num ��ǰ�� OLED�ĵ�ǰ�� ��Χ=2 4 6
uint8_t 	  g_camTaskCanTakeOverFlag=0;			          //1=cam������Խӹ�
uint32_t 	  g_offOrOnTureStateFlag=SYSTEMSTATE_GOTOSHUT;
uint8_t  	  g_camLoseFlagTable[6]={0,0,0,0,0,0};          //5������Ĵ��ڱ�־ ������ʧ �ϵ�����һ��
uint8_t     g_theTurelostNumTable[6]={0,0,0,0,0,0};       //ʵ�ʶ�Ƭ��
uint8_t     g_camHotShotTable[6]={0,0,0,0,0,0}; 

//-------------------------------------------------------------------------
//������
void  Cam_status_update_TASK (void* parameter)
{ 

	while (1) 
	{ 
		vTaskDelay(100);
		 /* �ж�����Ŀ��ػ�״̬ */
		  if( GPIO_ReadInputDataBit(GPIOA,  GPIO_Pin_1))
		   {
			   g_camPowerFlagTable[1] = 1;
			}
			else
			{
			   g_camPowerFlagTable[1] = 0;
			}
			
		    if( GPIO_ReadInputDataBit(GPIOA,  GPIO_Pin_1))
			{
			   g_camPowerFlagTable[2] = 1;
			}
			else
			{
			  g_camPowerFlagTable[2] = 0;
			}
			
			if( GPIO_ReadInputDataBit(GPIOA,  GPIO_Pin_1))
			{
			  g_camPowerFlagTable[3] = 1;
			}
			else
			{
			  g_camPowerFlagTable[3] = 0;
			}
			
			if( GPIO_ReadInputDataBit(GPIOA,  GPIO_Pin_1))
			{
			  g_camPowerFlagTable[4] = 1;
			}
			else
			{
			  g_camPowerFlagTable[4] = 0;
			}
			
			if( GPIO_ReadInputDataBit(GPIOA,  GPIO_Pin_1))
			{
			  g_camPowerFlagTable[5] = 1;
			}
			else
			{
			  g_camPowerFlagTable[5] = 0;
			}
	}
}







