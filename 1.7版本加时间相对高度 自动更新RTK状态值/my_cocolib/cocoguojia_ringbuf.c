

#include "cocoguojia_ringbuf.h"

T_Usart2RingBufDoStruct    U2RecBufStruct;
//T_Usart3RingBufDoStruct    U3RecBufStruct;


void cocoguojia_ringBufInit(void) 
{
    //------------------------------------------------------------------------------------------
    //串口2环形接收区初始化
    U2RecBufStruct.rawReadIndex=0;
    U2RecBufStruct.rawWriteIndex=0; 
    U2RecBufStruct.parseIndex=0; 
    memset((U2RecBufStruct.rawBuf),   0, sizeof((U2RecBufStruct.rawBuf)));        //清空分析接收处理缓冲区
    memset((U2RecBufStruct.parseBuf), 0, sizeof(U2RecBufStruct.parseBuf));        //清空串口接收处理缓冲区
    
    /*
    //------------------------------------------------------------------------------------------
    //串口3环形接收区初始化
    U3RecBufStruct.rawReadIndex=0;
    U3RecBufStruct.rawWriteIndex=0; 
    U3RecBufStruct.parseIndex=0; 
    memset((U3RecBufStruct.rawBuf),   0, sizeof((U3RecBufStruct.rawBuf)));        //清空分析接收处理缓冲区
    memset((U3RecBufStruct.parseBuf), 0, sizeof(U3RecBufStruct.parseBuf));        //清空串口接收处理缓冲区
    */
}	




