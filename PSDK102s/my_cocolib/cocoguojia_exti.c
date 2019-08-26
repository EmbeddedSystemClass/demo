
  

///////////////////////////////////////////////////
//自身头文件
#include "cocoguojia_exti.h"

//热靴检测先以第二版的CAM2为例子


//---------------------------------------------------------------------------------------
void cocoguojia_extiInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
    
	
	//开启按键GPIO口的时钟
	RCC_APB2PeriphClockCmd((RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO),ENABLE);
											
	//选择按键用到的GPIO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//配置为浮空输入
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;				//EXTI为中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;			//下降沿中断
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);		//选择EXTI0的信号源
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	//EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);		//选择EXTI1的信号源
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	//EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource3);		//选择EXTI3的信号源
	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	//EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource4);		//选择EXTI4的信号源
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
	//EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource5);		//选择EXTI5的信号源
	EXTI_InitStructure.EXTI_Line = EXTI_Line5;
	//EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	cocoguojia_exitInterruptDisable();
	
	//配置 NVIC 中断	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;				//配置中断源
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		//配置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				//配置子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//使能中断通道 
	NVIC_Init(&NVIC_InitStructure);
	//配置 NVIC 中断	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;				//配置中断源
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		//配置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				//配置子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//使能中断通道 
	NVIC_Init(&NVIC_InitStructure);
	//配置 NVIC 中断	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;				//配置中断源
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		//配置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				//配置子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//使能中断通道 
	NVIC_Init(&NVIC_InitStructure);
	//配置 NVIC 中断	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;				//配置中断源
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		//配置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				//配置子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//使能中断通道 
	NVIC_Init(&NVIC_InitStructure);
	//配置 NVIC 中断	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;				//配置中断源
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		//配置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				//配置子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//使能中断通道 
	NVIC_Init(&NVIC_InitStructure);
	
}


//-----------------------------------------
//禁止5个口线中断
void cocoguojia_exitInterruptDisable(void)
{
	EXTI->IMR &= (~(EXTI_Line0));
	EXTI->IMR &= (~(EXTI_Line1));
	EXTI->IMR &= (~(EXTI_Line3));
	EXTI->IMR &= (~(EXTI_Line4));
	EXTI->IMR &= (~(EXTI_Line5));
	
	EXTI->PR = EXTI_Line0;			//写1可以清除中断标志
	EXTI->PR = EXTI_Line1;			//写1可以清除中断标志
	EXTI->PR = EXTI_Line3;			//写1可以清除中断标志
	EXTI->PR = EXTI_Line4;			//写1可以清除中断标志
	EXTI->PR = EXTI_Line5;			//写1可以清除中断标志
}

//-----------------------------------------
//使能5个口线中断
void cocoguojia_exitInterruptEnable(void)
{	
	EXTI->PR = EXTI_Line0;			//写1可以清除中断标志
	EXTI->PR = EXTI_Line1;			//写1可以清除中断标志
	EXTI->PR = EXTI_Line3;			//写1可以清除中断标志
	EXTI->PR = EXTI_Line4;			//写1可以清除中断标志
	EXTI->PR = EXTI_Line5;			//写1可以清除中断标志
	
	EXTI->IMR |=EXTI_Line0;
	EXTI->IMR |=EXTI_Line1;
	EXTI->IMR |=EXTI_Line3;
	EXTI->IMR |=EXTI_Line4;
	EXTI->IMR |=EXTI_Line5;
}

    


//-----------------------------------------------------------
//0 口线中断函数
void EXTI0_IRQHandler(void)
{
	uint32_t ulReturn;
    //进入临界段，临界段可以嵌套
    ulReturn = taskENTER_CRITICAL_FROM_ISR();
	if(EXTI_GetITStatus(EXTI_Line0)!=RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line0);
		g_camRtkFlagTable[2]=1;
		
	}
   
    // 退出临界段 
    taskEXIT_CRITICAL_FROM_ISR( ulReturn );
}

//-----------------------------------------------------------
//1 口线中断函数
void EXTI1_IRQHandler(void)
{
	uint32_t ulReturn;
    //进入临界段，临界段可以嵌套
    ulReturn = taskENTER_CRITICAL_FROM_ISR();	
	if(EXTI_GetITStatus(EXTI_Line1)!=RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line1);
		g_camRtkFlagTable[1]=1;
		
	}
	   
    // 退出临界段 
    taskEXIT_CRITICAL_FROM_ISR( ulReturn );
}

//-----------------------------------------------------------
//3 口线中断函数
void EXTI3_IRQHandler(void)
{
	uint32_t ulReturn;
    //进入临界段，临界段可以嵌套
    ulReturn = taskENTER_CRITICAL_FROM_ISR();	
	if(EXTI_GetITStatus(EXTI_Line3)!=RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line3);
		g_camRtkFlagTable[5]=1;
		
	}
	   
    // 退出临界段 
    taskEXIT_CRITICAL_FROM_ISR( ulReturn );
}

//-----------------------------------------------------------
//4 口线中断函数
void EXTI4_IRQHandler(void)
{
	uint32_t ulReturn;
    //进入临界段，临界段可以嵌套
    ulReturn = taskENTER_CRITICAL_FROM_ISR();	
	if(EXTI_GetITStatus(EXTI_Line4)!=RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line4);
		g_camRtkFlagTable[4]=1;
		
	}
	   
    // 退出临界段 
    taskEXIT_CRITICAL_FROM_ISR( ulReturn );
}

//-----------------------------------------------------------
//5到9 口线中断函数
void EXTI9_5_IRQHandler(void)
{
	uint32_t ulReturn;
    //进入临界段，临界段可以嵌套
    ulReturn = taskENTER_CRITICAL_FROM_ISR();
	if(EXTI_GetITStatus(EXTI_Line5)!=RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line5);
		g_camRtkFlagTable[3]=1;
	}	
	   
    // 退出临界段 
    taskEXIT_CRITICAL_FROM_ISR( ulReturn );
}












