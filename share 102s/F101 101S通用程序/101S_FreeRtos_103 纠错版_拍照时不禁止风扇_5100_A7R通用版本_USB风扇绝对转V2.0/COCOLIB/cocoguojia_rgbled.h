

#ifndef __RGBLED_COCOGUOJIA_H__
#define __RGBLED_COCOGUOJIA_H__
#include "myinclude.h"

//------------------------------------------------------------------------------
//��ɫRGB��ɫLED ����
//R
#define LED1_ON			PCout(5)=0
#define LED1_OFF    	PCout(5)=1
//G
#define LED2_ON			PBout(0)=0
#define LED2_OFF    	PBout(0)=1
//B
#define LED3_ON			PBout(1)=0
#define LED3_OFF    	PBout(1)=1



//��������� Ϊ��
#define RGBLEDR_ON				LED2_OFF;	LED3_OFF;	LED1_ON;
//��������� Ϊ��
#define RGBLEDG_ON				LED1_OFF;	LED3_OFF;	LED2_ON;
//��������� Ϊ��
#define RGBLEDB_ON				LED1_OFF;	LED2_OFF;	LED3_ON;
//��+��=��
#define RGBLEDY_ON				LED3_OFF;	LED1_ON;	LED2_ON;
//��+��=��
#define RGBLEDP_ON				LED2_OFF;	LED1_ON;	LED3_ON;
//��+��=��
#define RGBLEDC_ON				LED1_OFF;	LED2_ON;	LED3_ON;
//��+��+��=��
#define RGBLEDW_ON				LED1_ON;	LED2_ON;	LED3_ON;
//�� ȫ��
#define RGBLED_ALLOFF			LED1_OFF;	LED2_OFF;	LED3_OFF;

extern void cocoguojia_rgbledInit(void);



#endif




