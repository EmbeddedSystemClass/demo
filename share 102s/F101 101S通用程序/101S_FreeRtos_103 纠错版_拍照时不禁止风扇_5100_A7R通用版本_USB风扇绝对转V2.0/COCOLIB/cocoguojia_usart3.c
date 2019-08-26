  
  
///////////////////////////////////////////////////
//����ͷ�ļ�
#include "cocoguojia_usart3.h"

uint8_t  cocoguojia_usart3ReceiveTable[USART3RXDMAXSIZE];	//usart3���ڽ��ջ�����
uint16_t cocoguojia_usart3ReceiveFrontPoint=0;				//usart3���ڽ��ն���ͷָ��
uint16_t cocoguojia_usart3ReceiveRearPoint=0;				//usart3���ڽ��ն���βָ��

//usart3��ʼ��
void cocoguojia_usart3Init(void)    
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	//�򿪴���1��GPIOʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);	//�򿪴���1������ʱ��
	
	//------------------------------------------------------------------------------------
	//usart3��GPIO����
	//usart3 Tx ��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	//usart3 Rx ��GPIO����Ϊ���ո���ģʽ
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	//------------------------------------------------------------------------------------
	//���ô��ڵĹ�������
	USART_InitStructure.USART_BaudRate=(uint32_t)9600;							    //����������
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;						//���ݳ���
	USART_InitStructure.USART_StopBits=USART_StopBits_1;							//ֹͣλ
	USART_InitStructure.USART_Parity=USART_Parity_No;								//У�鷽ʽ
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;	//Ӳ��������
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;						//����ģʽ �շ�һ��
	USART_Init(USART3,&USART_InitStructure);										//��ɴ��ڵĳ�ʼ������
	
	//------------------------------------------------------------------------------------
	//usart3���жϿ���������
	NVIC_InitStructure.NVIC_IRQChannel=USART3_IRQn;				//����usart3���ж�Դ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;		//��ռ���ȼ�����
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;			//�����ȼ�����
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;				//ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);				//ʹ�ܴ��ڽ����ж�
	USART_Cmd(USART3,ENABLE);									//ʹ�ܴ���1
}	

void cocoguojia_usart3PutChar(uint8_t Dat)
{
	USART_SendData(USART3, Dat);
	while(RESET==USART_GetFlagStatus(USART3,USART_FLAG_TXE))
	{
		;
	}
}

void cocoguojia_usart3PutStr(uint8_t *str)
{
	uint8_t *sp=str;
	uint16_t k=0;
	do
	{
		cocoguojia_usart3PutChar(*(sp+k));
		k++;
	}
	while(0!=(*(sp+k)));

	while(RESET==USART_GetFlagStatus(USART3,USART_FLAG_TC))
	{
		;
	}
}


//����3���жϺ���
void USART3_IRQHandler(void)
{
	uint8_t dat;
	uint16_t rearTemp=0;
	
	OSIntEnter(); 	 //����ͳ���жϵ�Ƕ�ײ�������Ƕ�ײ���+1
	
    if(RESET!=USART_GetITStatus(USART3,USART_IT_RXNE))
	{
		dat=USART_ReceiveData(USART3);
		
		cocoguojia_usart3ReceiveTable[cocoguojia_usart3ReceiveRearPoint]=dat;         	//���յ�������д��βָ��     
        rearTemp=((cocoguojia_usart3ReceiveRearPoint)+1)%USART3RXDMAXSIZE ;  			//βָ���һ�� ȡ������ �ȴ浽�ݴ�Ĵ�����
        if(cocoguojia_usart3ReceiveFrontPoint!=rearTemp)                   				//����Ӳ��� ͷָ�벻����βָ��
		{          
            cocoguojia_usart3ReceiveRearPoint=rearTemp;                      			//����βָ��ȡ����ֵ
		}
        else
        {
            ;                                               							//�������򲻴�����
        }    
	}
	   
    //��һ�� Ϊ�����˳�ǰ �����жϱ�־ �ж��ڴ���ʱ�� ��������ʱ ����ǰ��˵���������
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {         
            USART_ClearITPendingBit(USART3, USART_IT_RXNE);//����жϱ�־     
	}
 

	
    //��ֹ�н�������жϵ�Bug
    if(SET==USART_GetFlagStatus(USART3,USART_FLAG_ORE))
    {
        USART_ClearFlag(USART3,USART_FLAG_ORE);
        USART_ReceiveData(USART3);
    }
	
	OSIntExit();  //��os_core.c�ļ��ﶨ��,����и������ȼ������������,��ִ��һ�������л� 

	
}

