
  

///////////////////////////////////////////////////
//����ͷ�ļ�
#include "cocoguojia_exti.h"

//��ѥ������Եڶ����CAM2Ϊ����


//---------------------------------------------------------------------------------------
void cocoguojia_extiInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
    
	
	//��������GPIO�ڵ�ʱ��
	RCC_APB2PeriphClockCmd((RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO),ENABLE);
											
	//ѡ�񰴼��õ���GPIO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//����Ϊ��������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;				//EXTIΪ�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;			//�½����ж�
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);		//ѡ��EXTI0���ź�Դ
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	//EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);		//ѡ��EXTI1���ź�Դ
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	//EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource3);		//ѡ��EXTI3���ź�Դ
	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	//EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource4);		//ѡ��EXTI4���ź�Դ
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
	//EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource5);		//ѡ��EXTI5���ź�Դ
	EXTI_InitStructure.EXTI_Line = EXTI_Line5;
	//EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	cocoguojia_exitInterruptDisable();
	
	//���� NVIC �ж�	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;				//�����ж�Դ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		//������ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				//���������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//ʹ���ж�ͨ�� 
	NVIC_Init(&NVIC_InitStructure);
	//���� NVIC �ж�	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;				//�����ж�Դ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		//������ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				//���������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//ʹ���ж�ͨ�� 
	NVIC_Init(&NVIC_InitStructure);
	//���� NVIC �ж�	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;				//�����ж�Դ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		//������ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				//���������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//ʹ���ж�ͨ�� 
	NVIC_Init(&NVIC_InitStructure);
	//���� NVIC �ж�	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;				//�����ж�Դ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		//������ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				//���������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//ʹ���ж�ͨ�� 
	NVIC_Init(&NVIC_InitStructure);
	//���� NVIC �ж�	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;				//�����ж�Դ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		//������ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				//���������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//ʹ���ж�ͨ�� 
	NVIC_Init(&NVIC_InitStructure);
	
}


//-----------------------------------------
//��ֹ5�������ж�
void cocoguojia_exitInterruptDisable(void)
{
	EXTI->IMR &= (~(EXTI_Line0));
	EXTI->IMR &= (~(EXTI_Line1));
	EXTI->IMR &= (~(EXTI_Line3));
	EXTI->IMR &= (~(EXTI_Line4));
	EXTI->IMR &= (~(EXTI_Line5));
	
	EXTI->PR = EXTI_Line0;			//д1��������жϱ�־
	EXTI->PR = EXTI_Line1;			//д1��������жϱ�־
	EXTI->PR = EXTI_Line3;			//д1��������жϱ�־
	EXTI->PR = EXTI_Line4;			//д1��������жϱ�־
	EXTI->PR = EXTI_Line5;			//д1��������жϱ�־
}

//-----------------------------------------
//ʹ��5�������ж�
void cocoguojia_exitInterruptEnable(void)
{	
	EXTI->PR = EXTI_Line0;			//д1��������жϱ�־
	EXTI->PR = EXTI_Line1;			//д1��������жϱ�־
	EXTI->PR = EXTI_Line3;			//д1��������жϱ�־
	EXTI->PR = EXTI_Line4;			//д1��������жϱ�־
	EXTI->PR = EXTI_Line5;			//д1��������жϱ�־
	
	EXTI->IMR |=EXTI_Line0;
	EXTI->IMR |=EXTI_Line1;
	EXTI->IMR |=EXTI_Line3;
	EXTI->IMR |=EXTI_Line4;
	EXTI->IMR |=EXTI_Line5;
}

    


//-----------------------------------------------------------
//0 �����жϺ���
void EXTI0_IRQHandler(void)
{
	uint32_t ulReturn;
    //�����ٽ�Σ��ٽ�ο���Ƕ��
    ulReturn = taskENTER_CRITICAL_FROM_ISR();
	if(EXTI_GetITStatus(EXTI_Line0)!=RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line0);
		g_camRtkFlagTable[2]=1;
		
	}
   
    // �˳��ٽ�� 
    taskEXIT_CRITICAL_FROM_ISR( ulReturn );
}

//-----------------------------------------------------------
//1 �����жϺ���
void EXTI1_IRQHandler(void)
{
	uint32_t ulReturn;
    //�����ٽ�Σ��ٽ�ο���Ƕ��
    ulReturn = taskENTER_CRITICAL_FROM_ISR();	
	if(EXTI_GetITStatus(EXTI_Line1)!=RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line1);
		g_camRtkFlagTable[1]=1;
		
	}
	   
    // �˳��ٽ�� 
    taskEXIT_CRITICAL_FROM_ISR( ulReturn );
}

//-----------------------------------------------------------
//3 �����жϺ���
void EXTI3_IRQHandler(void)
{
	uint32_t ulReturn;
    //�����ٽ�Σ��ٽ�ο���Ƕ��
    ulReturn = taskENTER_CRITICAL_FROM_ISR();	
	if(EXTI_GetITStatus(EXTI_Line3)!=RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line3);
		g_camRtkFlagTable[5]=1;
		
	}
	   
    // �˳��ٽ�� 
    taskEXIT_CRITICAL_FROM_ISR( ulReturn );
}

//-----------------------------------------------------------
//4 �����жϺ���
void EXTI4_IRQHandler(void)
{
	uint32_t ulReturn;
    //�����ٽ�Σ��ٽ�ο���Ƕ��
    ulReturn = taskENTER_CRITICAL_FROM_ISR();	
	if(EXTI_GetITStatus(EXTI_Line4)!=RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line4);
		g_camRtkFlagTable[4]=1;
		
	}
	   
    // �˳��ٽ�� 
    taskEXIT_CRITICAL_FROM_ISR( ulReturn );
}

//-----------------------------------------------------------
//5��9 �����жϺ���
void EXTI9_5_IRQHandler(void)
{
	uint32_t ulReturn;
    //�����ٽ�Σ��ٽ�ο���Ƕ��
    ulReturn = taskENTER_CRITICAL_FROM_ISR();
	if(EXTI_GetITStatus(EXTI_Line5)!=RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line5);
		g_camRtkFlagTable[3]=1;
	}	
	   
    // �˳��ٽ�� 
    taskEXIT_CRITICAL_FROM_ISR( ulReturn );
}












