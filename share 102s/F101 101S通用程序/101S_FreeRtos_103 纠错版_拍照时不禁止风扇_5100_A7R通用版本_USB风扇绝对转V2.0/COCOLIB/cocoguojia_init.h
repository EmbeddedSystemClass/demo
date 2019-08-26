
#ifndef __INIT_COCOGUOJIA_H__
#define __INIT_COCOGUOJIA_H__

///////////////////////////////////////////////
//头文件 ucosIII 都是引用这个头文件
///////////////////////////////////////////////////
//自身头文件
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

extern void cocoguojia_libInit(void);				//功能:COCO_LIB库函数应用初始化 里面还是设置JTAG SWD模式
extern void cocoguojia_systemSoftReset(void);		//系统软件复位

#endif


