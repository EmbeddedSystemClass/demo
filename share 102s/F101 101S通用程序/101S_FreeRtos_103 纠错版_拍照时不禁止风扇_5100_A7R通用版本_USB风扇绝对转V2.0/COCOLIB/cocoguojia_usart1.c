  
  
///////////////////////////////////////////////////
//自身头文件
#include "cocoguojia_usart1.h"


uint8_t  cocoguojia_usart1ReceiveTable[USART1RXDMAXSIZE];	//USART1串口接收缓冲区
uint16_t cocoguojia_usart1ReceiveFrontPoint=0;				//USART1串口接收队列头指针
uint16_t cocoguojia_usart1ReceiveRearPoint=0;				//USART1串口接收队列尾指针

//USART1初始化
void cocoguojia_usart1Init(void)    
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	//打开串口1的GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);	//打开串口1的外设时钟
	
	//------------------------------------------------------------------------------------
	//USART1的GPIO配置
	//USART1 Tx 的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//USART1 Rx 的GPIO配置为浮空复用模式
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	//------------------------------------------------------------------------------------
	//配置串口的工作参数
	USART_InitStructure.USART_BaudRate=(uint32_t)115200;							//波特率设置
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;						//数据长度
	USART_InitStructure.USART_StopBits=USART_StopBits_1;							//停止位
	USART_InitStructure.USART_Parity=USART_Parity_No;								//校验方式
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;	//硬件流控制
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;						//工作模式 收发一起
	USART_Init(USART1,&USART_InitStructure);										//完成串口的初始化设置
	
		
	//------------------------------------------------------------------------------------
	//USART1的中断控制器配置
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;				//配置USART1的中断源
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;		//抢占优先级设置
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;			//子优先级设置
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;				//使能中断
	NVIC_Init(&NVIC_InitStructure);
	
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);				//使能串口接收中断
	USART_Cmd(USART1,ENABLE);									//使能串口1
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

//重新定向c库函数printf到串口 把MDK的工程选项"Use MicroLIB"勾选上 这样高度优化使代码更少,占资源更少
int fputc(int ch,FILE *f)
{
	USART_SendData(USART1,(uint8_t)ch);
	while(RESET==USART_GetFlagStatus(USART1,USART_FLAG_TC))
	{
		;
	}
	return(ch);
}


///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		/* 等待串口输入数据 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}

//串口1的中断函数
void USART1_IRQHandler(void)
{
	uint8_t dat;
	uint16_t rearTemp=0;
    uint32_t ulReturn;
    
    //进入临界段，临界段可以嵌套
    ulReturn = taskENTER_CRITICAL_FROM_ISR();
	
    if(RESET!=USART_GetITStatus(USART1,USART_IT_RXNE))
	{
		dat=USART_ReceiveData(USART1);
		
		cocoguojia_usart1ReceiveTable[cocoguojia_usart1ReceiveRearPoint]=dat;         	//接收到的数据写入尾指针     
        rearTemp=((cocoguojia_usart1ReceiveRearPoint)+1)%USART1RXDMAXSIZE ;  			//尾指针加一后 取余运算 先存到暂存寄存器里
        if(cocoguojia_usart1ReceiveFrontPoint!=rearTemp)                   				//如果队不满 头指针不等于尾指针
		{          
            cocoguojia_usart1ReceiveRearPoint=rearTemp;                      			//赋予尾指针取余后的值
		}
        else
        {
            ;                                               							//队列满则不处理了
        }    
	}
	   
    //这一步 为了在退出前 清零中断标志 中断内处理时候 可能有延时 导致前面说的那种情况
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {         
            USART_ClearITPendingBit(USART1, USART_IT_RXNE);//清除中断标志     
	}
 

	
    //防止有接收溢出中断的Bug
    if(SET==USART_GetFlagStatus(USART1,USART_FLAG_ORE))
    {
        USART_ClearFlag(USART1,USART_FLAG_ORE);
        USART_ReceiveData(USART1);
    }
	
    // 退出临界段 
    taskEXIT_CRITICAL_FROM_ISR( ulReturn );

	
}

