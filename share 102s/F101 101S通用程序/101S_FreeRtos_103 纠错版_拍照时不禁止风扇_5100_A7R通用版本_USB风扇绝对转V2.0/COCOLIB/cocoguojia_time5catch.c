


///////////////////////////////////////////////////
//����ͷ�ļ�
#include "cocoguojia_time5catch.h"

//�ɿ�����ˣ��ɿػ����PWM���� ������15ms 
//ʱ�䵥λ us  ����15ms  ���պ�ƽʱ��������һ��
//���ߵ�ƽʱ��ƽʱ��900~1200  ʵ��1052
//��Ҫ���յ�ʱ�򣬸ߵ�ƽʱ���Ǵ���1600~2100  ʵ��1998
//����������


/************ͨ�ö�ʱ��TIM�������壬ֻ��TIM2��3��4��5************/
// ��ʹ�ò�ͬ�Ķ�ʱ����ʱ�򣬶�Ӧ��GPIO�ǲ�һ���ģ����Ҫע��
// ��������Ĭ��ʹ��TIM5

void cocoguojia_tim5CatchInit(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure; 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	
    //������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
	
	//���벶��ͨ�� GPIO ��ʼ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;				//����ĳ���������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
   
	// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn ;	
	// ���������ȼ�Ϊ 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	// ������ռ���ȼ�Ϊ0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

	// PWM�ź� ���ں�ռ�ձȵļ���
	// ARR ���Զ���װ�ؼĴ�����ֵ
	// CLK_cnt����������ʱ�ӣ����� Fck_int / (psc+1) = 72M/(psc+1)
	// PWM �źŵ����� T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
	// ռ�ձ�P=CCR/(ARR+1)
	
	//ʱ���ṹ���ʼ��	 
	// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
	TIM_TimeBaseStructure.TIM_Period=CATCH_PERIOD;		//0XFFFF;	
	// ����CNT��������ʱ�� = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= (72-1);	
	// ʱ�ӷ�Ƶ���� ����������ʱ��ʱ��Ҫ�õ�
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// ����������ģʽ������Ϊ���ϼ���
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// �ظ���������ֵ��û�õ����ù�
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// ��ʼ����ʱ��
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	
	//���벶��ṹ���ʼ��
	// �������벶���ͨ������Ҫ���ݾ����GPIO������
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	
	// ���벶���źŵļ�������
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	// ����ͨ���Ͳ���ͨ����ӳ���ϵ����ֱ���ͷ�ֱ������
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	// �������Ҫ��������źŵķ�Ƶϵ��
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	// �������Ҫ��������źŵ��˲�ϵ��
	TIM_ICInitStructure.TIM_ICFilter = 0;
	// ��ʱ�����벶���ʼ��
	TIM_PWMIConfig(TIM5, &TIM_ICInitStructure);//�ر�Ҫע����һ�� Ӧ����TIM_PWMIConfig


	//��ֹ������
	TIM_Cmd(TIM5, DISABLE);
	//��ֹ���ºͲ����ж�   
	TIM_ITConfig(TIM5, TIM_IT_CC2, DISABLE);
	TIM_ITConfig(TIM5, TIM_IT_CC1, DISABLE);
	TIM_ITConfig(TIM5, TIM_IT_Update, DISABLE);
	// ������ºͲ����жϱ�־λ
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);
	TIM_ClearFlag(TIM5, TIM_IT_CC1);
	TIM_ClearFlag(TIM5, TIM_IT_CC2);	
	g_capture_FinishAndStartPhotoFlag=0;
	cocoguojia_tim5CatchEnable();
}

void cocoguojia_tim5CatchEnable(void)
{
	//�������ºͲ����ж�  
	//�ж�����
	TIM_ITConfig(TIM5, TIM_IT_CC2, ENABLE);
	TIM_ITConfig(TIM5, TIM_IT_CC1, ENABLE);
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
	
	// ������ºͲ����жϱ�־λ
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);
	TIM_ClearFlag(TIM5, TIM_IT_CC1);
	TIM_ClearFlag(TIM5, TIM_IT_CC2);
	
	// ʹ�ܼ�����
	TIM_Cmd(TIM5, ENABLE);
}


void cocoguojia_tim5CatchDisable(void)
{
	//��ֹ������
	TIM_Cmd(TIM5, DISABLE);
	
	//��ֹ���ºͲ����ж�   
	TIM_ITConfig(TIM5, TIM_IT_CC2, DISABLE);
	TIM_ITConfig(TIM5, TIM_IT_CC1, DISABLE);
	TIM_ITConfig(TIM5, TIM_IT_Update, DISABLE);
	//������ºͲ����жϱ�־λ
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);
	TIM_ClearFlag(TIM5, TIM_IT_CC1);
	TIM_ClearFlag(TIM5, TIM_IT_CC2);	
}


