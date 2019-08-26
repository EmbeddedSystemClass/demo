 
 


#ifndef __COCOGUOJIA_TIM1PWM_H__
#define __COCOGUOJIA_TIM1PWM_H__

 #include "myinclude.h"




/************ͨ�ö�ʱ��TIM�������壬ֻ��TIM2��3��4��5************/
// ��ʹ�ò�ͬ�Ķ�ʱ����ʱ�򣬶�Ӧ��GPIO�ǲ�һ���ģ����Ҫע��
// ��������Ĭ��ʹ��TIM1

#define            GENERAL_TIM                   TIM3
#define            GENERAL_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            GENERAL_TIM_CLK               RCC_APB2Periph_TIM1
#define            GENERAL_TIM_Period            9
#define            GENERAL_TIM_Prescaler         71
// TIM3 ����Ƚ�ͨ��1
#define            GENERAL_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            GENERAL_TIM_CH1_PORT          GPIOA
#define            GENERAL_TIM_CH1_PIN           GPIO_Pin_6

/**************************��������********************************/

extern void cocoguojia_tim1Init(void);
extern void cocoguojia_tim1Enable(void);
extern void cocoguojia_tim1Disable(void);


#endif	


