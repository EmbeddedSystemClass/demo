#include "myinclude.h"


static TaskHandle_t 	APP_TASKCREATE_Handle 	= NULL;		//����������


//--------------------------------------------------------------------------------
//��������
static void APP_TASKCREATE	(void);						    //���ڴ����������������

EventGroupHandle_t Key_EventGroup=NULL;           //KEY�¼�
QueueHandle_t usart2Send_Queue=NULL;              //��Ϣ����


//--------------------------------------------------------------------------------
//������
int main(void)
{	
  BaseType_t xReturn = pdPASS;	//����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS
  
  //��һ����������Ӳ����ʼ��
  BSP_Init();
	
  //�ڶ���������AppTaskCreate����������
  xReturn = xTaskCreate((TaskFunction_t )	APP_TASKCREATE,  		/* ������ں��� */
                        (const char*    )	"APP_TASKCREATE",		/* �������� */
                        (uint16_t       )	128,  					    /* ����ջ��С */
                        (void*          )	NULL,					      /* ������ں������� */
                        (UBaseType_t    )	1, 						      /* ��������ȼ� */
                        (TaskHandle_t*  )	&APP_TASKCREATE_Handle);	/* ������ƿ�ָ�� */ 
  
  //������������FreeRTOS����ʼ���������           
  if(pdPASS == xReturn)
  {
		vTaskStartScheduler();   //�������񣬿�������
  }
  else
  {
		return -1; 
  }	  
  
  //--------------------------------------------------------------------
  //��������ִ�е�����
  while(1)  
  {
		;
  }
}

