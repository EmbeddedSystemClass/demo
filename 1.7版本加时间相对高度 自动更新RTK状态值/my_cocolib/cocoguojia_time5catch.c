


///////////////////////////////////////////////////
//自身头文件
#include "cocoguojia_time5catch.h"

//飞控输出端，飞控会输出PWM波形 周期是15ms 
//时间单位 us  周期15ms  拍照和平时的周期是一样
//当高电平时间平时是900~1200  实际1052
//当要拍照的时候，高电平时间是大于1600~2100  实际1998
//其它忽略拍


/************通用定时器TIM参数定义，只限TIM2、3、4、5************/
// 当使用不同的定时器的时候，对应的GPIO是不一样的，这点要注意
// 我们这里默认使用TIM5

void cocoguojia_tim5CatchInit(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure; 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	
    //开启定时器时钟,即内部时钟CK_INT=72M
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
	
	//输入捕获通道 GPIO 初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;				//必须改成下拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
   
	// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn ;	
	// 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	// 设置抢占优先级为0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

	// PWM信号 周期和占空比的计算
	// ARR ：自动重装载寄存器的值
	// CLK_cnt：计数器的时钟，等于 Fck_int / (psc+1) = 72M/(psc+1)
	// PWM 信号的周期 T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
	// 占空比P=CCR/(ARR+1)
	
	//时基结构体初始化	 
	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_TimeBaseStructure.TIM_Period=CATCH_PERIOD;		//0XFFFF;	
	// 驱动CNT计数器的时钟 = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= (72-1);	
	// 时钟分频因子 ，配置死区时间时需要用到
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// 计数器计数模式，设置为向上计数
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// 重复计数器的值，没用到不用管
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// 初始化定时器
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	
	//输入捕获结构体初始化
	// 配置输入捕获的通道，需要根据具体的GPIO来配置
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	
	// 输入捕获信号的极性配置
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	// 输入通道和捕获通道的映射关系，有直连和非直连两种
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	// 输入的需要被捕获的信号的分频系数
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	// 输入的需要被捕获的信号的滤波系数
	TIM_ICInitStructure.TIM_ICFilter = 0;
	// 定时器输入捕获初始化
	TIM_PWMIConfig(TIM5, &TIM_ICInitStructure);//特别要注意这一点 应该是TIM_PWMIConfig


	//禁止计数器
	TIM_Cmd(TIM5, DISABLE);
	//禁止更新和捕获中断   
	TIM_ITConfig(TIM5, TIM_IT_CC2, DISABLE);
	TIM_ITConfig(TIM5, TIM_IT_CC1, DISABLE);
	TIM_ITConfig(TIM5, TIM_IT_Update, DISABLE);
	// 清除更新和捕获中断标志位
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);
	TIM_ClearFlag(TIM5, TIM_IT_CC1);
	TIM_ClearFlag(TIM5, TIM_IT_CC2);	
	g_capture_FinishAndStartPhotoFlag=0;
	cocoguojia_tim5CatchEnable();
}

void cocoguojia_tim5CatchEnable(void)
{
	//开启更新和捕获中断  
	//中断设置
	TIM_ITConfig(TIM5, TIM_IT_CC2, ENABLE);
	TIM_ITConfig(TIM5, TIM_IT_CC1, ENABLE);
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
	
	// 清除更新和捕获中断标志位
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);
	TIM_ClearFlag(TIM5, TIM_IT_CC1);
	TIM_ClearFlag(TIM5, TIM_IT_CC2);
	
	// 使能计数器
	TIM_Cmd(TIM5, ENABLE);
}


void cocoguojia_tim5CatchDisable(void)
{
	//禁止计数器
	TIM_Cmd(TIM5, DISABLE);
	
	//禁止更新和捕获中断   
	TIM_ITConfig(TIM5, TIM_IT_CC2, DISABLE);
	TIM_ITConfig(TIM5, TIM_IT_CC1, DISABLE);
	TIM_ITConfig(TIM5, TIM_IT_Update, DISABLE);
	//清除更新和捕获中断标志位
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);
	TIM_ClearFlag(TIM5, TIM_IT_CC1);
	TIM_ClearFlag(TIM5, TIM_IT_CC2);	
}


void cocoguojia_tim5CatchDisableInterrupt(void)
{
	//禁止计数器
	TIM_Cmd(TIM5, DISABLE);
	
	//禁止更新和捕获中断   
	TIM_ITConfig(TIM5, TIM_IT_CC2, DISABLE);
	TIM_ITConfig(TIM5, TIM_IT_CC1, DISABLE);
	TIM_ITConfig(TIM5, TIM_IT_Update, DISABLE);
	// 清除更新和捕获中断标志位
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);
	TIM_ClearFlag(TIM5, TIM_IT_CC1);
	TIM_ClearFlag(TIM5, TIM_IT_CC2);	
}

