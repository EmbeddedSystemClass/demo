

#ifndef __RGBLED_COCOGUOJIA_H__
#define __RGBLED_COCOGUOJIA_H__
#include "myinclude.h"

//------------------------------------------------------------------------------
//三色RGB彩色LED 定义
//R
#define LED1_ON			PCout(5)=0
#define LED1_OFF    	PCout(5)=1
//G
#define LED2_ON			PBout(0)=0
#define LED2_OFF    	PBout(0)=1
//B
#define LED3_ON			PBout(1)=0
#define LED3_OFF    	PBout(1)=1



//如果单独亮 为红
#define RGBLEDR_ON				LED2_OFF;	LED3_OFF;	LED1_ON;
//如果单独亮 为绿
#define RGBLEDG_ON				LED1_OFF;	LED3_OFF;	LED2_ON;
//如果单独亮 为蓝
#define RGBLEDB_ON				LED1_OFF;	LED2_OFF;	LED3_ON;
//红+绿=黄
#define RGBLEDY_ON				LED3_OFF;	LED1_ON;	LED2_ON;
//红+蓝=紫
#define RGBLEDP_ON				LED2_OFF;	LED1_ON;	LED3_ON;
//绿+蓝=青
#define RGBLEDC_ON				LED1_OFF;	LED2_ON;	LED3_ON;
//红+绿+蓝=白
#define RGBLEDW_ON				LED1_ON;	LED2_ON;	LED3_ON;
//黑 全灭
#define RGBLED_ALLOFF			LED1_OFF;	LED2_OFF;	LED3_OFF;

extern void cocoguojia_rgbledInit(void);



#endif




