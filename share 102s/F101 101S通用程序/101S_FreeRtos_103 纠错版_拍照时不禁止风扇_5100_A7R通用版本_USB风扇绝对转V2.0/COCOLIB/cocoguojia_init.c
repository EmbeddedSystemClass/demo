

#include "cocoguojia_init.h"

//----------------------------------------------------------------------
//�ڲ�����
static void cocoguojia_debugJtagSwdSet(u8 mode);//JTAG SWD ģʽ����


//#################################################################################################
//ȫ�ֺ���
//#################################################################################################

////////////////////////////////////////////////////////////////
//����: COCO_LIB�⺯��Ӧ�ó�ʼ�� ���滹������JTAG SWDģʽ
void cocoguojia_libInit(void)
{	
    uint8_t returnDat=1;
	//------------------------------------------------------------------------------------------------------------
	//�Ȱ��漰����GPIOʹ��	(���������û��Ҳ�� ����һ��ͷ�ȶ�ʹ�ܻ��ǲ����)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);	
	
	//----------------------------------------------------------------
	//00		JTAG+SWD
	//01		SWD
	//10		ALL SHUT
	cocoguojia_debugJtagSwdSet(0x01); 	 						//coocguojia һ��ֻʹ��SWD
	//----------------------------------------------------------------
	//#define NVIC_PriorityGroup_0         //0 bits for pre-emption priority   4 bits for subpriority 
	//#define NVIC_PriorityGroup_1         //1 bits for pre-emption priority   3 bits for subpriority 
	//#define NVIC_PriorityGroup_2         //2 bits for pre-emption priority   2 bits for subpriority 
	//#define NVIC_PriorityGroup_3         //3 bits for pre-emption priority   1 bits for subpriority 
	//#define NVIC_PriorityGroup_4         //4 bits for pre-emption priority   0 bits for subpriority 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);		//�жϷ���  cocoguojia��Ϊ��UCOS��ȫ���ж�ѡ�� ��ռ���ȼ�=0 �����ȼ�=0 �����������ж�Ƕ�� ���ⲻ�ɿص�������� �Ǹ���ѡ��
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
    cocoguojia_tim5CatchInit();                         //����ڶ���eeprom�� �پ����Ƿ�ʹ��
	cocoguojia_tim1Init();
	cocoguojia_eeCheckOk();
    
	
}

///////////////////////////////////////////////////////////
//����:ϵͳ�����λ
void cocoguojia_systemSoftReset(void)
{   
	SCB->AIRCR =0X05FA0000|(u32)0x04;	  
} 


//#################################################################################################
//�ڲ�����
//#################################################################################################
//////////////////////////////////////////////////
//����: 	JTAG SWD ģʽ����
//��ʽ����: mode
//00		ȫʹ��
//01		ʹ��SWD
//10		ȫ�ر�
//��ע:		Ҫ��JTAG��������Ϊ��ͨGPIO��ʹ��ʱ������ر�JTAG,���ر�JTAG�󽫲�����JLINK���س��򣬵������ô�������,��JTAG���س���ǰ������ʹ��JTAG	
static void cocoguojia_debugJtagSwdSet(u8 mode)
{
	u32 temp;
	temp=mode;
	temp<<=25;
	RCC->APB2ENR|=1<<0;     //��������ʱ��	   
	AFIO->MAPR&=0XF8FFFFFF; //���MAPR��[26:24]
	AFIO->MAPR|=temp;       //����jtagģʽ
} 









