
///////////////////////////////////////////////////
//����ͷ�ļ�
#include "cocoguojia_tim1pwm.h"



//���PWM 4K�ķ�����  ռ�ձ�Ϊ50%

//-------------------------------------------------------------------
//CH1 PWM�����ʼ������
void cocoguojia_tim1Init(void)
{
    //������ʱ��1 PWM ����ṹ�����
    TIM_OCInitTypeDef TIM1_OCInitStruct;
     
    //������ʱ��1�ṹ�����
    TIM_TimeBaseInitTypeDef TIM1_TimeBaseInitStruct;
     
    //����GPIO�ṹ�����
    GPIO_InitTypeDef GPIOA_InitStruct;
     
    //ʹ�ܶ�ʱ��1
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
     
    //ʹ��IOʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
     
    //��ʼ��IO����
    GPIOA_InitStruct.GPIO_Pin = GPIO_Pin_11;
    GPIOA_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIOA_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;   
    GPIO_Init(GPIOA,&GPIOA_InitStruct);

     
    //��ʱ������
    TIM1_TimeBaseInitStruct.TIM_Period = 250-1;		//����������Ϊ TIMx ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ ����Ƶ
    TIM1_TimeBaseInitStruct.TIM_Prescaler = 72-1;	//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
    TIM1_TimeBaseInitStruct.TIM_ClockDivision = 0;	//ʱ�ӷָ�:TDTS = Tck_tim
    TIM1_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
    TIM_TimeBaseInit(TIM1,&TIM1_TimeBaseInitStruct);
     
    //���ö�ʱ������PWM�������
    TIM1_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2; //�������ģʽ 2   PWM����Ƚ�ģʽ
    TIM1_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM1_OCInitStruct.TIM_Pulse = 125-1; //���ô�װ�벶��ȽϼĴ���������ֵ
    TIM1_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High; //������Ը�
    TIM_OC4Init(TIM1,&TIM1_OCInitStruct);
	
    //CH1 Ԥװ��ʹ��
    TIM_OC4PreloadConfig(TIM1,TIM_OCPreload_Enable);  
    //ʹ�� TIMx �� ARR �ϵ�Ԥװ�ؼĴ���
    TIM_ARRPreloadConfig(TIM1,ENABLE);
    //��ʱ�����PWMʹ��
    TIM_CtrlPWMOutputs(TIM1,DISABLE);
    //ʹ�ܶ�ʱ��
    TIM_Cmd(TIM1,DISABLE);
}


void cocoguojia_tim1Enable(void)
{	
	//��ʱ�����PWMʹ��
    TIM_CtrlPWMOutputs(TIM1,ENABLE);
	TIM_Cmd(TIM1, ENABLE);
}



void cocoguojia_tim1Disable(void)
{	
    //��ʱ�����PWMʹ��
    TIM_CtrlPWMOutputs(TIM1,DISABLE);
	TIM_Cmd(TIM1, DISABLE);	

}


/*
//------------------------------------------------------------------------------------------------------
void TIM1_IRQHandler(void)
{
    uint32_t ulReturn;
    //�����ٽ�Σ��ٽ�ο���Ƕ��
    ulReturn = taskENTER_CRITICAL_FROM_ISR();
	TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  //����жϱ�־λ
    // �˳��ٽ�� 
    taskEXIT_CRITICAL_FROM_ISR( ulReturn );
}
*/



	



