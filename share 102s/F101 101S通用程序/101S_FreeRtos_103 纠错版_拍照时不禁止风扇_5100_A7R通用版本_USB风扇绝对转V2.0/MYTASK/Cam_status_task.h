
#ifndef __U2RECE_TASK_H__
#define __U2RECE_TASK_H__

#include "myinclude.h"

//#define USART1PUTOUTDEBUG  		1

extern TaskHandle_t 	Cam_status_update_TASK_Handle;

extern void Cam_status_update_TASK (void* parameter);			



//------------------------------------------------
//OLED��ʾ״̬Ԥ����
#define OLEDSHOWSTATE_RUN   6			
#define OLEDSHOWSTATE_INIT 	7		
#define OLEDSHOWSTATE_LOGO	8		

#define OLEDSHOWSTATE_SETMODEL1 9
#define OLEDSHOWSTATE_SETMODEL2	10	
#define OLEDSHOWSTATE_SETMODEL3	11	
#define OLEDSHOWSTATE_SETMODEL4	12	

#define OLEDSHOWSTATE_REPAIRMODEL1 13
#define OLEDSHOWSTATE_REPAIRMODEL2 14
#define OLEDSHOWSTATE_REPAIRMODEL3 15
#define OLEDSHOWSTATE_REPAIRMODEL4 16

//------------------------------------------------
//ϵͳ״̬Ԥ����
//0=��ʵ�ػ�  1=��ʵ����  2=�ػ������л���ȷ���Ƿ���ʵ�ػ� 3=���������л���ȷ���Ƿ���ʵ���� 4=�ػ�ʧ��  5=����ʧ��
#define SYSTEMSTATE_OFFTRUE 		     0
#define SYSTEMSTATE_ONTRUE 				 1
#define SYSTEMSTATE_GOTOSHUT 			 2
#define SYSTEMSTATE_GOTOOPEN			 3
#define SYSTEMSTATE_SHUTFAIL 			 4
#define SYSTEMSTATE_OPENFAIL 			 5
#define SYSTEMSTATE_CAMLOSE 			 6


#define TEMPEREATURE_MAX 30




#define TTLEDGELONG 			         5

#define     MODELFLAG_PWM    0
#define     MODELFLAG_TTL_H  1
#define     MODELFLAG_TTL_L  2


//------------------------------------------------
//USART2 �������� Ԥ����
#define USART2COM_OPEN 		     1
#define USART2COM_SHUT 		     2
#define USART2COM_PHOTO 		 3
#define USART2COM_ASK 		     4
#define USART2COM_KEY 		     5




extern uint8_t 	g_camTaskCanTakeOverFlag;				//1=cam������Խӹ�
extern uint8_t 	g_triggerFlag;							//1=PWM���������� Ϊ1ʱҪ��ʾ��һҳ
extern uint32_t  g_oledStateFlag;							//0��ʾ������
extern uint32_t  g_offOrOnTureStateFlag;					//0=off�ػ�  1=on����  2=�ػ������л���ȷ���Ƿ���ʵ�ػ� 3=���������л���ȷ���Ƿ���ʵ���� ״̬��־
extern uint16_t  g_tim5TriggerPlauseUs;						//����ʱ ��⵽��PWM������
extern uint16_t  g_tim5PlauseUs;							//��⵽��PWM������
extern uint16_t  g_tim5CatchNum;							//�����ұ���������Ч�ܴ���
extern uint16_t  g_photoPicNum[6];         					//5������ֱ����Ƭ��
extern uint8_t   g_camPowerFlagTable[6];        			//5������Ŀ��ػ���־
extern uint8_t 	 g_capture_FinishAndStartPhotoFlag;			//��׽��������Ҫ�����������̱�־ 1=��ʼ��������

extern uint8_t   g_readingFlag;								//1=���ڶ���  0=��������״̬
extern uint8_t   g_lastReadingFlag;							//�ϴ�״̬ 1=���ڶ���  0=��������״̬


extern uint8_t   g_setModelFlag;							//1=���뵽����ģʽ
extern uint8_t   g_pwmOrTtlModelFlag;						//0=pwm   1=ttl

extern uint16_t  g_ttlCurrentNum;							//��ǰ��������
extern uint16_t  g_ttlTriggerNum;							//�����ǵĴ�������

extern uint8_t   g_currentRowNum;							//��ǰ�� Num



extern uint8_t   g_camLoseFlagTable[6];                          //5������Ķ�ʧ��־ ��1=��ʧ �ϵ�����һ��

extern uint16_t  g_ds18b20Temp;                                  //ds18b20 �¶��ݴ�


extern uint8_t     g_temperatureDelayLastFlag;  //�¶��ӳٱ�־ 0=�ϴ��Ǵ�С����ʹ�ܷ���  1=�ϴ��ǴӴ�С�رշ���  2=���ȹ�����ʱ�ر�

extern uint8_t     g_camHotShotTable[6];



extern uint8_t     g_theTurelostNumTable[6];     //ʵ�ʶ�Ƭ��

//--------------------------------
//����1�̰� ���� �¼�λ
#define KEY_SHORT (0x00000001<<0)
#define KEY_LONG  (0x00000001<<1)

#endif

