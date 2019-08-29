
#ifndef __GPIO_COCOGUOJIA_H__
#define __GPIO_COCOGUOJIA_H__




//这里采用的是 GPIO 位带操作  STM32是32位系统的 所以膨胀成4个字节来访问时最高效的
//外设最低1M字节空间 膨胀后(一位变4字节) 当然只有LSB有效
//另: 其实 外设和SRAM 都是 最低1M字节空间 膨胀后(一位变4字节) 当然只有LSB有效
//其膨胀后 的地址范围属于保留地址 故不会和其他寄存器地址重合
//外设地址膨胀前的可膨胀地址区范围 0X40000000~0X40100000
//SARM地址膨胀前的可膨胀地址区范围 0X20000000~0X20100000

//位带地址统一公式
#define BITBAND(addr,bitnum) ((addr&0xf0000000)+0x02000000+((addr&0x00ffffff)<<5)+(bitnum<<2))
//把一个地址转换为一个指针
#define MEM_ADDR(addr) *((volatile unsigned long *)(addr))
//把位带别名区地址转换成指针
#define BIT_BAND(addr,bitnum) MEM_ADDR(BITBAND(addr,bitnum))
//GPIO ODR寄存器地址
#define GPIOA_ODR_ADDR (GPIOA_BASE+12)	
#define GPIOB_ODR_ADDR (GPIOB_BASE+12)	
#define GPIOC_ODR_ADDR (GPIOC_BASE+12)	
#define GPIOD_ODR_ADDR (GPIOD_BASE+12)	
#define GPIOE_ODR_ADDR (GPIOE_BASE+12)	
#define GPIOF_ODR_ADDR (GPIOF_BASE+12)	
#define GPIOG_ODR_ADDR (GPIOG_BASE+12)	
//GPIO IDR寄存器地址
#define GPIOA_IDR_ADDR (GPIOA_BASE+8)	
#define GPIOB_IDR_ADDR (GPIOB_BASE+8)
#define GPIOC_IDR_ADDR (GPIOC_BASE+8)
#define GPIOD_IDR_ADDR (GPIOD_BASE+8)
#define GPIOE_IDR_ADDR (GPIOE_BASE+8)
#define GPIOF_IDR_ADDR (GPIOF_BASE+8)
#define GPIOG_IDR_ADDR (GPIOG_BASE+8)

//GPIO 输出
#define PAout(n) BIT_BAND(GPIOA_ODR_ADDR,n)
#define PBout(n) BIT_BAND(GPIOB_ODR_ADDR,n)
#define PCout(n) BIT_BAND(GPIOC_ODR_ADDR,n)
#define PDout(n) BIT_BAND(GPIOD_ODR_ADDR,n)
#define PEout(n) BIT_BAND(GPIOE_ODR_ADDR,n)
#define PFout(n) BIT_BAND(GPIOF_ODR_ADDR,n)
#define PGout(n) BIT_BAND(GPIOG_ODR_ADDR,n)

//GPIO 输入
#define PAin(n) BIT_BAND(GPIOA_IDR_ADDR,n)
#define PBin(n) BIT_BAND(GPIOB_IDR_ADDR,n)
#define PCin(n) BIT_BAND(GPIOC_IDR_ADDR,n)
#define PDin(n) BIT_BAND(GPIOD_IDR_ADDR,n)
#define PEin(n) BIT_BAND(GPIOE_IDR_ADDR,n)
#define PFin(n) BIT_BAND(GPIOF_IDR_ADDR,n)
#define PGin(n) BIT_BAND(GPIOG_IDR_ADDR,n)

#endif






