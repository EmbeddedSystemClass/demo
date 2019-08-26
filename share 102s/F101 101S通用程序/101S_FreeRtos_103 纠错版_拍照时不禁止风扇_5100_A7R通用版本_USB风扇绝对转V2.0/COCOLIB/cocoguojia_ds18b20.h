#ifndef __COCOGUOJIA_DS18B20_H
#define __COCOGUOJIA_DS18B20_H
#include "myinclude.h" 

//IO��������
#define DS18B20_IO_IN()  {GPIOB->CRL&=0XFFF0FFFF;GPIOB->CRL|=1<<18;}//����ģʽ
#define DS18B20_IO_OUT() {GPIOB->CRL&=0XFFF0FFFF;GPIOB->CRL|=3<<16;}//�������ģʽ  50MHz


////IO��������											   
#define	DS18B20_DQ_OUT PBout(4) //���ݶ˿�
#define	DS18B20_DQ_IN  PBin(4)  //���ݶ˿�
   	
extern uint8_t cocoguojia_ds18b20Init(void);
extern uint16_t cocoguojia_ds18b20GetTemp(void);

#endif















