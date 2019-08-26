

#include "cocoguojia_init.h"

//----------------------------------------------------------------------
//内部函数
static void cocoguojia_debugJtagSwdSet(u8 mode);//JTAG SWD 模式设置


//#################################################################################################
//全局函数
//#################################################################################################

////////////////////////////////////////////////////////////////
//功能: COCO_LIB库函数应用初始化 里面还是设置JTAG SWD模式
void cocoguojia_libInit(void)
{	
    uint8_t returnDat=1;
	//------------------------------------------------------------------------------------------------------------
	//先把涉及到的GPIO使能	(这个在这里没有也行 不过一开头先都使能还是不错地)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);	
	
	//----------------------------------------------------------------
	//00		JTAG+SWD
	//01		SWD
	//10		ALL SHUT
	cocoguojia_debugJtagSwdSet(0x01); 	 						//coocguojia 一般只使能SWD
	//----------------------------------------------------------------
	//#define NVIC_PriorityGroup_0         //0 bits for pre-emption priority   4 bits for subpriority 
	//#define NVIC_PriorityGroup_1         //1 bits for pre-emption priority   3 bits for subpriority 
	//#define NVIC_PriorityGroup_2         //2 bits for pre-emption priority   2 bits for subpriority 
	//#define NVIC_PriorityGroup_3         //3 bits for pre-emption priority   1 bits for subpriority 
	//#define NVIC_PriorityGroup_4         //4 bits for pre-emption priority   0 bits for subpriority 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);		//中断分组  cocoguojia认为在UCOS里全部中断选择 抢占优先级=0 子优先级=0 这样不产生中断嵌套 避免不可控的情况出现 是个好选择
    //---------------------------------------------------------------- 
         						
    cocoguojia_rgbledInit();
    while(1==returnDat)
    {
        returnDat=cocoguojia_ds18b20Init();
    }   
    //cocoguojia_extiInit();
	cocoguojia_usartNConfigurationInit(1,115200);
    cocoguojia_usartNConfigurationInit(2,9600);     
	cocoguojia_oledInit();	
    cocoguojia_tim5CatchInit();                         //这个在读完eeprom后 再决定是否使能
	cocoguojia_tim1Init();
	cocoguojia_eeCheckOk();
    
	
}

///////////////////////////////////////////////////////////
//功能:系统软件复位
void cocoguojia_systemSoftReset(void)
{   
	SCB->AIRCR =0X05FA0000|(u32)0x04;	  
} 


//#################################################################################################
//内部函数
//#################################################################################################
//////////////////////////////////////////////////
//功能: 	JTAG SWD 模式设置
//形式参数: mode
//00		全使能
//01		使能SWD
//10		全关闭
//备注:		要把JTAG口引脚作为普通GPIO口使用时，必须关闭JTAG,当关闭JTAG后将不能用JLINK下载程序，但可以用串口下载,用JTAG下载程序前，必须使能JTAG	
static void cocoguojia_debugJtagSwdSet(u8 mode)
{
	u32 temp;
	temp=mode;
	temp<<=25;
	RCC->APB2ENR|=1<<0;     //开启辅助时钟	   
	AFIO->MAPR&=0XF8FFFFFF; //清除MAPR的[26:24]
	AFIO->MAPR|=temp;       //设置jtag模式
} 









