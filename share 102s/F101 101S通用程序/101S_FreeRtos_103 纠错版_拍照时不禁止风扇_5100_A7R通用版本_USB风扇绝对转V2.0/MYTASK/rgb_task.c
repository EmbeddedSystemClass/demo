#include "rgb_task.h"
 
TaskHandle_t 	RGB_TASK_Handle 	= NULL;		//����������


//-------------------------------------------------------------------------
//������
void  RGB_TASK ( void * p_arg )
{
    uint8_t ledrflag=0;//1=�ո����ڱ���
    uint16_t k=0;
    uint8_t i=0;
    uint8_t num=0;
	while (1) 
	{ 

               
		if(SYSTEMSTATE_OFFTRUE==g_offOrOnTureStateFlag)
		{
			KEY_LED_OFF;
		}
		else if(SYSTEMSTATE_ONTRUE==g_offOrOnTureStateFlag)
		{
			KEY_LED_ON;
		}
		else
		{
			;
		}	
      
        if(1==g_camTaskCanTakeOverFlag)
        {        
            if(SYSTEMSTATE_ONTRUE==g_offOrOnTureStateFlag)  //��ʵ����
            {
                KEY_LED_ON;
                if(READ_ENING)
                {
                    k=0;
                    if((0==g_readingFlag)||(1==ledrflag))
                    {
                        RGBLED_ALLOFF;//��
                    }
                    RGBLEDB_ON;       //��ɫ�Ƴ���
                    FAN_ON;           //USB���� ���Ⱦ���ת
                    g_readingFlag=1;
                    ledrflag=0;
                }
                else
                { 
                    num=0;
                    for(i=1;i<=5;i++)
                    {
                        num+=g_theTurelostNumTable[i];   //ͳ�ƶ�Ƭ����
                    }
                    if((1==g_readingFlag)||(1==ledrflag))
                    {              
                        RGBLED_ALLOFF;//��            
                    }
                    g_readingFlag=0;
                    ledrflag=0;
                    k++;
                    //----------------------------------------------------------------------------
                    //����ʵ��Ƭ��ʱ�� 1s��˸һ�� ÿ��������ٶ���9�ŵ�ʱ�� ���� ��˸һ��100ms
                    if((50-num)==k)
                    {
                        RGBLEDG_ON;//�̵���˸
                    }
                    else  if(100-num-num<=k)
                    {
                        RGBLED_ALLOFF;//��
                        k=0;
                    }                            
                }  
            }
            else
            {
                KEY_LED_OFF;                
                if(READ_ENING)
                {
                    if((0==g_readingFlag)||(1==ledrflag))
                    {
                        RGBLED_ALLOFF;//��
                    }
                    k=0;
                    RGBLEDB_ON;     //��ɫ�Ƴ���
                    FAN_ON;         //USB���� ���Ⱦ���ת
                    g_readingFlag=1;
                    ledrflag=0;
                }
                else
                { 
                    if(1==g_readingFlag)
                    {
                        RGBLED_ALLOFF;//��                                        
                    }  
                    
                    if((SYSTEMSTATE_CAMLOSE==g_offOrOnTureStateFlag)||(SYSTEMSTATE_SHUTFAIL==g_offOrOnTureStateFlag)||(SYSTEMSTATE_OPENFAIL==g_offOrOnTureStateFlag))
                    {
                        ledrflag=1;
                        k++;
                        //----------------------------------------------------------------------------
                        //����ʵ��Ƭ��ʱ�� 1s��˸һ�� ÿ��������ٶ���9�ŵ�ʱ�� ���� ��˸һ��100ms
                        if(50==k)
                        {
                            RGBLEDR_ON;//�����˸
                        }
                        else if(100<=k)
                        {
                            RGBLED_ALLOFF;//��
                            k=0;
                        }         
                    }
                    else
                    {
                        if(1==ledrflag)
                        {
                            RGBLED_ALLOFF;//��
                        }
                        ledrflag=0; 
                        k=0;
                        RGBLEDG_ON;//�̵Ƴ���
                    }                 
                    g_readingFlag=0;                                       
                }
            } 
        } 
        else
        {
            ;
        }
        
		vTaskDelay(10);
	}	
}


