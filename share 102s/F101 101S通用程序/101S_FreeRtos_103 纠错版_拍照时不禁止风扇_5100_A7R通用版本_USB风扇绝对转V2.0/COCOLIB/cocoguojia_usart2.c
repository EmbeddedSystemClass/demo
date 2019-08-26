


///////////////////////////////////////////////////
//自身头文件
#include "cocoguojia_usart2.h"
#include "cocoguojia_ringbuf.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//外部函数
//-------------------------------------------------------------------------------------------------
//USART2初始化
void cocoguojia_usart2Init(void)    
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	//打开串口2的GPIO时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);	//打开串口2的外设时钟
   
	//------------------------------------------------------------------------------------
	//USART1的GPIO配置
	//USART1 Tx 的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//USART1 Rx 的GPIO配置为浮空复用模式
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	//------------------------------------------------------------------------------------
	//配置串口的工作参数
	USART_InitStructure.USART_BaudRate=(uint32_t)9600;							//波特率设置
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;						//数据长度
	USART_InitStructure.USART_StopBits=USART_StopBits_1;							//停止位
	USART_InitStructure.USART_Parity=USART_Parity_No;								//校验方式
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;	//硬件流控制
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;						//工作模式 收发一起
	USART_Init(USART2,&USART_InitStructure);										//完成串口的初始化设置
	
	//------------------------------------------------------------------------------------
	//USART1的中断控制器配置
	NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;				//配置USART2的中断源
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;		//抢占优先级设置
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;			//子优先级设置
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;				//使能中断
	NVIC_Init(&NVIC_InitStructure);
	
	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);				//使能串口接收中断
    USART_ClearFlag(USART2,USART_FLAG_TC);                      //清除发送完成标志
    //USART_ITConfig(USART2, USART_IT_TXE, ENABLE);   		    //使能发送数据寄存器空中断  
	

    //------------------------------------------------------------------------------------------
    //环形接收区初始化 
    USART_Cmd(USART2,ENABLE);									//使能串口2
}	



//-----------------------------------------------------------------
//串口2 发送1个字符
void cocoguojia_usart2PutChar(uint8_t Dat)
{
	//USART_ClearFlag(USART2,USART_FLAG_TC);  //如果使能这个 则 RS485通讯 小板回复会变慢
	USART_SendData(USART2, Dat);
	while(RESET==USART_GetFlagStatus(USART2,USART_FLAG_TC))
	{
		;
	}
}

//-----------------------------------------------------------------
//串口2 发送字符串
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
//串口1 发送数组 发送n个
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
//中断函数
void USART2_IRQHandler()
{
    uint8_t dat;
    uint32_t ulReturn;
    
    //进入临界段，临界段可以嵌套
    ulReturn = taskENTER_CRITICAL_FROM_ISR();
     
    
    if(RESET!=USART_GetITStatus(USART2, USART_IT_RXNE))
    {
        dat = (uint8_t)(USART_ReceiveData(USART2) & 0x00FF);
        
        U2RecBufStruct.rawBuf[U2RecBufStruct.rawWriteIndex]=dat;
        U2RecBufStruct.rawWriteIndex++;
        U2RecBufStruct.rawWriteIndex%=USART2_RECRAW_RINGBUGLENMAX;    
    }
    

     
	//这一步 为了在退出前 清零中断标志 中断内处理时候 可能有延时 导致前面说的那种情况
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {         
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);//清除中断标志     
	}
 
	
    //防止有接收溢出中断的Bug
    if(SET==USART_GetFlagStatus(USART2,USART_FLAG_ORE))
    {
        USART_ClearFlag(USART2,USART_FLAG_ORE);
        USART_ReceiveData(USART2);
    }
   
    // 退出临界段 
    taskEXIT_CRITICAL_FROM_ISR( ulReturn );
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  


 





