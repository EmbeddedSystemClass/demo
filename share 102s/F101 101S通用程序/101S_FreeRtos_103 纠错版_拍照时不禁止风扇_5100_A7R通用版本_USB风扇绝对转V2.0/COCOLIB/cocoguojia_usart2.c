


///////////////////////////////////////////////////
//����ͷ�ļ�
#include "cocoguojia_usart2.h"
#include "cocoguojia_ringbuf.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�ⲿ����
//-------------------------------------------------------------------------------------------------
//USART2��ʼ��
void cocoguojia_usart2Init(void)    
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	//�򿪴���2��GPIOʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);	//�򿪴���2������ʱ��
   
	//------------------------------------------------------------------------------------
	//USART1��GPIO����
	//USART1 Tx ��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//USART1 Rx ��GPIO����Ϊ���ո���ģʽ
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	//------------------------------------------------------------------------------------
	//���ô��ڵĹ�������
	USART_InitStructure.USART_BaudRate=(uint32_t)9600;							//����������
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;						//���ݳ���
	USART_InitStructure.USART_StopBits=USART_StopBits_1;							//ֹͣλ
	USART_InitStructure.USART_Parity=USART_Parity_No;								//У�鷽ʽ
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;	//Ӳ��������
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;						//����ģʽ �շ�һ��
	USART_Init(USART2,&USART_InitStructure);										//��ɴ��ڵĳ�ʼ������
	
	//------------------------------------------------------------------------------------
	//USART1���жϿ���������
	NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;				//����USART2���ж�Դ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;		//��ռ���ȼ�����
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;			//�����ȼ�����
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;				//ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);
	
	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);				//ʹ�ܴ��ڽ����ж�
    USART_ClearFlag(USART2,USART_FLAG_TC);                      //���������ɱ�־
    //USART_ITConfig(USART2, USART_IT_TXE, ENABLE);   		    //ʹ�ܷ������ݼĴ������ж�  
	

    //------------------------------------------------------------------------------------------
    //���ν�������ʼ�� 
    USART_Cmd(USART2,ENABLE);									//ʹ�ܴ���2
}	



//-----------------------------------------------------------------
//����2 ����1���ַ�
void cocoguojia_usart2PutChar(uint8_t Dat)
{
	//USART_ClearFlag(USART2,USART_FLAG_TC);  //���ʹ����� �� RS485ͨѶ С��ظ������
	USART_SendData(USART2, Dat);
	while(RESET==USART_GetFlagStatus(USART2,USART_FLAG_TC))
	{
		;
	}
}

//-----------------------------------------------------------------
//����2 �����ַ���
void cocoguojia_usart2PutStr(uint8_t *str)
{
	uint8_t *sp=str;
	uint16_t k=0;
	do
	{
		cocoguojia_usart2PutChar(*(sp+k));
		k++;
	}
	while(0!=(*(sp+k)));
}

//-----------------------------------------------------------------
//����1 �������� ����n��
void cocoguojia_usart2PutTableForN(uint8_t *str,uint16_t n)
{
	uint8_t *sp=str;
	uint16_t k=0;
    
    for(k=0;k<n;k++)
    {
        cocoguojia_usart2PutChar(*(sp+k));
    }
}


////////////////////////////////////////////////////////////
//�жϺ���
void USART2_IRQHandler()
{
    uint8_t dat;
    uint32_t ulReturn;
    
    //�����ٽ�Σ��ٽ�ο���Ƕ��
    ulReturn = taskENTER_CRITICAL_FROM_ISR();
     
    
    if(RESET!=USART_GetITStatus(USART2, USART_IT_RXNE))
    {
        dat = (uint8_t)(USART_ReceiveData(USART2) & 0x00FF);
        
        U2RecBufStruct.rawBuf[U2RecBufStruct.rawWriteIndex]=dat;
        U2RecBufStruct.rawWriteIndex++;
        U2RecBufStruct.rawWriteIndex%=USART2_RECRAW_RINGBUGLENMAX;    
    }
    

     
	//��һ�� Ϊ�����˳�ǰ �����жϱ�־ �ж��ڴ���ʱ�� ��������ʱ ����ǰ��˵���������
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {         
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);//����жϱ�־     
	}
 
	
    //��ֹ�н�������жϵ�Bug
    if(SET==USART_GetFlagStatus(USART2,USART_FLAG_ORE))
    {
        USART_ClearFlag(USART2,USART_FLAG_ORE);
        USART_ReceiveData(USART2);
    }
   
    // �˳��ٽ�� 
    taskEXIT_CRITICAL_FROM_ISR( ulReturn );
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  


 





