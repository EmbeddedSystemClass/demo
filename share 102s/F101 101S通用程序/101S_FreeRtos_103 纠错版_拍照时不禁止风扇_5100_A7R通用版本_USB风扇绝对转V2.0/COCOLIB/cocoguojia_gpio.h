
#ifndef __GPIO_COCOGUOJIA_H__
#define __GPIO_COCOGUOJIA_H__




//������õ��� GPIO λ������  STM32��32λϵͳ�� �������ͳ�4���ֽ�������ʱ���Ч��
//�������1M�ֽڿռ� ���ͺ�(һλ��4�ֽ�) ��Ȼֻ��LSB��Ч
//��: ��ʵ �����SRAM ���� ���1M�ֽڿռ� ���ͺ�(һλ��4�ֽ�) ��Ȼֻ��LSB��Ч
//�����ͺ� �ĵ�ַ��Χ���ڱ�����ַ �ʲ���������Ĵ�����ַ�غ�
//�����ַ����ǰ�Ŀ����͵�ַ����Χ 0X40000000~0X40100000
//SARM��ַ����ǰ�Ŀ����͵�ַ����Χ 0X20000000~0X20100000

//λ����ַͳһ��ʽ
#define BITBAND(addr,bitnum) ((addr&0xf0000000)+0x02000000+((addr&0x00ffffff)<<5)+(bitnum<<2))
//��һ����ַת��Ϊһ��ָ��
#define MEM_ADDR(addr) *((volatile unsigned long *)(addr))
//��λ����������ַת����ָ��
#define BIT_BAND(addr,bitnum) MEM_ADDR(BITBAND(addr,bitnum))
//GPIO ODR�Ĵ�����ַ
#define GPIOA_ODR_ADDR (GPIOA_BASE+12)	
#define GPIOB_ODR_ADDR (GPIOB_BASE+12)	
#define GPIOC_ODR_ADDR (GPIOC_BASE+12)	
#define GPIOD_ODR_ADDR (GPIOD_BASE+12)	
#define GPIOE_ODR_ADDR (GPIOE_BASE+12)	
#define GPIOF_ODR_ADDR (GPIOF_BASE+12)	
#define GPIOG_ODR_ADDR (GPIOG_BASE+12)	
//GPIO IDR�Ĵ�����ַ
#define GPIOA_IDR_ADDR (GPIOA_BASE+8)	
#define GPIOB_IDR_ADDR (GPIOB_BASE+8)
#define GPIOC_IDR_ADDR (GPIOC_BASE+8)
#define GPIOD_IDR_ADDR (GPIOD_BASE+8)
#define GPIOE_IDR_ADDR (GPIOE_BASE+8)
#define GPIOF_IDR_ADDR (GPIOF_BASE+8)
#define GPIOG_IDR_ADDR (GPIOG_BASE+8)

//GPIO ���
#define PAout(n) BIT_BAND(GPIOA_ODR_ADDR,n)
#define PBout(n) BIT_BAND(GPIOB_ODR_ADDR,n)
#define PCout(n) BIT_BAND(GPIOC_ODR_ADDR,n)
#define PDout(n) BIT_BAND(GPIOD_ODR_ADDR,n)
#define PEout(n) BIT_BAND(GPIOE_ODR_ADDR,n)
#define PFout(n) BIT_BAND(GPIOF_ODR_ADDR,n)
#define PGout(n) BIT_BAND(GPIOG_ODR_ADDR,n)

//GPIO ����
#define PAin(n) BIT_BAND(GPIOA_IDR_ADDR,n)
#define PBin(n) BIT_BAND(GPIOB_IDR_ADDR,n)
#define PCin(n) BIT_BAND(GPIOC_IDR_ADDR,n)
#define PDin(n) BIT_BAND(GPIOD_IDR_ADDR,n)
#define PEin(n) BIT_BAND(GPIOE_IDR_ADDR,n)
#define PFin(n) BIT_BAND(GPIOF_IDR_ADDR,n)
#define PGin(n) BIT_BAND(GPIOG_IDR_ADDR,n)

#endif






