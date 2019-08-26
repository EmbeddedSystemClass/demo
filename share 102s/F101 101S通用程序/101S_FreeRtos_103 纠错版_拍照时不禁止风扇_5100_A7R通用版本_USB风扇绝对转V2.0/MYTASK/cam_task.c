#include "cam_task.h"
 
/*************************************************
Function: void  CAM_TASK (void const * argument)
Description: �ж��������ʲô����״̬���Ӷ����н�����л��Ϳ��ػ�����
Return: �޷���ֵ
*************************************************/

TaskHandle_t 	CAM_TASK_Handle = NULL;		//����������

void  CAM_TASK (void* parameter)
{  
  	uint8_t n = 0,i = 0,j = 0;
    EventBits_t eventTemp=0;

  

    /* �ȴ���־g_camTaskCanTakeOverFlag��λ cam������Խӹܱ�־��λ */
    while (0 == g_camTaskCanTakeOverFlag) 
    {
        vTaskDelay(10);
    }
	
    vTaskDelay(100);    
	  POWER_NO;            /* �ػ�@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
    vTaskDelay(100);
    
    
    /* ����Ƿ������ػ� */
    for(i = 1; i <= 5; i++)
    {
        if(0 != g_camPowerFlagTable[i])
        {
            i = 210;
        }
    }
    if(200 <= i)
    {
        g_offOrOnTureStateFlag = SYSTEMSTATE_SHUTFAIL;    /* �ػ�ʧ�� */  	
    }
    else
    {
        g_offOrOnTureStateFlag = SYSTEMSTATE_OFFTRUE;     /* �ػ��ɹ� */
    }
    xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite); /* ����ľ�������͵����� 32λ�����ǵ�ǰ֪ͨ */   
    FMQ_ON;
    vTaskDelay(500);
    FMQ_OFF;
    vTaskDelay(100);
    n = 0;    
   
	while (1) 
	{  
    /*   ��ʵ�ػ���   */
    if(SYSTEMSTATE_OFFTRUE == g_offOrOnTureStateFlag)        
		{             
            /* �����5������Ŀ��ػ�״̬ */         
            for(i = 1; i <= 5; i++)
            {
                if(0 != g_camPowerFlagTable[i])
                {
                    i = 210;
                }
            }

            /* ��������û�ػ�����ͻȻ���� */
            if(200 <= i)
            {
                POWER_NO;           /* �ػ�@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
                vTaskDelay(300);  
                g_offOrOnTureStateFlag=SYSTEMSTATE_SHUTFAIL;
                xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite);            /* ����ľ�������͵����� 32λ�����ǵ�ǰ֪ͨ */
                FMQ_ON;
                vTaskDelay(500);
                FMQ_OFF;
                vTaskDelay(100);
                n = 0;                
            }
            
            /* ����Ƿ��а��������� ��������� ������ */            
            eventTemp = xEventGroupWaitBits(Key_EventGroup,KEY_SHORT,pdTRUE,pdFALSE,0);/* �¼��������������������Ȥ���¼����˳�ʱ����¼�λ���������Ȥ�������¼�,���ȴ�  */         
            if(KEY_SHORT == (eventTemp&KEY_SHORT))//�����̰���
            {
                g_offOrOnTureStateFlag = SYSTEMSTATE_GOTOOPEN;
                xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite);   /* ����ľ�������͵����� 32λ�����ǵ�ǰ֪ͨ */
                FMQ_ON;
                vTaskDelay(500);
                FMQ_OFF;
                vTaskDelay(100);
                n = 0;
            }        
     }
		
		
		/*  �ػ�ʧ��  */   
    if(SYSTEMSTATE_SHUTFAIL == g_offOrOnTureStateFlag)	         
		{	
            n++;
            if((1 == n) || (0 == (n % 3)))
            {
                FMQ_ON;
            }
            else
            {
                FMQ_OFF;
            }    
            if(8 <= n)
            {
								POWER_NO;       /* �ػ�@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
                vTaskDelay(100);                
                n=0;
            }
           
			/* �ֱ���5������Ŀ��ػ�״̬  */
            vTaskDelay(300);       
            for(i = 1; i <= 5; i++)
            {
                if(0 != g_camPowerFlagTable[i])
                {
                    i = 210;
                }
            }
            if(100 > i)
            {
                g_offOrOnTureStateFlag = SYSTEMSTATE_OFFTRUE;
                xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite);  /* ����ľ�������͵����� 32λ�����ǵ�ǰ֪ͨ */
                FMQ_ON;
                vTaskDelay(500);
                FMQ_OFF;
                vTaskDelay(100);
                n = 0;   	
            }
            else
            {                
                eventTemp = xEventGroupWaitBits(Key_EventGroup,KEY_SHORT,pdTRUE,pdFALSE,0);/* �¼��������������������Ȥ���¼����˳�ʱ����¼�λ���������Ȥ�������¼�,���ȴ� */         
                if(KEY_SHORT==(eventTemp&KEY_SHORT))/* �����̰��� */
                {
                    g_offOrOnTureStateFlag = SYSTEMSTATE_GOTOOPEN;
                    xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite);       /* ����ľ�������͵����� 32λ�����ǵ�ǰ֪ͨ */
                    FMQ_ON;
                    vTaskDelay(500);
                    FMQ_OFF;
                    vTaskDelay(100);
                    n = 0;
                } 
            }                
     } 
		
		 
		/* ����ʧ�� */    
    if(SYSTEMSTATE_OPENFAIL == g_offOrOnTureStateFlag)			    
		{	
            n++;
            if((1 == n) || (0 == (n % 3)))
            {
                FMQ_ON;
            }
            else
            {
                FMQ_OFF;
            }    
            if(8 <= n)
            {
								POWER_EN;       /* ����@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
                vTaskDelay(100);
                n = 0;
            }
            
                   
           /* �ֱ���5������Ŀ��ػ�״̬  */
            vTaskDelay(300);         
            for(i = 1; i <= 5; i++)
            {
                if(1 != g_camPowerFlagTable[i])
                {
                    i = 210;
                }
            }
            if(100 > i)
            {
								vTaskDelay(3000);
								vTaskDelay(3000);
								vTaskDelay(3000);
								PHOTOALL_EN;               /* ����@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */  
								vTaskDelay(100);
								PHOTOALL_NO;							
								vTaskDelay(3000);		
                g_offOrOnTureStateFlag = SYSTEMSTATE_ONTRUE;
                xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite);  /* ����ľ�������͵����� 32λ�����ǵ�ǰ֪ͨ */
                FMQ_ON;
                vTaskDelay(500);
                FMQ_OFF;
                vTaskDelay(100);
                n = 0;   	
            }
            else
            {                
                eventTemp = xEventGroupWaitBits(Key_EventGroup,KEY_SHORT,pdTRUE,pdFALSE,0);/* �¼��������������������Ȥ���¼����˳�ʱ����¼�λ���������Ȥ�������¼�,���ȴ� */         
                if(KEY_SHORT == (eventTemp&KEY_SHORT))//�����̰���
                {
                    g_offOrOnTureStateFlag = SYSTEMSTATE_GOTOSHUT;
                    xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite);   /* ����ľ�������͵����� 32λ�����ǵ�ǰ֪ͨ */
                    FMQ_ON;
                    vTaskDelay(500);
                    FMQ_OFF;
                    vTaskDelay(100);
                } 
            }                
     } 
		
		 
		/* �ػ��� */  
     if(SYSTEMSTATE_GOTOSHUT == g_offOrOnTureStateFlag)	       
		{	
		    POWER_NO;       /* �ػ�@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
            vTaskDelay(100);
            
            for(j = 0; j < 50; j++)   //15s
            {              
				/* �ֱ���5������Ŀ��ػ�״̬  */
                vTaskDelay(300);
                
                for(i = 1; i <= 5; i++)
                {
                    if(1 == g_camPowerFlagTable[i])
                    {
                        i = 210;
                    }
                }
                if(100 > i)
                {
                    j = 220;
                }   
            }
            if(200 <= j)
            {
                g_offOrOnTureStateFlag = SYSTEMSTATE_OFFTRUE;
                xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite);   /* ����ľ�������͵����� 32λ�����ǵ�ǰ֪ͨ */
                FMQ_ON;
                vTaskDelay(500);
                FMQ_OFF;
                vTaskDelay(100);
                n = 0;   	
            }
            else
            {
			        	POWER_NO;         /* �ػ�@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
                vTaskDelay(100); 
                
                g_offOrOnTureStateFlag = SYSTEMSTATE_SHUTFAIL;     	
                xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite);    /* ����ľ�������͵����� 32λ�����ǵ�ǰ֪ͨ */
                FMQ_ON;
                vTaskDelay(500);
                FMQ_OFF;
                vTaskDelay(100);
                n = 0;    
            } 
     }
		
		 
		/* ������   */
    if(SYSTEMSTATE_GOTOOPEN == g_offOrOnTureStateFlag)								       
		{	  
			      POWER_EN;       /* ����@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
            vTaskDelay(100);      
            for(j = 0; j < 50; j++)//15s
            {
				/* �ֱ���5������Ŀ��ػ�״̬  */
                vTaskDelay(300);      
                for(i = 1; i <= 5; i++)
                {
                    if(0 == g_camPowerFlagTable[i])
                    {
                        i = 210;
                    }
                }
                if(100 > i)
                {
                    j = 220;
                }   
            }
            //j=230;//����//////////////////////////////
            if(200 <= j)
            {  
				
								vTaskDelay(3000);
								vTaskDelay(3000);
								vTaskDelay(3000);
								PHOTOALL_EN;            /* ���� @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  */
								vTaskDelay(100);		
								PHOTOALL_NO;							
								vTaskDelay(3000);	
                g_offOrOnTureStateFlag = SYSTEMSTATE_ONTRUE;                
                xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite);         /* ����ľ�������͵����� 32λ�����ǵ�ǰ֪ͨ */
                FMQ_ON;
                vTaskDelay(500);
                FMQ_OFF;
                vTaskDelay(100);
                n = 0; 
                                
            }
            else
            {
                vTaskDelay(300);
                g_offOrOnTureStateFlag = SYSTEMSTATE_OPENFAIL;     	
                xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite);         /* ����ľ�������͵����� 32λ�����ǵ�ǰ֪ͨ */
                FMQ_ON;
                vTaskDelay(500);
                FMQ_OFF;
                vTaskDelay(100);
                n = 0;    
            } 
     }                               
        vTaskDelay(1);
    }
}




