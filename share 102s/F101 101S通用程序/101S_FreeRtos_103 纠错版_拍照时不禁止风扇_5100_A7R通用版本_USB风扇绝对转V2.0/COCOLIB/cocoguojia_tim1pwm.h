 
 


#ifndef __COCOGUOJIA_TIM1PWM_H__
#define __COCOGUOJIA_TIM1PWM_H__

 #include "myinclude.h"




/************通用定时器TIM参数定义，只限TIM2、3、4、5************/
// 当使用不同的定时器的时候，对应的GPIO是不一样的，这点要注意
// 我们这里默认使用TIM1

#define            GENERAL_TIM                   TIM3
#define            GENERAL_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            GENERAL_TIM_CLK               RCC_APB2Periph_TIM1
#define            GENERAL_TIM_Period            9
#define            GENERAL_TIM_Prescaler         71
// TIM3 输出比较通道1
#define            GENERAL_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            GENERAL_TIM_CH1_PORT          GPIOA
#define            GENERAL_TIM_CH1_PIN           GPIO_Pin_6

/**************************函数声明********************************/

extern void cocoguojia_tim1Init(void);
extern void cocoguojia_tim1Enable(void);
extern void cocoguojia_tim1Disable(void);


#endif	


