#include "oled_task.h"
 
 /* **************************************************
 say:
 OLED ����ʾģ�� ֻ�Ǹ���ʾģ�� �Ǹ��������֫ģ�� û���κξ���Ȩ
 ����дOLED�����ʱ�򣬲�Ҫ���κλ�Ӱ�쵽���˵��õĳ����
 Ҳ������ʱ(�����������100ms����ʱ �����е�)
 �ṩ���ĵ��ú��� ������������
 ����״̬��־ ֵǣ��OLED�Լ�״̬��־
 ��ϵͳ״̬��־û��һ���ϵ ǧ��Ҫ��ϵͳ׮���־����������OLED����ı�־����� �������ҵ� 
 *****************************************************************************************/
 
 
/*************************************************
Function: void  OLED_TASK (void const * argument)
Description: OLED�� ��ʾ��ǰ����Ĺ���״̬����������
Return: �޷���ֵ
*************************************************/
 
TaskHandle_t 	OLED_TASK_Handle = NULL;		//����������


void  OLED_TASK (void* parameter)
{
  BaseType_t xReturn = pdPASS;
	uint32_t p_msg = 0;
	uint8_t n = 0;
	
	OLED_Clear();	
    vTaskDelay(10);	
    
	while (1) 
	{	     
     xReturn = xTaskNotifyWait(0,0XFFFFFFFF,&p_msg,0);/* ���뺯����ʱ���������bit���Ƴ���ʱ��������е�bit����������֪ͨ&p_msg������ʱ��=���ȴ� */
         
		if(pdPASS == xReturn)
		{
            OLED_Clear();
			g_oledStateFlag = p_msg;
            vTaskDelay(10);	 
            n = 0;            
		}
		else
		{
				vTaskDelay(200);
		}    
		
		if(OLEDSHOWSTATE_LOGO == g_oledStateFlag)
		{
			cocoguojia_oledShowShareuavLogoAndID();
		}
		

		if( (SYSTEMSTATE_OFFTRUE == g_oledStateFlag) || (SYSTEMSTATE_ONTRUE == g_oledStateFlag) )
		{
			if((10 < n) && (20 >= n))
			{	
				if(1 == g_triggerFlag)
				{
					n = 40;
                    if(SYSTEMSTATE_ONTRUE == g_oledStateFlag)
                    {
                        cocoguojia_oledRunShowFor1(1,1);
                    }
                    else
                    {
                         cocoguojia_oledRunShowFor1(1,0);
                    }    
					vTaskDelay(200);				                    
				}
				else
				{	
					cocoguojia_oledRunShowFor2();
					n++;
				}	
			}
			else if(10 >= n)
			{
				if(1 == g_triggerFlag)
				{
					n = 1;	
					g_triggerFlag = 0;
					if(SYSTEMSTATE_ONTRUE == g_oledStateFlag)
                    {
                        cocoguojia_oledRunShowFor1(1,1);
                    }
                    else
                    {
                         cocoguojia_oledRunShowFor1(1,0);
                    }    
					vTaskDelay(200);							
				}
				else
				{	
					if(SYSTEMSTATE_ONTRUE == g_oledStateFlag)
                    {
                        cocoguojia_oledRunShowFor1(0,1);
                    }
                    else
                    {
                         cocoguojia_oledRunShowFor1(0,0);
                    }    
					n++;
				}
			}
			else
			{
				OLED_Clear();
				vTaskDelay(2);		
				n=0;
			}
			
			if(40 <= n)
			{
				OLED_Clear();
				vTaskDelay(2);		
				n=0;
			}				
		}
        
    if(OLEDSHOWSTATE_REPAIRMODEL1 == g_oledStateFlag)
		{
			cocoguojia_oledShowRepairModel(1);
		}
    if(OLEDSHOWSTATE_REPAIRMODEL2 == g_oledStateFlag)
		{
			cocoguojia_oledShowRepairModel(2);
		}
    if(OLEDSHOWSTATE_REPAIRMODEL3 == g_oledStateFlag)
		{
			cocoguojia_oledShowRepairModel(3);
		}
    if(OLEDSHOWSTATE_REPAIRMODEL4 == g_oledStateFlag)
		{
			cocoguojia_oledShowRepairModel(4);
		}
		
		

		if(OLEDSHOWSTATE_INIT == g_oledStateFlag)        //��ʾ ϵͳ��ʼ����
		{
			cocoguojia_oledShowSystemInit();
		}
		

		
		if(SYSTEMSTATE_GOTOSHUT == g_oledStateFlag)     //��ʾ �ػ���
		{
			cocoguojia_oledShowShuting();
		}
		

		
		if(SYSTEMSTATE_GOTOOPEN == g_oledStateFlag)     //��ʾ ������
		{
			cocoguojia_oledShowOpening();
		}
		

		
		if(SYSTEMSTATE_SHUTFAIL == g_oledStateFlag)     //�ػ�ʧ��
		{
			cocoguojia_oledShowShutWrong();
		}
		
		
	
		if(SYSTEMSTATE_OPENFAIL == g_oledStateFlag)     //����ʧ��
		{
			cocoguojia_oledShowOpenWrong();
		}	
        
        if(SYSTEMSTATE_CAMLOSE == g_oledStateFlag)     //����ʧ��
		{
			cocoguojia_oledShowLoseWrong();
		}	
         

		
		if(OLEDSHOWSTATE_SETMODEL1 == g_oledStateFlag)    //���뵽����ģʽ page1
		{
			cocoguojia_oledShowSetModel1();
		}

		
		if(OLEDSHOWSTATE_SETMODEL3 == g_oledStateFlag)    //���뵽���ý����ʾ����
		{
			cocoguojia_oledShowSetModel3(0);
		}
		
		
		if(OLEDSHOWSTATE_SETMODEL4 == g_oledStateFlag)    //���뵽���ý����ʾ����
		{
			cocoguojia_oledShowSetModel3(1);
		}
        
        vTaskDelay(10);		
	 }	
}




