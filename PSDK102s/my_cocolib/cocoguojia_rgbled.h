

#ifndef __RGBLED_COCOGUOJIA_H__
#define __RGBLED_COCOGUOJIA_H__
#include "myinclude.h"

//------------------------------------------------------------------------------
//三色RGB彩色LED 定义
//R
#define LEDR_ON			PCout(6)=0
#define LEDR_OFF    	PCout(6)=1
//G
#define LEDG_ON			PBout(15)=0
#define LEDG_OFF    	PBout(15)=1
//B
#define LEDB_ON			PBout(14)=0
#define LEDB_OFF    	PBout(14)=1



//如果单独亮 为红
#define RGBLEDR_ON				LEDG_OFF;	LEDB_OFF;	LEDR_ON;
//如果单独亮 为绿
#define RGBLEDG_ON				LEDR_OFF;	LEDB_OFF;	LEDG_ON;
//如果单独亮 为蓝
#define RGBLEDB_ON				LEDR_OFF;	LEDG_OFF;	LEDB_ON;
//红+绿=黄
#define RGBLEDY_ON				LEDB_OFF;	LEDR_ON;	LEDG_ON;
//红+蓝=紫
#define RGBLEDP_ON				LEDG_OFF;	LEDR_ON;	LEDB_ON;
//绿+蓝=青
#define RGBLEDC_ON				LEDR_OFF;	LEDG_ON;	LEDB_ON;
//红+绿+蓝=白
#define RGBLEDW_ON				LEDR_ON;	LEDG_ON;	LEDB_ON;
//黑 全灭
#define RGBLED_ALLOFF			LEDR_OFF;	LEDG_OFF;	LEDB_OFF;

extern void cocoguojia_rgbledInit(void);



#endif




