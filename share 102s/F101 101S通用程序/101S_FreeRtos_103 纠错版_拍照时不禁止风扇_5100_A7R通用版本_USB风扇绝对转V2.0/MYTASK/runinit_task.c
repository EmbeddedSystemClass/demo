#include "runinit_task.h"

static void PowerUpAndShowLogo(void);                   /* ϵͳ���Ϻ͵���ʾLOGO */
static void ReadEepromAndShowResultAndSetDoWhit(void);  /* ��EEPROM����ʾ�ϴ����ý�����Ҵ������ò��� */
static void ShowSystemInit(void);                       /* LED��ʾϵͳ��ʼ�� */


/***********************************
say:
RUN_TASK �������������߼� ���һЩ�ؼ���״̬��־ ����ϵͳ������
��ϵͳ״̬���� ���ܸı�ϵͳ״̬��־ 
***********************************/

 
/*************************************************
Function: void  RUNINIT_TASK (void const * argument)
Description: ���ƿ�������
Return: �޷���ֵ
*************************************************/
TaskHandle_t 	RUNINIT_TASK_Handle = NULL;		//����������


void  RUNINIT_TASK (void* parameter)
{ 


    PowerUpAndShowLogo();                   /* ϵͳ���Ϻ͵���ʾLOGO */ 
       
    ReadEepromAndShowResultAndSetDoWhit();  /* ��EEPROM����ʾ�ϴ����ý�����Ҵ������ò��� */
    ShowSystemInit();                       /* OLED��ʾ ϵͳ��ʼ�� */
	
    vTaskDelay(100);
	  g_camTaskCanTakeOverFlag=1;	


    vTaskDelay(500);
    vTaskDelete(RUNINIT_TASK_Handle); 			/* ɾ��RUNINIT_TASK_Handle���� */ 
    vTaskDelay(10);

}
 

/* ϵͳ���Ϻ͵���ʾLOGO */
static void PowerUpAndShowLogo(void)
{
    BaseType_t xReturn = pdPASS;

	  uint16_t n = 0;
    
    xReturn = xReturn;	

	  g_offOrOnTureStateFlag = OLEDSHOWSTATE_LOGO;			    //��ʾLOGO                             
    xReturn = xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite);//����ľ�������͵����� 32λ�����ǵ�ǰ֪ͨ                                 							 			 							 
	  n=10;
    while(n--)//��������Ъ����2s �����˸
		{		
			FMQ_ON;
			RGBLEDR_ON;
			LEDPOINT_ON;
					KEY_LED_ON;
					vTaskDelay(100);
			FMQ_OFF;
			RGBLED_ALLOFF;
			LEDPOINT_OFF;
					KEY_LED_OFF;
			vTaskDelay(100);
		}

	FMQ_ON;/* �������������� ��ʾϵͳ�ϵ�ɹ� */
	vTaskDelay(500);
	FMQ_OFF;
	vTaskDelay(100);
	RGBLEDG_ON;/* �̵Ƴ��� */
}



uint8_t currentPageNum = 1;//��ǰ������ҳ 1=pwm ttlѡ��  2=ttl ����ѡ��  3=���ս����ʾ