//--------------------------------------------------------------------
//���ڴ����������������
static void APP_TASKCREATE(void)
{
    BaseType_t xReturn = pdPASS;		           /* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */

    taskENTER_CRITICAL();           					 /* �����ٽ��� */

    Key_EventGroup=xEventGroupCreate();
    usart2Send_Queue=xQueueCreate(5,1);
    
    
    //���� AppTASKOled ����
    xReturn = xTaskCreate(  (TaskFunction_t )	OLED_TASK, 			    /* ������ں���*/
                            (const char*    )	"OLED_TASK",			  /* ��������*/
                            (uint16_t       )	1024,   				    /* ����ջ��С*/
                            (void*          )	NULL,				        /* ������ں�������*/
                            (UBaseType_t    )	2,	    			      /* ��������ȼ�*/
                            (TaskHandle_t*  )	&OLED_TASK_Handle);	/* ������ƿ�ָ��*/
    if(pdPASS == xReturn)
    {
        PRINTF_BEFORE printf("$$����OLED_TASK����ɹ�\r\n");
    }
                            
    //���� KEY_TASK ����
    xReturn = xTaskCreate(  (TaskFunction_t )	KEY_TASK, 			    /* ������ں���*/
                            (const char*    )	"KEY_TASK",			    /* ��������*/
                            (uint16_t       )	512,   				      /* ����ջ��С*/
                            (void*          )	NULL,				        /* ������ں�������*/
                            (UBaseType_t    )	3,	    			      /* ��������ȼ�*/
                            (TaskHandle_t*  )	&KEY_TASK_Handle);  /* ������ƿ�ָ��*/
    if(pdPASS == xReturn)
    {
        PRINTF_BEFORE printf("$$����KEY_TASK����ɹ�\r\n");
    }
                            
    //���� CAM_TASK ����
    xReturn = xTaskCreate(  (TaskFunction_t )	CAM_TASK, 			    /* ������ں���*/
                            (const char*    )	"CAM_TASK",			    /* ��������*/
                            (uint16_t       )	1024,   				    /* ����ջ��С*/
                            (void*          )	NULL,				        /* ������ں�������*/
                            (UBaseType_t    )	4,	    			      /* ��������ȼ�*/
                            (TaskHandle_t*  )	&CAM_TASK_Handle);	/* ������ƿ�ָ��*/
    if(pdPASS == xReturn)
    {
        PRINTF_BEFORE printf("$$����CAM_TASK����ɹ�\r\n");
    }
                            
    //���� RGB_TASK ����
    xReturn = xTaskCreate(  (TaskFunction_t )	RGB_TASK, 			    /* ������ں���*/
                            (const char*    )	"RGB_TASK",			    /* ��������*/
                            (uint16_t       )	512,   				      /* ����ջ��С*/
                            (void*          )	NULL,				        /* ������ں�������*/
                            (UBaseType_t    )	5,	    			      /* ��������ȼ�*/
                            (TaskHandle_t*  )	&RGB_TASK_Handle);  /* ������ƿ�ָ��*/
    if(pdPASS == xReturn)
    {
        PRINTF_BEFORE printf("$$����RGB_TASK����ɹ�\r\n");
    }
    
    //���� RUNINIT_TASK ����
    xReturn = xTaskCreate(  (TaskFunction_t )	RUNINIT_TASK, 			    /* ������ں���*/
                            (const char*    )	"RUNINIT_TASK",			    /* ��������*/
                            (uint16_t       )	512,   				          /* ����ջ��С*/
                            (void*          )	NULL,				            /* ������ں�������*/
                            (UBaseType_t    )	6,	    			          /* ��������ȼ�*/
                            (TaskHandle_t*  )	&RUNINIT_TASK_Handle);	/* ������ƿ�ָ��*/
    if(pdPASS == xReturn)
    {
        PRINTF_BEFORE printf("$$����RUN_TASK����ɹ�\r\n");
    }
          
             
    //���� U2RECE_TASK ����
    xReturn = xTaskCreate(  (TaskFunction_t )	Cam_status_update_TASK, 			    /* ������ں���*/
                            (const char*    )	"Cam_status_update_TASK",			    /* ��������*/
                            (uint16_t       )	512,   				                    /* ����ջ��С*/
                            (void*          )	NULL,				                      /* ������ں�������*/
                            (UBaseType_t    )	7,	    			                    /* ��������ȼ�*/														
                            (TaskHandle_t*  )	&Cam_status_update_TASK_Handle);	/* ������ƿ�ָ��*/
    if(pdPASS == xReturn)
    {
        PRINTF_BEFORE printf("$$����Cam_status_update_TASK �ɹ�\r\n");
    }
    
    //���� TRIGGER_TASK ����
    xReturn = xTaskCreate(  (TaskFunction_t )	TRIGGER_TASK, 			    /* ������ں���*/
                            (const char*    )	"TRIGGER_TASK",			    /* ��������*/
                            (uint16_t       )	1024,   				        /* ����ջ��С*/
                            (void*          )	NULL,				            /* ������ں�������*/
                            (UBaseType_t    )	8,	    			          /* ��������ȼ�*/
                            (TaskHandle_t*  )	&TRIGGER_TASK_Handle);	/* ������ƿ�ָ��*/
    if(pdPASS == xReturn)
    {
        PRINTF_BEFORE printf("$$����TRIGGER_TASK����ɹ�\r\n");
    }

    //���� TRIGGER_TASK ����
    xReturn = xTaskCreate(  (TaskFunction_t )	TEMP_TASK, 			        /* ������ں���*/
                            (const char*    )	"TEMP_TASK",			      /* ��������*/
                            (uint16_t       )	1024,   				        /* ����ջ��С*/
                            (void*          )	NULL,				            /* ������ں�������*/
                            (UBaseType_t    )	8,	    			          /* ��������ȼ�*/
                            (TaskHandle_t*  )	&TEMP_TASK_Handle);   	/* ������ƿ�ָ��*/
    if(pdPASS == xReturn)
    {
        PRINTF_BEFORE printf("$$����TEMP_TASK�ɹ�\r\n");
    }		
    
    #ifdef CPU_TASK_EN 
    /* ����CPU_TASK���� */
    xReturn = xTaskCreate((TaskFunction_t )CPU_TASK,       /* ������ں��� */
                        (const char*    )"CPU_TASK",       /* �������� */
                        (uint16_t       )512,              /* ����ջ��С */
                        (void*          )NULL,	           /* ������ں������� */
                        (UBaseType_t    )12,	             /* ��������ȼ� */
                        (TaskHandle_t*  )&CPU_TASK_Handle);/* ������ƿ�ָ�� */
    if(pdPASS == xReturn)
    PRINTF_BEFORE printf("����CPU_Task����ɹ�!\r\n");
    #endif
        

		
    vTaskDelete(APP_TASKCREATE_Handle); 							    /* ɾ��AppTASKCreate���� */
  
  
    taskEXIT_CRITICAL();            								      /* �˳��ٽ��� */

}











