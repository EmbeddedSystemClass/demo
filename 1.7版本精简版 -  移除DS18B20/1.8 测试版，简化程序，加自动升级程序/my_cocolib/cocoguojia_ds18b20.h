#ifndef __COCOGUOJIA_DS18B20_H
#define __COCOGUOJIA_DS18B20_H
#include "myinclude.h" 

//IO方向设置
#define DS18B20_IO_IN()  {GPIOA->CRH&=0X0FFFFFFF;GPIOA->CRH|=1<<30;}//输入模式
#define DS18B20_IO_OUT() {GPIOA->CRH&=0X0FFFFFFF;GPIOA->CRH|=3<<28;}//推挽输出模式  50MHz


////IO操作函数											   
#define	DS18B20_DQ_OUT PAout(15) //数据端口
#define	DS18B20_DQ_IN  PAin(15)  //数据端口
   	
extern uint8_t cocoguojia_ds18b20Init(void);
extern uint16_t cocoguojia_ds18b20GetTemp(void);

#endif















