 
 
///////////////////////////////////////////////////
//自身头文件
#include "cocoguojia_rgbled.h"

void cocoguojia_rgbledInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//要进行IO口设置 首先先定义一个IO口结构体
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);
	
		
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14|GPIO_Pin_15;//RGB LED 管脚初始化 都为上拉推挽输出 低电平灯亮
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;//RGB LED 管脚初始化 都为上拉推挽输出 低电平灯亮
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
}

