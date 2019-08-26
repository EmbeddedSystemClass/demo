
#ifndef __USART1_COCOGUOJIA_H__
#define __USART1_COCOGUOJIA_H__

#include "myinclude.h"

#define USART1RXDMAXSIZE   16
extern uint8_t cocoguojia_usart1ReceiveTable[USART1RXDMAXSIZE];
extern uint16_t cocoguojia_usart1ReceiveRearPoint;
extern uint16_t cocoguojia_usart1ReceiveFrontPoint;

 	
extern void cocoguojia_usart1Init(void);
extern void cocoguojia_usart1PutChar(uint8_t Dat);
extern void cocoguojia_usart1PutStr(uint8_t *str);


#endif
