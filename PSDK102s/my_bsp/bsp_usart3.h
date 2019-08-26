#ifndef __BSP_USART3_H__
#define	__BSP_USART3_H__



#include "myinclude.h"

#define PRINTF_BEFORE if(0==1)
//#define PRINTF_BEFORE 



#define PRINTF_BEFORE1 if(0==1)
//#define PRINTF_BEFORE1 



//#define PRINTF_BEFORE2 if(0==1)
#define PRINTF_BEFORE2 

//------------------------------------------------
//18554016766@126.com
//shareuav122333


//-------------------------------------------------------------------------------------------------
//用户开发帐户
#define USER_APP_NAME               "SHARE-101S"
#define USER_APP_ID                 "65187"
#define USER_APP_KEY                "f37923a5b2f94477defd81783ef1005"
#define USER_DEVELOPER_ACCOUNT      "18554016766@126.com"


//PSDK protocol data packet data field max length

#define PSDK_PROTHEADEBEFORERLEN          10
#define PSDK_PROTHEADECRC16RLEN           2
#define PSDK_PROTDATAMAXLEN               256
#define PSDK_PROTHEADECRC32RLEN           4



#define PSDK_PROTHEADERLEN   (PSDK_PROTHEADEBEFORERLEN+PSDK_PROTHEADECRC16RLEN)




#define USER_OTHER_NAME               "SHARE-101S"


/* Exported macros -----------------------------------------------------------*/
#define PSDK_PROTFRAMEMAXLEN         (PSDK_PROTHEADEBEFORERLEN + PSDK_PROTDATAMAXLEN + PSDK_PROTHEADECRC32RLEN)
#define PSDK_PROTFRAMEMINLEN         (PSDK_PROTHEADERLEN + PSDK_PROTHEADECRC32RLEN)


    

//-----------------------------------------------------------
//串口3 环形接收原始缓冲区
#define USART3_RECRAW_RINGBUGLENMAX 1024

//串口3 环形接收分析 缓冲区
#define  USART3_RECPARSE_RINGBUGLENMAX 512

typedef struct _Usart3RingRecBufDoStruct {
    uint8_t  rawBuf[USART3_RECRAW_RINGBUGLENMAX];       //串口3 环形接收缓冲区
    uint16_t rawReadIndex;
    uint16_t rawWriteIndex;
    
    uint8_t  parseBuf[USART3_RECPARSE_RINGBUGLENMAX];   //串口3 环形接收分析缓冲区
    uint16_t parseIndex;
} T_Usart3RingBufDoStruct;



extern T_Usart3RingBufDoStruct    Usart3RingRecBufDoStruct;



//-------------------------------------------------------------------------
//PSDK 格式头定义
//frameheader(格式头系列)=SOF+Length/Version +Session/ACK +Padding/ENC +RES1 +CMD SET +CMD ID +SEQ +CRC16 
//总体数据全局 = frameheader(格式头系列) + framedata(帧数据) + crc32(校验)
//SDK protocol frame header structure
//说明：lenAndVer为uint16_t变量，其中位域length占10位，位域version占6位

/*
    union 
	{
       
        struct 
		{
            uint16_t length     : 10;
            uint16_t version    : 6;
        } lenAndVer;
        uint16_t infoData;
    } lenAndVer;
  

   
    union {
        struct {
            uint8_t ackReq      : 5;
            uint8_t packetType  : 1;
            uint8_t reserved    : 2;
        } cmdType;
        uint8_t infoData;
    } cmdType;
    
    
   
    union {
        struct {
            uint8_t padding     : 5;
            uint8_t encType     : 3;
        } paddingAndEnc;
        uint8_t infoData;
    } paddingAndEnc;
 */
    

typedef struct _PsdkProtFrameAllStruct 
{
    uint8_t sof;
    uint16_t lenVer;
    uint16_t lenVer_len;
    uint16_t lenVer_Ver;
    
    uint8_t ackType;
    uint8_t ackType_Ack;
    uint8_t ackType_Type;
    
    uint8_t paddingEnc;
    uint8_t paddingEnc_padding;
    uint8_t paddingEnc_Enc;
    
    uint8_t reserve;
    
    uint8_t cmdSet;
    uint8_t cmdID;
    uint16_t seqNum;
    uint16_t crc16;
    uint8_t sendData[255];
    uint8_t sendDataLen;
    uint32_t crc32;
} T_PsdkProtFrameAllStruct;


extern uint8_t psdkProtFrameHeaderTable[PSDK_PROTHEADEBEFORERLEN];
extern uint8_t psdkProtFrameCrc16Table[PSDK_PROTHEADECRC16RLEN];

extern uint8_t psdkProtFrameDataTable[PSDK_PROTDATAMAXLEN];

extern uint8_t psdkProtFrameCrc32Table[PSDK_PROTHEADECRC32RLEN];


extern T_PsdkProtFrameAllStruct   PsdkProtFrameAllStruct;



void Bsp_usart3Init(void);

void Bsp_usart3PutChar(uint8_t Dat);
void Bsp_usart3SendArray(uint8_t *array, uint16_t num);
void Bsp_usart3PutStr(uint8_t *str);

uint8_t Bsp_usart3ReadChar(T_Usart3RingBufDoStruct *ringBufStruct,T_PsdkProtFrameAllStruct *FrameAllStruct);


extern uint8_t g_keyTable[];




//--------------------------------------------------------
//乒乓缓存 用来存储gps rtk的信息
extern uint8_t g_GpsRtkProtectFlag;//1=数组1处于保护 2=数组2处于保护 0=平时
extern uint8_t g_gpsRtkPointFlag;//1=当前最新的是数组1  2=当前最新的是数组2  为了数据的透明性
extern uint8_t g_gpsRtkTempTable1[200];
extern uint8_t g_gpsRtkTempTable2[200];
extern uint8_t g_gpsRtkTable1Len;
extern uint8_t g_gpsRtkTable2Len;


#endif




