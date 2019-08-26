  
  
///////////////////////////////////////////////////
//����ͷ�ļ�
#include "cocoguojia_usart1.h"


uint8_t  cocoguojia_usart1ReceiveTable[USART1RXDMAXSIZE];	//USART1���ڽ��ջ�����
uint16_t cocoguojia_usart1ReceiveFrontPoint=0;				//USART1���ڽ��ն���ͷָ��
uint16_t cocoguojia_usart1ReceiveRearPoint=0;				//USART1���ڽ��ն���βָ��

//USART1��ʼ��
void cocoguojia_usart1Init(void)    
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	//�򿪴���1��GPIOʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);	//�򿪴���1������ʱ��
	
	//------------------------------------------------------------------------------------
	//USART1��GPIO����
	//USART1 Tx ��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//USART1 Rx ��GPIO����Ϊ���ո���ģʽ
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	//------------------------------------------------------------------------------------
	//���ô��ڵĹ�������
	USART_InitStructure.USART_BaudRate=(uint32_t)115200;							//����������
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;						//���ݳ���
	USART_InitStructure.USART_StopBits=USART_StopBits_1;							//ֹͣλ
	USART_InitStructure.USART_Parity=USART_Parity_No;								//У�鷽ʽ
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;	//Ӳ��������
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;						//����ģʽ �շ�һ��
	USART_Init(USART1,&USART_InitStructure);										//��ɴ��ڵĳ�ʼ������
	
		
	//------------------------------------------------------------------------------------
	//USART1���жϿ���������
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;				//����USART1���ж�Դ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;		//��ռ���ȼ�����
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;			//�����ȼ�����
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;				//ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);
	
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);				//ʹ�ܴ��ڽ����ж�
	USART_Cmd(USART1,ENABLE);									//ʹ�ܴ���1
}	



void cocoguojia_usart1PutChar(uint8_t Dat)
{
	USART_SendData(USART1, Dat);
	while(RESET==USART_GetFlagStatus(USART1,USART_FLAG_TC))
	{
		;
	}
}


void cocoguojia_usart1PutStr(uint8_t *str)
{
	uint8_t *sp=str;
	uint16_t k=0;
	do
	{
		cocoguojia_usart1PutChar(*(sp+k));
		k++;
	}
	while(0!=(*(sp+k)));

	while(RESET==USART_GetFlagStatus(USART1,USART_FLAG_TC))
	{
		;
	}
}

//���¶���c�⺯��printf������ ��MDK�Ĺ���ѡ��"Use MicroLIB"��ѡ�� �����߶��Ż�ʹ�������,ռ��Դ����
int fputc(int ch,FILE *f)
{
	USART_SendData(USART1,(uint8_t)ch);
	while(RESET==USART_GetFlagStatus(USART1,USART_FLAG_TC))
	{
		;
	}
	return(ch);
}


///�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		/* �ȴ������������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}

//����1���жϺ���
void USART1_IRQHandler(void)
{
	uint8_t dat;
	uint16_t rearTemp=0;
    uint32_t ulReturn;
    
    //�����ٽ�Σ��ٽ�ο���Ƕ��
    ulReturn = taskENTER_CRITICAL_FROM_ISR();
	
    if(RESET!=USART_GetITStatus(USART1,USART_IT_RXNE))
	{
		dat=USART_ReceiveData(USART1);
		
		cocoguojia_usart1ReceiveTable[cocoguojia_usart1ReceiveRearPoint]=dat;         	//���յ�������д��βָ��     
        rearTemp=((cocoguojia_usart1ReceiveRearPoint)+1)%USART1RXDMAXSIZE ;  			//βָ���һ�� ȡ������ �ȴ浽�ݴ�Ĵ�����
        if(cocoguojia_usart1ReceiveFrontPoint!=rearTemp)                   				//����Ӳ��� ͷָ�벻����βָ��
		{          
            cocoguojia_usart1ReceiveRearPoint=rearTemp;                      			//����βָ��ȡ����ֵ
		}
        else
        {
            ;                                               							//�������򲻴�����
        }    
	}
	   
    //��һ�� Ϊ�����˳�ǰ �����жϱ�־ �ж��ڴ���ʱ�� ��������ʱ ����ǰ��˵���������
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {         
            USART_ClearITPendingBit(USART1, USART_IT_RXNE);//����жϱ�־     
	}
 

	
    //��ֹ�н�������жϵ�Bug
    if(SET==USART_GetFlagStatus(USART1,USART_FLAG_ORE))
    {
        USART_ClearFlag(USART1,USART_FLAG_ORE);
        USART_ReceiveData(USART1);
    }
	
    // �˳��ٽ�� 
    taskEXIT_CRITICAL_FROM_ISR( ulReturn );

	
}

