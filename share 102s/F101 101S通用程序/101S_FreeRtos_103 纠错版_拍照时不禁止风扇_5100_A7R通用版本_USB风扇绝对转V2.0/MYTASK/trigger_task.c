#include "trigger_task.h" 
 
TaskHandle_t 	TRIGGER_TASK_Handle 	= NULL;		//����������



//-------------------------------------------------------------------------
//������
void  TRIGGER_TASK ( void * p_arg )
{
		EventBits_t eventTemp=0;
		uint8_t openWrongNum=0;//��������ʧ�ܼ��� �������ڵ���3�򿪻�ʧ��
		uint8_t i=0;
	  uint8_t newT=0;//Ϊ0��ʱ�� ����
	  uint8_t ttlStartFlag=0;//0=�տ�ʼҪ����ʼ 1=�������Ч�׶� 2=�ȴ�����Ҫ����β

 
    vTaskDelay(1000);
    //#################################
    taskENTER_CRITICAL();           								    //�����ٽ���
    g_ds18b20Temp= cocoguojia_ds18b20GetTemp();
    taskEXIT_CRITICAL();            								    //�˳��ٽ���
    //################################## 
    
    while (0==g_camTaskCanTakeOverFlag) 
    {
        vTaskDelay(10);
    }
    vTaskDelay(100);
    
	while (1) 
	{  	
		if(0==g_pwmOrTtlModelFlag)			//PWMģʽ 
		{
			;
		}
		else if(1==g_pwmOrTtlModelFlag)	//TTL-Hģʽ
		{
			if(0==ttlStartFlag)           //0=�տ�ʼҪ����ʼ 
			{
					if(READTTL_IS0)             //��ƽΪ0          
					{
						if(1==newT)               //֮ǰ����Ϊ��
						{
							g_ttlCurrentNum=0;      //���»���
							newT=0;                 //֮ǰΪ0�ı�־Ҳ����
						}		
						g_ttlCurrentNum++;        //��ǰ��������
					}
					else                        //��ƽΪ1 
					{
							newT=1;
							g_ttlCurrentNum++;
							if(9999<=g_ttlCurrentNum)
							{
								g_ttlCurrentNum=9999;
							}
					}	
					if(10==g_ttlCurrentNum)     //�ӵ�10 ��ʾ�͵�ƽ��10ms ��־�͵�ƽ�׶εõ��Ͽ� ����
					{
						ttlStartFlag=1;         //1=�������Ч�׶�
					}
			}
			else if(1==ttlStartFlag)      //1=�������Ч�׶�
			{	
					g_ttlCurrentNum++;
					if(READTTL_IS1)           //��ƽΪ1 
					{
						 if(g_ttlCurrentNum>=TTLEDGELONG)
						 {
								 ttlStartFlag=2;
						 }  
					}
					else
					{
						g_ttlCurrentNum=0;
					}	
			}
			else                          //2=�ȴ�����Ҫ����β
			{
					g_ttlCurrentNum++;
					if(9999<=g_ttlCurrentNum)
					{
						g_ttlCurrentNum=9999;
					}
									
					if(READTTL_IS0)    //��ƽΪ0
					{           
						if(g_ttlCurrentNum<=(2020))  //�ڱ���ģʽ�� ֻҪ����С��2s ��������Ч����
						{
								g_ttlTriggerNum=g_ttlCurrentNum;                                       
								g_triggerFlag=1;							
								g_capture_FinishAndStartPhotoFlag=1;//����
						}	
						else
						{
								;//������
						}                  
						g_ttlCurrentNum=0;
						ttlStartFlag=0;						
					}
					else
					{
						;
					}	
			}	
		}
		else 								//TTL-Lģʽ
		{
			if(0==ttlStartFlag)
			{
				if(READTTL_IS1)
				{
					if(1==newT)
					{
						g_ttlCurrentNum=0;
						newT=0;
					}		
					g_ttlCurrentNum++;
				}
				else
				{
					newT=1;
					g_ttlCurrentNum++;
					if(9999<=g_ttlCurrentNum)
					{
						g_ttlCurrentNum=9999;
					}
				}	
				if(10==g_ttlCurrentNum)
				{
					ttlStartFlag=1;
				}
			}
			else if(1==ttlStartFlag)
			{	
				g_ttlCurrentNum++;
				if(READTTL_IS0)
				{
                   if(g_ttlCurrentNum>=TTLEDGELONG)
                   {
                      ttlStartFlag=2;
                       	
                   }    
				}
				else
				{
					g_ttlCurrentNum=0;
				}	
			}
			else
			{
				g_ttlCurrentNum++;
				if(9999<=g_ttlCurrentNum)
				{
					g_ttlCurrentNum=9999;
				}
				if(READTTL_IS1)
				{                 
                    if(g_ttlCurrentNum<=2020)
                    {
                        g_ttlTriggerNum=g_ttlCurrentNum;    
                        g_triggerFlag=1;
                        g_capture_FinishAndStartPhotoFlag=1;//����
                    }	
                    else
                    {
                        ;//������
                    }    
					g_ttlCurrentNum=0;
					ttlStartFlag=0;						
				}
				else
				{
					;
				}	
			}	
		}
    
       if(SYSTEMSTATE_ONTRUE==g_offOrOnTureStateFlag)		//�������ǿ���״̬ ��������һ���޶�һ��(�ж���Ҳ���޶�)
       {
            if(1==g_capture_FinishAndStartPhotoFlag)        //���ձ�־��λ
            {					
                cocoguojia_tim5CatchDisable();				//������������ ��׽�ж���ʱ��ֹ��  �������ʵʱ��
                g_tim5CatchNum++;
                                                                 
                FMQ_ON; 
                g_triggerFlag=1;
                LEDPOINT_ON;
                                       
				PHOTOALL_EN;              //����@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@              
                vTaskDelay(100);
                vTaskDelay(120);
                vTaskDelay(50);
                        
                
                LEDPOINT_OFF;
                FMQ_OFF;  					
                           
                
                g_capture_FinishAndStartPhotoFlag=0;		//�������̽��� ��������
                cocoguojia_tim5CatchEnable();				//���̽����� ʹ�ܲ�׽�ж�	 
                
                for(i=1;i<=5;i++)
                {
                    if(1!=g_camPowerFlagTable[i])
                    {
                        i=210;
                    }
                }
                //i=50;//����///////////////////////////
                if(100<i)
                {
                    openWrongNum++;
                    if(3<=openWrongNum)
                    {
                        openWrongNum=0;
                        g_offOrOnTureStateFlag=SYSTEMSTATE_OPENFAIL;
                        xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite);            //����ľ�������͵����� 32λ�����ǵ�ǰ֪ͨ
                        FMQ_ON;
                        vTaskDelay(500);
                        FMQ_OFF;
                        vTaskDelay(100);     
                    }
                }
                else
                {
                    openWrongNum=0;
                }              
            }
            else    
            {

            }
            
             eventTemp=xEventGroupWaitBits(Key_EventGroup,KEY_SHORT,pdTRUE,pdFALSE,0);//�¼��������������������Ȥ���¼����˳�ʱ����¼�λ���������Ȥ�������¼�,���ȴ�          
             if(KEY_SHORT==(eventTemp&KEY_SHORT))//�����̰���
             {
                    g_offOrOnTureStateFlag=SYSTEMSTATE_GOTOSHUT;
                    xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite);            //����ľ�������͵����� 32λ�����ǵ�ǰ֪ͨ
                    FMQ_ON;
                    vTaskDelay(500);
                    FMQ_OFF;
                    vTaskDelay(100);
             }        
        }
        else
        {           
            if(1==g_capture_FinishAndStartPhotoFlag)
            {
                g_capture_FinishAndStartPhotoFlag=0;			//�ػ�״̬ ��������
            }
        }	
        vTaskDelay(1); 
    } 		
}

