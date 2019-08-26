#ifndef __COCOGUOJIA_DS18B20_H
#define __COCOGUOJIA_DS18B20_H
#include "myinclude.h" 

//IO方向设置
#define DS18B20_IO_IN()  {GPIOB->CRL&=0XFFF0FFFF;GPIOB->CRL|=1<<18;}//输入模式
#define DS18B20_IO_OUT() {GPIOB->CRL&=0XFFF0FFFF;GPIOB->CRL|=3<<16;}//推挽输出模式  50MHz


////IO操作函数											   
#define	DS18B20_DQ_OUT PBout(4) //数据端口
#define	DS18B20_DQ_IN  PBin(4)  //数据端口
   	
extern uint8_t cocoguojia_ds18b20Init(void);
extern uint16_t cocoguojia_ds18b20GetTemp(void);

#endif















