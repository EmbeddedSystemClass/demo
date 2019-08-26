  
  
///////////////////////////////////////////////////
//自身头文件
#include "cocoguojia_usart3.h"

uint8_t  cocoguojia_usart3ReceiveTable[USART3RXDMAXSIZE];	//usart3串口接收缓冲区
uint16_t cocoguojia_usart3ReceiveFrontPoint=0;				//usart3串口接收队列头指针
uint16_t cocoguojia_usart3ReceiveRearPoint=0;				//usart3串口接收队列尾指针

//usart3初始化
void cocoguojia_usart3Init(void)    
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	//打开串口1的GPIO时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);	//打开串口1的外设时钟
	
	//------------------------------------------------------------------------------------
	//usart3的GPIO配置
	//usart3 Tx 的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	//usart3 Rx 的GPIO配置为浮空复用模式
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	//------------------------------------------------------------------------------------
	//配置串口的工作参数
	USART_InitStructure.USART_BaudRate=(uint32_t)9600;							    //波特率设置
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;						//数据长度
	USART_InitStructure.USART_StopBits=USART_StopBits_1;							//停止位
	USART_InitStructure.USART_Parity=USART_Parity_No;								//校验方式
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;	//硬件流控制
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;						//工作模式 收发一起
	USART_Init(USART3,&USART_InitStructure);										//完成串口的初始化设置
	
	//------------------------------------------------------------------------------------
	//usart3的中断控制器配置
	NVIC_InitStructure.NVIC_IRQChannel=USART3_IRQn;				//配置usart3的中断源
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;		//抢占优先级设置
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;			//子优先级设置
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;				//使能中断
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);				//使能串口接收中断
	USART_Cmd(USART3,ENABLE);									//使能串口1
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


//串口3的中断函数
void USART3_IRQHandler(void)
{
	uint8_t dat;
	uint16_t rearTemp=0;
	
	OSIntEnter(); 	 //用于统计中断的嵌套层数，对嵌套层数+1
	
    if(RESET!=USART_GetITStatus(USART3,USART_IT_RXNE))
	{
		dat=USART_ReceiveData(USART3);
		
		cocoguojia_usart3ReceiveTable[cocoguojia_usart3ReceiveRearPoint]=dat;         	//接收到的数据写入尾指针     
        rearTemp=((cocoguojia_usart3ReceiveRearPoint)+1)%USART3RXDMAXSIZE ;  			//尾指针加一后 取余运算 先存到暂存寄存器里
        if(cocoguojia_usart3ReceiveFrontPoint!=rearTemp)                   				//如果队不满 头指针不等于尾指针
		{          
            cocoguojia_usart3ReceiveRearPoint=rearTemp;                      			//赋予尾指针取余后的值
		}
        else
        {
            ;                                               							//队列满则不处理了
        }    
	}
	   
    //这一步 为了在退出前 清零中断标志 中断内处理时候 可能有延时 导致前面说的那种情况
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {         
            USART_ClearITPendingBit(USART3, USART_IT_RXNE);//清除中断标志     
	}
 

	
    //防止有接收溢出中断的Bug
    if(SET==USART_GetFlagStatus(USART3,USART_FLAG_ORE))
    {
        USART_ClearFlag(USART3,USART_FLAG_ORE);
        USART_ReceiveData(USART3);
    }
	
	OSIntExit();  //在os_core.c文件里定义,如果有更高优先级的任务就绪了,则执行一次任务切换 

	
}

