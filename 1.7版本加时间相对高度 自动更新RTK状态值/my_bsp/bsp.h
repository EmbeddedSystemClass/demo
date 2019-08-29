

#ifndef __BSP__H
#define __BSP__H


//------------------------------------------------------------------------------
//读数据状态检测
#define READ_ENING			(0==PBin(1))


//------------------------------------------------------------------------------
//蜂鸣器
#define FMQIO_OFF    	PAout(11)=0
#define FMQ_ON			cocoguojia_tim1Enable()
#define FMQ_OFF    		cocoguojia_tim1Disable()


//------------------------------------------------------------------------------
//软开关
#define KEY_ENING			(0==PCin(7))

//------------------------------------------------------------------------------
//KEY LED
#define KEY_LED_ON			PAout(8)=0
#define KEY_LED_OFF    		PAout(8)=1



//------------------------------------------------------------------------------
//FAN
#define FAN_ON			    PAout(4)=1
#define FAN_OFF    		    PAout(4)=0


//------------------------------------------------------------------------------
//指示灯
//#define LEDPOINT_ON		PAout(1)=0
//#define LEDPOINT_OFF    	PAout(1)=1



//------------------------------------------------------------------------------
//TTL使能后 管脚状态检测
#define READTTL_IS0			(0==PAin(0))
#define READTTL_IS1			(1==PAin(0))

//------------------------------------------------------------------------------
//RS485 管脚切换
#define RS485_TXD_EN        PBout(4)=1	
#define RS485_RXD_EN    	PBout(4)=0



//读写切换管脚
#define SWITCH_WRITE	 PAout(12)=0
#define SWITCH_READ      PAout(12)=1



extern void BSP_Init(void);


#endif




