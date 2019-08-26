#include "cocoguojia_ds18b20.h"


static void delay_us(uint32_t nTimer);
static void DS18B20_Rst(void);
static uint8_t DS18B20_Check(void);
static uint8_t DS18B20_Read_Bit(void);
static uint8_t DS18B20_Read_Byte(void);
static void DS18B20_Write_Byte(uint8_t dat);
static void DS18B20_Start(void);


static void delay_us1(void);



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------
//us��ʱ
static void delay_us750()
{
	u32 i=0;
	for(i=0;i<745;i++){
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();__NOP();
		
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------
//us��ʱ
static void delay_us1(void)
{
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();
		
}




//��ʼ��DS18B20��IO�� DQ ͬʱ���DS�Ĵ���
//����1:������
//����0:����    	 
uint8_t cocoguojia_ds18b20Init(void)
{   
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PORTB��ʱ�� 
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				//PORTB.4 �������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_SetBits(GPIOB,GPIO_Pin_4);    //���1
    
	DS18B20_Rst();
	return DS18B20_Check();
}  

//��ds18b20�õ��¶�ֵ
//���ȣ�0.1C
//����ֵ��
//���� ���Ϊ��λ ������32768
//���� 
//�¶�ֵ ��-550~1250�� 

uint16_t cocoguojia_ds18b20GetTemp(void)
{

    uint8_t temp;
    uint8_t TL,TH;
	uint16_t tem;
    DS18B20_Start ();                    // ds1820 start convert
    DS18B20_Rst();
    DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0xbe);// convert	    
    TL=DS18B20_Read_Byte(); // LSB   
    TH=DS18B20_Read_Byte(); // MSB  
	    	  
    if(TH>7)
    {
        TH=~TH;
        TL=~TL; 
        temp=0;//�¶�Ϊ�� 
        
        tem=TH; //��ø߰�λ
        tem<<=8;    
        tem+=TL;//��õװ�λ 
        tem+=1;
    }
    else 
    {
        temp=1;//�¶�Ϊ��
        
        tem=TH; //��ø߰�λ
        tem<<=8;    
        tem+=TL;//��õװ�λ        
    }
    
    tem=((float)tem)*0.625;//ת�� 
    
    tem/=10;//��ȷ���㼴��

	if(temp)
    {
        return tem; //�����¶�ֵ
    }
	else 
    {
        return (tem|0x8000);  
    }  
   
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------
//us��ʱ
static void delay_us(uint32_t nTimer)
{
	u32 i=0;
	for(i=0;i<nTimer;i++){
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();__NOP();
		
	}
}//__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();

//��λDS18B20
static void DS18B20_Rst(void)	   
{                 
	DS18B20_IO_OUT(); //SET OUTPUT
    DS18B20_DQ_OUT=0; //����DQ
    delay_us750();    //����750us
    DS18B20_DQ_OUT=1; //DQ=1 
    delay_us(15);     //15US
}
//�ȴ�DS18B20�Ļ�Ӧ
//����1:δ��⵽DS18B20�Ĵ���
//����0:����
static uint8_t DS18B20_Check(void) 	   
{   
	uint8_t retry=0;
	DS18B20_IO_IN();//SET  INPUT	 
    while (DS18B20_DQ_IN&&retry<200)
	{
		retry++;
		delay_us1();
	};	 
	if(retry>=200)return 1;
	else retry=0;
    while (!DS18B20_DQ_IN&&retry<240)
	{
		retry++;
		delay_us1();
	};
	if(retry>=240)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
//��DS18B20��ȡһ��λ
//����ֵ��1/0
static uint8_t DS18B20_Read_Bit(void) 			 // read one bit
{
    uint8_t data;
	DS18B20_IO_OUT();//SET  OUTPUT
    DS18B20_DQ_OUT=0; 
	delay_us1();delay_us1();
    DS18B20_DQ_OUT=1; 
	DS18B20_IO_IN();//SET INPUT
	delay_us(15);
	if(DS18B20_DQ_IN)data=1;
    else data=0;	 
    delay_us(60);             
    return data;
}
//��DS18B20��ȡһ���ֽ�
//����ֵ������������
static uint8_t DS18B20_Read_Byte(void)    // read one byte
{        
    uint8_t i,j,dat;
    dat=0;
	for (i=1;i<=8;i++) 
	{
        j=DS18B20_Read_Bit();
        dat=(j<<7)|(dat>>1);
    }						    
    return dat;
}
//дһ���ֽڵ�DS18B20
//dat��Ҫд����ֽ�
static void DS18B20_Write_Byte(uint8_t dat)     
 {             
    uint8_t j;
    uint8_t testb;
	DS18B20_IO_OUT();//SET OUTPUT;
    for (j=1;j<=8;j++) 
	{
        testb=dat&0x01;
        dat=dat>>1;
        if (testb) 
        {
            DS18B20_DQ_OUT=0;// Write 1
            delay_us1();delay_us1();                            
            DS18B20_DQ_OUT=1;
            delay_us(60);             
        }
        else 
        {
            DS18B20_DQ_OUT=0;// Write 0
            delay_us(60);             
            DS18B20_DQ_OUT=1;
            delay_us1();delay_us1();                          
        }
    }
}
 
//��ʼ�¶�ת��
static void DS18B20_Start(void)// ds1820 start convert
{   						               
    DS18B20_Rst();	   
	DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0x44);// convert
} 
















