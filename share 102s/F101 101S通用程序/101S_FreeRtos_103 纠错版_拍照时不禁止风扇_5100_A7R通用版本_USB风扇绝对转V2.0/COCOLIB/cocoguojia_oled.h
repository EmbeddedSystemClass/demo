

#ifndef __OLED_COCOGUOJIA_H__
#define __OLED_COCOGUOJIA_H__

///////////////////////////////////////////////////
//����ͷ�ļ�
#include "myinclude.h"

extern void cocoguojia_oledInit(void);


 
extern void cocoguojia_oledRunShowFor1(uint8_t model,uint8_t offOnFlag);
extern void cocoguojia_oledRunShowFor2(void);
extern void cocoguojia_oledShowShareuavLogoAndID(void);
extern void cocoguojia_oledShowSystemInit(void);

extern void cocoguojia_oledShowShuting(void);
extern void cocoguojia_oledShowOpening(void);


extern void cocoguojia_oledShowShutWrong(void);
extern void cocoguojia_oledShowOpenWrong(void);
extern void cocoguojia_oledShowLoseWrong(void);


extern void cocoguojia_oledShowSetModel1(void);
extern void cocoguojia_oledShowSetModel3(uint8_t curr);

extern void cocoguojia_oledShowRepairModel(uint8_t step);


//OLED 4�ӿ�
//OLED���Դ�
//��Ÿ�ʽ����
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 
//----------------------------------------------------------------
//GND  ��Դ��
//VCC  ��5V��3.3v��Դ
//D0   ��PB9��SCL��
//D1   ��PB8��SDA��
//RES  ��PA15
//DC   ��PA12
//CS   ��PA11              
// ----------------------------------------------------------------


//OLEDģʽ����
//0:4�ߴ���ģʽ
#define OLED_MODE 0
#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	



//-----------------OLED�˿ڶ���----------------  					   
#define OLED_CS_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_15)//CS
#define OLED_CS_Set()  GPIO_SetBits(GPIOA,GPIO_Pin_15)

#define OLED_RST_Clr() GPIO_ResetBits(GPIOC,GPIO_Pin_11)//RES
#define OLED_RST_Set() GPIO_SetBits(GPIOC,GPIO_Pin_11)

#define OLED_DC_Clr() GPIO_ResetBits(GPIOC,GPIO_Pin_10)//DC
#define OLED_DC_Set() GPIO_SetBits(GPIOC,GPIO_Pin_10)


//ʹ��4�ߴ��нӿ�ʱʹ�� 
#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOD,GPIO_Pin_2)//CLK D0
#define OLED_SCLK_Set() GPIO_SetBits(GPIOD,GPIO_Pin_2)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOC,GPIO_Pin_12)//DIN D1
#define OLED_SDIN_Set() GPIO_SetBits(GPIOC,GPIO_Pin_12)




 		     
#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����


//OLED�����ú���
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
	 



