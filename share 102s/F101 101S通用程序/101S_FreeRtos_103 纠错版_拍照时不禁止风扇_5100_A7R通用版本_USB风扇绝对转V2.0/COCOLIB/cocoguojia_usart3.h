
#ifndef __USART3_COCOGUOJIA_H__
#define __USART3_COCOGUOJIA_H__

///////////////////////////////////////////////
//头文件 ucosIII 都是引用这个头文件
#include "includes.h"



#define USART3RXDMAXSIZE   200
extern uint8_t cocoguojia_usart3ReceiveTable[USART3RXDMAXSIZE];
extern uint16_t cocoguojia_usart3ReceiveRearPoint;
extern uint16_t cocoguojia_usart3ReceiveFrontPoint;

 	
extern void cocoguojia_usart3Init(void);
extern void cocoguojia_usart3PutChar(uint8_t Dat);
extern void cocoguojia_usart3PutStr(uint8_t *str);

#endif
