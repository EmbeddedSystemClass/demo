
#ifndef __INIT_COCOGUOJIA_H__
#define __INIT_COCOGUOJIA_H__

///////////////////////////////////////////////
//ͷ�ļ� ucosIII �����������ͷ�ļ�
///////////////////////////////////////////////////
//����ͷ�ļ�
#include "myinclude.h"
#include "cocoguojia_gpio.h"
#include "cocoguojia_rgbled.h"
//#include "cocoguojia_exti.h"
#include "cocoguojia_oled.h"
#include "cocoguojia_tim1pwm.h" 
#include "cocoguojia_time5catch.h"
#include "cocoguojia_24c02.h"
#include "cocoguojia_usart.h"
#include "cocoguojia_ds18b20.h"

extern void cocoguojia_libInit(void);				//����:COCO_LIB�⺯��Ӧ�ó�ʼ�� ���滹������JTAG SWDģʽ
extern void cocoguojia_systemSoftReset(void);		//ϵͳ�����λ

#endif


