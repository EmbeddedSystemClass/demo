

#ifndef __OLED_COCOGUOJIA_H__
#define __OLED_COCOGUOJIA_H__

///////////////////////////////////////////////////
//自身头文件
#include "myinclude.h"

extern void cocoguojia_oledInit(void);
extern u8 SYS_VER_NUB[5];

 
extern void cocoguojia_oledRunShowFor1(uint8_t offOnFlag);
extern void cocoguojia_oledRunShowFor2(void);
extern void cocoguojia_oledShowShareuavLogoAndID(void);
extern void cocoguojia_oledShowSystemInit(void);

extern void cocoguojia_oledShowShuting(void);
extern void cocoguojia_oledShowOpening(void);


extern void cocoguojia_oledShowShutWrong(void);
extern void cocoguojia_oledShowOpenWrong(void);
extern void cocoguojia_oledShowLoseWrong(void);


extern void cocoguojia_oledShowSetModel1(void);
extern void cocoguojia_oledShowSetModel2(uint8_t curr);
//extern void cocoguojia_oledShowSetModel22(void);

extern void cocoguojia_oledShowRepairModel(uint8_t step);


//OLED 4接口
//OLED的显存
//存放格式如下
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 
//----------------------------------------------------------------
//GND  电源地
//VCC  接5V或3.3v电源
//D0   接PB9（SCL）
//D1   接PB8（SDA）
//RES  接PA15
//DC   接PA12
//CS   接PA11              
// ----------------------------------------------------------------


//OLED模式设置
//0:4线串行模式
#define OLED_MODE 0
#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	



//-----------------OLED端口定义----------------  					   
#define OLED_CS_Clr()  GPIO_ResetBits(GPIOB,GPIO_Pin_5)//CS
#define OLED_CS_Set()  GPIO_SetBits(GPIOB,GPIO_Pin_5)

#define OLED_RST_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_7)//RES
#define OLED_RST_Set() GPIO_SetBits(GPIOB,GPIO_Pin_7)

#define OLED_DC_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_6)//DC
#define OLED_DC_Set() GPIO_SetBits(GPIOB,GPIO_Pin_6)


//使用4线串行接口时使用 
#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_9)//CLK D0
#define OLED_SCLK_Set() GPIO_SetBits(GPIOB,GPIO_Pin_9)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_8)//DIN D1
#define OLED_SDIN_Set() GPIO_SetBits(GPIOB,GPIO_Pin_8)





 		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据


//OLED控制用函数
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y, u8 *p);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);


void OLED_ShowCHinese2(u8 x,u8 y,u8 no);
#endif  
	 



