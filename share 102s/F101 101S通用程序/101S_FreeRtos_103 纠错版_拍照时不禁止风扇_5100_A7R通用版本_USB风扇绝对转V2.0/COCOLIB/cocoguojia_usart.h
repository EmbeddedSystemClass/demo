

#ifndef ___COCOGUOJIA__USART_H__
#define ___COCOGUOJIA__USART_H__

///////////////////////////////////////////////
//头文件 ucosIII 都是引用这个头文件
/* FreeRTOS头文件 */
#include "myinclude.h" 

//串口接收缓冲区大小与定义
#define USART1RXDMAXSIZE            8
#define USART2RXDMAXSIZE            200
#define USART3RXDMAXSIZE            8
#define USART4RXDMAXSIZE            8
#define USART5RXDMAXSIZE            8


extern uint8_t  cocoguojia_usart1ReceiveTable[USART1RXDMAXSIZE];	//USART1串口接收缓冲区
extern uint16_t cocoguojia_usart1ReceiveWritePoint;				    //USART1串口接收队列头指针
extern uint16_t cocoguojia_usart1ReceiveReadPoint;				    //USART1串口接收队列尾指针

extern uint8_t  cocoguojia_usart2ReceiveTable[USART2RXDMAXSIZE];	//USART1串口接收缓冲区
extern uint16_t cocoguojia_usart2ReceiveWritePoint;				    //USART1串口接收队列头指针
extern uint16_t cocoguojia_usart2ReceiveReadPoint;				    //USART1串口接收队列尾指针


extern void cocoguojia_usartNConfigurationInit  (uint8_t usartNum,uint32_t baud);
extern void cocoguojia_usartNPutChar            (uint8_t uartNum,uint8_t chr);
extern void cocoguojia_usartNPutStr             (uint16_t uartNum,const uint8_t *str);
extern void cocoguojia_usartNPutTableForNum     (uint8_t uartNum,uint8_t *sp, uint16_t num);

#endif