void cocoguojia_tim5CatchDisableInterrupt(void)
{
	//��ֹ������
	TIM_Cmd(TIM5, DISABLE);
	
	//��ֹ���ºͲ����ж�   
	TIM_ITConfig(TIM5, TIM_IT_CC2, DISABLE);
	TIM_ITConfig(TIM5, TIM_IT_CC1, DISABLE);
	TIM_ITConfig(TIM5, TIM_IT_Update, DISABLE);
	// ������ºͲ����жϱ�־λ
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);
	TIM_ClearFlag(TIM5, TIM_IT_CC1);
	TIM_ClearFlag(TIM5, TIM_IT_CC2);	
}

static uint16_t  rising_edge_num;		//�����ط���ʱ�� TIM5�������ݴ�
static uint16_t  falling_edge_num;		//�½��ط���ʱ�� TIM5�������ݴ�
static uint8_t	 update_num=0;			//��������ݴ�
static uint8_t 	 lastGetNum=0;			//�������� ����Ч����Ĵ��� �����趨Ϊ����3�ξ�����Ч
static uint8_t 	 orderNumFlag=0;		//Ҫ�Ȳ��������� �ٲ����½��ز�������ȷ�Ĳ�׽��ʽ





void TIM5_IRQHandler(void)
{
	uint16_t  temp=0;
    uint32_t ulReturn;
    //�����ٽ�Σ��ٽ�ο���Ƕ��
    ulReturn = taskENTER_CRITICAL_FROM_ISR();
	
	if ( TIM_GetITStatus (TIM5, TIM_IT_CC1 ) == SET)	//��һͨ�������ز����ж�
	{
		TIM_ClearITPendingBit(TIM5,TIM_IT_CC1);     	//��� TIM5 ���жϴ�����λ
		if(0==g_capture_FinishAndStartPhotoFlag)		//�����������û�����ڽ���
		{
			if(0==orderNumFlag)							//���˳�� ����Ҫ����������
			{		
				rising_edge_num=TIM_GetCapture1(TIM5);	//�ѵ�ǰ��TIM5��������rising_edge_num
				update_num=0;							//�����������
				orderNumFlag=1;							//����˳���־ ��Ҫ�����½���		
			}
		}
		else
		{
			lastGetNum=0;								//�������� ����Ч����Ĵ��� ���� ����������� 
														//�����������������ǻ���PWM��⵽ ��ɵļٴ����ۼ�
			
			orderNumFlag=0;								//����˳���־ ��Ҫ����������
		}	
	}

	if (TIM_GetITStatus(TIM5,TIM_IT_CC2)==SET)								//�ڶ�ͨ�� �½��ز���
	{	
		TIM_ClearITPendingBit(TIM5,TIM_IT_CC2);								//���жϱ�־
		if(0==g_capture_FinishAndStartPhotoFlag)
		{
			if(1==orderNumFlag)												//�����ǰ˳���־�� ��Ҫ�����½���
			{	
				falling_edge_num=TIM_GetCapture2(TIM5);						//�ѵ�ǰ��TIM5������falling_edge_num
				if(falling_edge_num>=rising_edge_num)						//���
				{
					temp=falling_edge_num-rising_edge_num;
				}
				else
				{
					temp=CATCH_PERIOD+1-rising_edge_num+falling_edge_num;	//����Ҫ��� ��Ҫ����������	
				}	
				g_tim5PlauseUs=temp;										//����������ֵд��ȫ�ֱ��� ����OLED��ʾ��
				
				if(SYSTEMSTATE_ONTRUE==g_offOrOnTureStateFlag)				//�������
				{
					if((1600<=temp)&&(2200>=temp))							//���������Χ ��Ҫ������������
					{
						lastGetNum++;										//�������� ����Ч����Ĵ���++
						if(3<=lastGetNum)
						{	
							g_tim5TriggerPlauseUs=temp;						//�ѵ�ǰֵ����g_tim5TriggerPlauseUs ������ֵ						
							g_capture_FinishAndStartPhotoFlag=1;			//��׽��������Ҫ������������ ��־��λ�� ���ж����������
							lastGetNum=0;									//�������� ����Ч����Ĵ��� ���� ����������� 
						}
					}
					else
					{
						lastGetNum=0;										//�������� ����Ч����Ĵ��� ���� ����������� 
					}
				}
				orderNumFlag=0;												//����˳���־ ��Ҫ����������
			}
		}
		else																//�������������� ��Ҫ����
		{
			lastGetNum=0;													//�������� ����Ч����Ĵ��� ���� ����������� 
			orderNumFlag=0;													//����˳���־ ��Ҫ����������
		}	
		
	}
	if (TIM_GetITStatus(TIM5,TIM_IT_Update)==SET)							//��ʱ�����
	{
		TIM_ClearITPendingBit(TIM5,TIM_IT_Update);
		update_num++;
		if (update_num>=10)
		{
			update_num=10;													//������10���� ��˵������ʱ��̫���� 40ms�� �������Ϊ2 ��������Ϊû������
			g_tim5PlauseUs=0;
			orderNumFlag=0;
			lastGetNum=0;
		}
	}
    // �˳��ٽ�� 
    taskEXIT_CRITICAL_FROM_ISR( ulReturn );
}



	
//--------------------------------------------------------------------------------------------------------
//TTL ������ʽ
void cocoguojia_ttlTestInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//���벶��ͨ�� GPIO ��ʼ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;				//����ĳ���������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
}	