/* ��EEPROM����ʾ�ϴ����ý�����Ҵ������ò��� */
static void ReadEepromAndShowResultAndSetDoWhit(void)
{
    BaseType_t xReturn = pdPASS;
    EventBits_t eventTemp = 0;
    uint8_t theLastpwmOrTtlModelFlag = 0;
    uint8_t i = 0;
    xReturn = xReturn;

 	/* ��eeprom */ 
	cocoguojia_readSetParameter();

   /* ���뵽����ģʽ page3 ��ʾ��ǰ���ý�� */
	  g_offOrOnTureStateFlag = OLEDSHOWSTATE_SETMODEL4;
    xReturn = xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite);//����ľ�������͵����� 32λ�����ǵ�ǰ֪ͨ
   	FMQ_ON;
    vTaskDelay(500);
    FMQ_OFF;				 
    vTaskDelay(200);
    vTaskDelay(1000);
    vTaskDelay(1000);
    
     /* ������� */
    eventTemp = xEventGroupWaitBits(Key_EventGroup,KEY_SHORT|KEY_LONG,pdTRUE,pdFALSE,0);//�¼��������������������Ȥ���¼����˳�ʱ����¼�λ���������Ȥ�������¼�  
    if(KEY_SHORT == (eventTemp&KEY_SHORT))/* �����̰��� */
    {
		if(0 == g_pwmOrTtlModelFlag)
		{        
			g_currentRowNum = 2;  /* �ϴ�����Ϊ PWM */
		}
		else if(1 == g_pwmOrTtlModelFlag)
		{
			g_currentRowNum = 4;  /* �ϴ�����Ϊ TTL-H */
		}
		else
		{
			g_currentRowNum = 6;  /* �ϴ�����Ϊ TTL-L */
		}	
        theLastpwmOrTtlModelFlag = g_pwmOrTtlModelFlag;		
		    g_setModelFlag = 1; /* ���뵽���ý��� page1 */
    }
   
    
	if(1 == g_setModelFlag)   /* ������뵽���ý��� */
	{

		/* ��ʾ���ý���1 */
		g_offOrOnTureStateFlag = OLEDSHOWSTATE_SETMODEL1;
		xReturn = xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite);/* ����ľ�������͵����� 32λ�����ǵ�ǰ֪ͨ */						 
		FMQ_ON;
        vTaskDelay(500);
        FMQ_OFF;
        vTaskDelay(100);
	}
	while(1 == g_setModelFlag)
	{

	    /* ���ü�� ����ʾϵͳ��ʼ��֮ǰ��һ�°�ť���� */		
        eventTemp = xEventGroupWaitBits(Key_EventGroup,KEY_SHORT|KEY_LONG,pdTRUE,pdFALSE,0);/* �¼��������������������Ȥ���¼����˳�ʱ����¼�λ���������Ȥ�������¼� */

        if(KEY_SHORT == (eventTemp&KEY_SHORT))/* �����̰��� */
        {
            if(0 == g_currentRowNum)
            {
                g_pwmOrTtlModelFlag = 0;
                g_currentRowNum = 2;
            }
            else if(2 == g_currentRowNum)
            {
                g_pwmOrTtlModelFlag = 1;
                g_currentRowNum = 4;
            }		
            else if(4 == g_currentRowNum)
            {
                g_pwmOrTtlModelFlag = 2;
                g_currentRowNum = 6;
            }
            else
            {
                g_pwmOrTtlModelFlag = theLastpwmOrTtlModelFlag;
                g_currentRowNum = 0;
            }             
        }
        else if(KEY_LONG == (eventTemp&KEY_LONG))/* ���������� */
        {	
            if(0 != g_currentRowNum)
            {

                /* ���뵽����ģʽ page3 */
                g_offOrOnTureStateFlag = OLEDSHOWSTATE_SETMODEL3;
                xReturn = xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite);/* ����ľ�������͵����� 32λ�����ǵ�ǰ֪ͨ */
                FMQ_ON;
                vTaskDelay(500);
                FMQ_OFF;
                vTaskDelay(100);

                /* дeeprom */
                cocoguojia_writeSetParameter();												 
                vTaskDelay(200);
                vTaskDelay(3000);
                g_setModelFlag = 0;
            }
            else
            {

                /* ���뵽�޸�ģʽ page1 */
                g_offOrOnTureStateFlag=OLEDSHOWSTATE_REPAIRMODEL1;
                xReturn=xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite);//����ľ�������͵����� 32λ�����ǵ�ǰ֪ͨ */
                FMQ_ON;
                vTaskDelay(500);
                FMQ_OFF;
                vTaskDelay(100);               
                POWER_EN;                     /* ����@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
                vTaskDelay(100);
                for(i = 0; i < 20; i++)
                {
                                             
                   POWER_EN;                  /* ����@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
                   vTaskDelay(500);
                   if((1==g_camPowerFlagTable[1])&&(1==g_camPowerFlagTable[2])&&(1==g_camPowerFlagTable[3])&&(1==g_camPowerFlagTable[4])&&(1==g_camPowerFlagTable[5]))
                   {
                        i = 200;
                   }
                }
                
                /* ���뵽�޸�ģʽ page2 */
                g_offOrOnTureStateFlag = OLEDSHOWSTATE_REPAIRMODEL2;
                xReturn = xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite);/* ����ľ�������͵����� 32λ�����ǵ�ǰ֪ͨ */
                FMQ_ON;
                vTaskDelay(500);
                FMQ_OFF;
                vTaskDelay(100);
                i=0;
                while(0 == i)
                {
                     eventTemp = xEventGroupWaitBits(Key_EventGroup,KEY_SHORT|KEY_LONG,pdTRUE,pdFALSE,0);//�¼��������������������Ȥ���¼����˳�ʱ����¼�λ���������Ȥ�������¼�,���ȴ�         
                     if(KEY_SHORT == (eventTemp&KEY_SHORT))/* �����̰��� */
                     {
                        OK_EN;       /* KEY ����@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
							          vTaskDelay(100);
						           	OK_NO;
											 
											 
                        FMQ_ON;
                        vTaskDelay(100);
                        FMQ_OFF;
                        vTaskDelay(100);
                     } 
                     else if(KEY_LONG == (eventTemp&KEY_LONG))/* ������ */
                     {
                           i = 1;
                     }
                     else
                     {
                       vTaskDelay(10);
                     }                         
                }                 
                

                /* ���뵽�޸�ģʽ page3 */
                g_offOrOnTureStateFlag = OLEDSHOWSTATE_REPAIRMODEL3;
                xReturn = xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite);//����ľ�������͵����� 32λ�����ǵ�ǰ֪ͨ
                FMQ_ON;
                vTaskDelay(500);
                FMQ_OFF;
                vTaskDelay(100);
								POWER_NO;                     /* �ػ�@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
                vTaskDelay(1000);
                for(i = 0; i < 20; i++)
                {
																							
								   POWER_NO;                  /* �ػ�@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
                   vTaskDelay(500);
                   if((0==g_camPowerFlagTable[1])&&(0==g_camPowerFlagTable[2])&&(0==g_camPowerFlagTable[3])&&(0==g_camPowerFlagTable[4])&&(0==g_camPowerFlagTable[5]))
                   {
                        i = 200;
                   }
                }
                vTaskDelay(2000);
    
                /* ���뵽�޸�ģʽ page4 */
                g_offOrOnTureStateFlag = OLEDSHOWSTATE_REPAIRMODEL4;
                xReturn = xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite);/* ����ľ�������͵����� 32λ�����ǵ�ǰ֪ͨ */
                FMQ_ON;
                vTaskDelay(500);
                FMQ_OFF;
                vTaskDelay(100);
                while(1)
                {
                    vTaskDelay(100);
                }               
            }  
        }
        else 
        {
            ;
        }    
		vTaskDelay(10);
	}
	
	if(0==g_pwmOrTtlModelFlag)
	{
		cocoguojia_tim5CatchInit();
	}
	else
	{
		cocoguojia_ttlTestInit();
	}	
}



/* OLED��ʾϵͳ��ʼ�� */
static void ShowSystemInit(void)
{

	/* ��ʾ ϵͳ��ʼ�� */
	g_offOrOnTureStateFlag = OLEDSHOWSTATE_INIT;
    xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite);/* ����ľ�������͵����� 32λ�����ǵ�ǰ֪ͨ */
    FMQ_ON;
    vTaskDelay(500);
    FMQ_OFF;
    vTaskDelay(100);    
								 
}

