
#ifndef __RINGBUF_COCOGUOJIA_H
#define __BINGBUF_COCOGUOJIA_H

#include "myinclude.h"

//-----------------------------------------------------------
//串口2 环形接收原始缓冲区
#define USART2_RECRAW_RINGBUGLENMAX 512
//串口2 环形接收分析 缓冲区
#define  USART2_RECPARSE_RINGBUGLENMAX 64


typedef struct Usart2RingBufDoStruct_T{
    uint8_t  rawBuf[USART2_RECRAW_RINGBUGLENMAX];       //串口2 环形接收缓冲区
    uint16_t rawReadIndex;
    uint16_t rawWriteIndex;
    
   uint8_t  parseBuf[USART2_RECPARSE_RINGBUGLENMAX];   //串口2 环形接收分析缓冲区
   uint16_t parseIndex;
} T_Usart2RingBufDoStruct;



extern  T_Usart2RingBufDoStruct    U2RecBufStruct;


/*
//-----------------------------------------------------------
//串口3 环形接收原始缓冲区
#define USART3_RECRAW_RINGBUGLENMAX 1024
//串口3 环形接收分析 缓冲区
#define  USART3_RECPARSE_RINGBUGLENMAX 512
typedef struct Usart3RingBufDoStruct_T{
    uint8_t  rawBuf[USART3_RECRAW_RINGBUGLENMAX];       //串口2 环形接收缓冲区
    uint16_t rawReadIndex;
    uint16_t rawWriteIndex;
    
   uint8_t  parseBuf[USART3_RECPARSE_RINGBUGLENMAX];   //串口2 环形接收分析缓冲区
   uint16_t parseIndex;
} T_Usart3RingBufDoStruct;
//extern  T_Usart3RingBufDoStruct    U3RecBufStruct;
*/



extern void cocoguojia_ringBufInit(void); 


#endif




