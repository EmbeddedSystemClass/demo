

#include "bsp.h"
#include "myinclude.h"

#ifdef CPU_TASK_EN  
#include "bsp_TiMbase.h" 
#endif

//----------------------------------------------------------------------------
void BSP_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//要进行IO口设置 首先先定义一个IO口结构体
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);
	

	FAN_OFF;LEDPOINT_OFF;KEY_LED_OFF;FMQ_OFF;
	
	//-------------------------------------------------
	//蜂鸣器和相机拍照指示灯
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);   
    
	//-------------------------------------------------
	//数据通信检测管脚
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	//-------------------------------------------------
	//软开关
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	//-------------------------------------------------
	//软开关 指示灯
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
    
    //-------------------------------------------------
	//涡轮风扇
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
    
  //-------------------------------------------------

	/*相机开机关机管脚*/  // 1个总的
	
	
	/*相机拍照管脚*/   //5个 分别控制的
	
	
	/* ok按键 */
	
	
	
	
    
    FAN_OFF;LEDPOINT_OFF;KEY_LED_OFF;FMQ_OFF;
		
		
		
		
		
		
		
	cocoguojia_libInit();
    
    #ifdef CPU_TASK_EN  
	BASIC_TIM_Init(); /* 基本定时器初始化	*/
    #endif
}


