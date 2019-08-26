
///////////////////////////////////////////////////
//自身头文件
#include "cocoguojia_tim1pwm.h"



//输出PWM 4K的方波形  占空比为50%

//-------------------------------------------------------------------
//CH1 PWM输出初始化设置
void cocoguojia_tim1Init(void)
{
    //声明定时器1 PWM 输出结构体变量
    TIM_OCInitTypeDef TIM1_OCInitStruct;
     
    //声明定时器1结构体变量
    TIM_TimeBaseInitTypeDef TIM1_TimeBaseInitStruct;
     
    //声明GPIO结构体变量
    GPIO_InitTypeDef GPIOA_InitStruct;
     
    //使能定时器1
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
     
    //使能IO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
     
    //初始化IO配置
    GPIOA_InitStruct.GPIO_Pin = GPIO_Pin_11;
    GPIOA_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIOA_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;   
    GPIO_Init(GPIOA,&GPIOA_InitStruct);

     
    //定时器配置
    TIM1_TimeBaseInitStruct.TIM_Period = 250-1;		//设置用来作为 TIMx 时钟频率除数的预分频值 不分频
    TIM1_TimeBaseInitStruct.TIM_Prescaler = 72-1;	//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM1_TimeBaseInitStruct.TIM_ClockDivision = 0;	//时钟分割:TDTS = Tck_tim
    TIM1_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
    TIM_TimeBaseInit(TIM1,&TIM1_TimeBaseInitStruct);
     
    //设置定时器外设PWM输出配置
    TIM1_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2; //脉宽调制模式 2   PWM输出比较模式
    TIM1_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM1_OCInitStruct.TIM_Pulse = 125-1; //设置待装入捕获比较寄存器的脉冲值
    TIM1_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性高
    TIM_OC4Init(TIM1,&TIM1_OCInitStruct);
	
    //CH1 预装载使能
    TIM_OC4PreloadConfig(TIM1,TIM_OCPreload_Enable);  
    //使能 TIMx 在 ARR 上的预装载寄存器
    TIM_ARRPreloadConfig(TIM1,ENABLE);
    //定时器输出PWM使能
    TIM_CtrlPWMOutputs(TIM1,DISABLE);
    //使能定时器
    TIM_Cmd(TIM1,DISABLE);
}


void cocoguojia_tim1Enable(void)
{	
	//定时器输出PWM使能
    TIM_CtrlPWMOutputs(TIM1,ENABLE);
	TIM_Cmd(TIM1, ENABLE);
}



void cocoguojia_tim1Disable(void)
{	
    //定时器输出PWM使能
    TIM_CtrlPWMOutputs(TIM1,DISABLE);
	TIM_Cmd(TIM1, DISABLE);	

}


/*
//------------------------------------------------------------------------------------------------------
void TIM1_IRQHandler(void)
{
    uint32_t ulReturn;
    //进入临界段，临界段可以嵌套
    ulReturn = taskENTER_CRITICAL_FROM_ISR();
	TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  //清除中断标志位
    // 退出临界段 
    taskEXIT_CRITICAL_FROM_ISR( ulReturn );
}
*/



	



