
/* Includes ------------------------------------------------------------------*/
#include "psdk_arch_sys.h"
#include "pps.h"
#include "log_printf.h"
#include "flash.h"
#include <os_util.h>


#include "myinclude.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define HOTSHOE_RCC          RCC_APB2Periph_GPIOA
#define HOTSHOE_PORT         GPIOA
#define HOTSHOE_PIN          GPIO_Pin_1
//#define HOTSHOE_EXTIPORT    EXTI_PortSourceGPIOA
//#define HOTSHOE_EXTIPIN     EXTI_PinSource1
#define HOTSHOE_EXTILINE    EXTI_Line1
#define HOTSHOE_IRQn        EXTI1_IRQn


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//precise survey HOTSHOE signal semaphore and global variables

static osSemaphoreId s_hotShoeSem = NULL;




void HotShoe_Init(void)  //热靴信号初始化
{
    EXTI_InitTypeDef EXTI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd((RCC_APB2Periph_GPIOA),ENABLE);
												
	/* 配置为浮空输入 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	/* EXTI为中断模式 */
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* 下降沿中断 */
	EXTI_InitStructure.EXTI_Trigger =EXTI_Trigger_Rising; //EXTI_Trigger_Falling;//EXTI_Trigger_Rising;	
	/* 选择EXTI的信号源 */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1); 
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	//cocoguojia_exitInterruptDisable();
	
  /* 配置中断源：*/
  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
  /* 配置抢占优先级 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
  /* 配置子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  /* 使能中断通道 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  //cocoguojia_exitInterruptEnable();
          
	osSemaphoreDef(hotShoe);  //声明一个信号量容器
  s_hotShoeSem = osSemaphoreCreate(osSemaphore(hotShoe), 1); //在线程里给信号量容器创建一个令牌
  osSemaphoreWait(s_hotShoeSem, 0);	 //信号量-1；
}

/**
 * @brief Check whether PPS signal be triggered once.检查PPS信号是否触发一次。
 * @param None.
 * @return None.
 */
bool HotShoe_isTriggered(void)
{
    if(osSemaphoreWait(s_hotShoeSem, 0) == osOK)
    {
        return true;
    }
    else 
    {

        return false;
    }
}


#define HOTSHOE_IRQHandler  EXTI1_IRQHandler
void HOTSHOE_IRQHandler(void)
{
    if (EXTI_GetITStatus(HOTSHOE_EXTILINE) != RESET)
    {        
        osSemaphoreRelease(s_hotShoeSem);
        EXTI_ClearITPendingBit(HOTSHOE_EXTILINE);

    }
}


