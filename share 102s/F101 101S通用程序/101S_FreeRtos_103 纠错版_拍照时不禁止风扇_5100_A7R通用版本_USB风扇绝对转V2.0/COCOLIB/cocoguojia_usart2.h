
#ifndef __USART2_COCOGUOJIA_H__
#define __USART2_COCOGUOJIA_H__
#include "myinclude.h"

extern void cocoguojia_usart2Init(void);
extern void cocoguojia_usart2PutChar(uint8_t Dat);
extern void cocoguojia_usart2PutStr(uint8_t *str);
extern void cocoguojia_usart2PutTableForN(uint8_t *str,uint16_t n);


#endif


