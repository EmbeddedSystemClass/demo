#ifndef __BSP__H
#define __BSP__H

/*读数据状态检测*/
#define READ_ENING			(0==PBin(1))


/* 蜂鸣器 */
#define FMQIO_OFF    	PAout(11)=0
//#define FMQ_ON			  cocoguojia_tim1Enable()
//#define FMQ_OFF    		cocoguojia_tim1Disable()
#define FMQ_ON			  	PAout(11)=0
#define FMQ_OFF    			PAout(11)=0

/* 机械开关 */
#define KEY_ENING			(0==PCin(7))


/* KEY LED */
#define KEY_LED_ON			PAout(8)=0
#define KEY_LED_OFF    		PAout(8)=1


/* 风扇 */
#define FAN_ON			    PAout(4)=1
#define FAN_OFF    		    PAout(4)=0



/* 读写切换管脚 */
#define SWITCH_WRITE	   PAout(12)=0
#define SWITCH_READ      PAout(12)=1


/* 拍照 */
#define PHOTO1_EN 		PAout(12)=0                     //待修改
#define PHOTO1_NO 		PAout(12)=0
#define PHOTO2_EN 		PAout(12)=0                     //待修改
#define PHOTO2_NO 		PAout(12)=0
#define PHOTO3_EN 		PAout(12)=0                     //待修改
#define PHOTO3_NO 		PAout(12)=0
#define PHOTO4_EN 		PAout(12)=0                     //待修改
#define PHOTO4_NO 		PAout(12)=0
#define PHOTO5_EN 		PAout(12)=0                     //待修改
#define PHOTO5_NO 		PAout(12)=0

#define PHOTOALL_EN 		PAout(12)=0                     //待修改
#define PHOTOALL_NO 		PAout(12)=0





/* 开关机 */
#define POWER_EN 		PAout(12)=0                     //待修改
#define POWER_NO 		PAout(12)=0


/* OK按键 */
#define OK_EN 			PAout(12)=0                     //待修改
#define OK_NO 			PAout(12)=0  





extern void BSP_Init(void);


#endif




