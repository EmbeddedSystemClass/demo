

#include "bsp.h"
#include "myinclude.h"

#ifdef CPU_TASK_EN  
#include "bsp_TiMbase.h" 
#endif

//----------------------------------------------------------------------------
void BSP_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//Ҫ����IO������ �����ȶ���һ��IO�ڽṹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);
	

	FAN_OFF;LEDPOINT_OFF;KEY_LED_OFF;FMQ_OFF;
	
	//-------------------------------------------------
	//���������������ָʾ��
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);   
    
	//-------------------------------------------------
	//����ͨ�ż��ܽ�
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	//-------------------------------------------------
	//����
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	//-------------------------------------------------
	//���� ָʾ��
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
    
    //-------------------------------------------------
	//���ַ���
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
    
  //-------------------------------------------------

	/*��������ػ��ܽ�*/  // 1���ܵ�
	
	
	/*������չܽ�*/   //5�� �ֱ���Ƶ�
	
	
	/* ok���� */
	
	
	
	
    
    FAN_OFF;LEDPOINT_OFF;KEY_LED_OFF;FMQ_OFF;
		
		
		
		
		
		
		
	cocoguojia_libInit();
    
    #ifdef CPU_TASK_EN  
	BASIC_TIM_Init(); /* ������ʱ����ʼ��	*/
    #endif
}