static uint16_t  rising_edge_num;		//上升沿发生时的 TIM5计数器暂存
static uint16_t  falling_edge_num;		//下降沿发生时的 TIM5计数器暂存
static uint8_t	 update_num=0;			//溢出计数暂存
static uint8_t 	 lastGetNum=0;			//连续捕获 到有效脉冲的次数 这里设定为连续3次就算有效
static uint8_t 	 orderNumFlag=0;		//要先捕获到上升沿 再捕获到下降沿才算是正确的捕捉方式





void TIM5_IRQHandler(void)
{
	uint16_t  temp=0;
    uint32_t ulReturn;
    //进入临界段，临界段可以嵌套
    ulReturn = taskENTER_CRITICAL_FROM_ISR();
	
	if ( TIM_GetITStatus (TIM5, TIM_IT_CC1 ) == SET)	//第一通道上升沿捕获中断
	{
		TIM_ClearITPendingBit(TIM5,TIM_IT_CC1);     	//清除 TIM5 的中断待处理位
		if(0==g_capture_FinishAndStartPhotoFlag)		//如果拍照流程没有正在进行
		{
			if(0==orderNumFlag)							//如果顺序 是先要捕获上升沿
			{		
				rising_edge_num=TIM_GetCapture1(TIM5);	//把当前的TIM5计数器存rising_edge_num
				update_num=0;							//溢出计数清零
				orderNumFlag=1;							//更改顺序标志 是要捕获下降沿		
			}
		}
		else
		{
			lastGetNum=0;								//连续捕获 到有效脉冲的次数 清零 这里必须清零 
														//以免在拍照流程中是还有PWM检测到 造成的假次数累计
			
			orderNumFlag=0;								//更改顺序标志 是要捕获上升沿
		}	
	}

	if (TIM_GetITStatus(TIM5,TIM_IT_CC2)==SET)								//第二通道 下降沿捕获
	{	
		TIM_ClearITPendingBit(TIM5,TIM_IT_CC2);								//清中断标志
		if(0==g_capture_FinishAndStartPhotoFlag)
		{
			if(1==orderNumFlag)												//如果当前顺序标志对 是要捕获下降沿
			{	
				falling_edge_num=TIM_GetCapture2(TIM5);						//把当前的TIM5计数器falling_edge_num
				if(falling_edge_num>=rising_edge_num)						//算差
				{
					temp=falling_edge_num-rising_edge_num;
				}
				else
				{
					temp=CATCH_PERIOD+1-rising_edge_num+falling_edge_num;	//不但要算差 还要加上周期数	
				}	
				g_tim5PlauseUs=temp;										//将读的脉冲值写进全局变量 供给OLED显示用
				
				if(SYSTEMSTATE_ONTRUE==g_offOrOnTureStateFlag)				//如果开机
				{
					if((1600<=temp)&&(2200>=temp))							//如果在允许范围 则要进行拍照流程
					{
						lastGetNum++;										//连续捕获 到有效脉冲的次数++
						if(3<=lastGetNum)
						{	
							g_tim5TriggerPlauseUs=temp;						//把当前值存入g_tim5TriggerPlauseUs 即触发值						
							g_capture_FinishAndStartPhotoFlag=1;			//捕捉结束并且要进行拍照流程 标志置位了 在中断外进行流程
							lastGetNum=0;									//连续捕获 到有效脉冲的次数 清零 这里必须清零 
						}
					}
					else
					{
						lastGetNum=0;										//连续捕获 到有效脉冲的次数 清零 这里必须清零 
					}
				}
				orderNumFlag=0;												//更改顺序标志 是要捕获上升沿
			}
		}
		else																//正在拍照流程中 都要忽略
		{
			lastGetNum=0;													//连续捕获 到有效脉冲的次数 清零 这里必须清零 
			orderNumFlag=0;													//更改顺序标志 是要捕获上升沿
		}	
		
	}
	if (TIM_GetITStatus(TIM5,TIM_IT_Update)==SET)							//定时器溢出
	{
		TIM_ClearITPendingBit(TIM5,TIM_IT_Update);
		update_num++;
		if (update_num>=10)
		{
			update_num=10;													//如果溢出10次了 则说明脉冲时间太长了 40ms了 所以如果为2 则我们认为没有脉冲
			g_tim5PlauseUs=0;
			orderNumFlag=0;
			lastGetNum=0;
		}
	}
    // 退出临界段 
    taskEXIT_CRITICAL_FROM_ISR( ulReturn );
}



	
//--------------------------------------------------------------------------------------------------------
//TTL 触发方式
void cocoguojia_ttlTestInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//输入捕获通道 GPIO 初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;				//必须改成下拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
}	









