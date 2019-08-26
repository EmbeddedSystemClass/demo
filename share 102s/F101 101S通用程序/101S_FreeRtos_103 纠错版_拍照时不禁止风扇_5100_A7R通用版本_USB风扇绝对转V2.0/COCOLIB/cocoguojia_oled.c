 


#include "cocoguojia_oledfont.h"

///////////////////////////////////////////////////
//����ͷ�ļ�
#include "cocoguojia_rgbled.h"

//static void OLED_ShowCharFanBai(u8 x,u8 y,u8 chr);

//��SSD1106д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	u8 i;			  
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();		  
	OLED_CS_Clr();
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK_Clr();
		if(dat&0x80)
		   OLED_SDIN_Set();
		else 
		   OLED_SDIN_Clr();
		OLED_SCLK_Set();
		dat<<=1;   
	}				 		  
	OLED_CS_Set();
	OLED_DC_Set();   	  
} 

	void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD); 
}   	  
//����OLED��ʾ    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x02,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //������ʾ
}


//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//�õ�ƫ�ƺ��ֵ			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(SIZE ==16)
			{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
			}
			else {	
				OLED_Set_Pos(x,y+1);
				for(i=0;i<6;i++)
				OLED_WR_Byte(F6x8[c][i],OLED_DATA);
				
			}
}

//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 16/12 
static void OLED_ShowCharFanBai(u8 x,u8 y,u8 chr)
{      	
	unsigned char c=0,i=0;	
	c=chr-' ';//�õ�ƫ�ƺ��ֵ			
	if(x>Max_Column-1){x=0;y=y+2;}
	if(SIZE ==16)
	{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_WR_Byte(~F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_Byte(~F8X16[c*16+i+8],OLED_DATA);
	}
	else 
	{	
		OLED_Set_Pos(x,y+1);
		for(i=0;i<6;i++)
		OLED_WR_Byte(~F6x8[c][i],OLED_DATA);			
	}
}


//m^n����
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//��ʾ2������
//x,y :�������	 
//num:��ֵ(0~4294967295);
//len :���ֵ�λ��
//size:�����С16 
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ');
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0'); 
	}
} 
//��ʾһ���ַ��Ŵ�
void OLED_ShowString(u8 x,u8 y,u8 *chr)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j]);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}

//��ʾһ���ַ��Ŵ�
void OLED_ShowStringFanBai(u8 x,u8 y,u8 *chr)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowCharFanBai(x,y,chr[j]);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}


//��ʾ����
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
				adder+=1;
      }					
}


//��ʾ����
void OLED_ShowCHinese2(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(Hzk2[2*no][t],OLED_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_Byte(Hzk2[2*no+1][t],OLED_DATA);
				adder+=1;
      }					
}

//������ʾ����
void OLED_ShowCHinese2FanBai(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(~Hzk2[2*no][t],OLED_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_Byte(~Hzk2[2*no+1][t],OLED_DATA);
				adder+=1;
      }					
}



/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
	    }
	}
} 





//####################################################################################################################

static void delay_oled(void);
//----------------------------------------------------------------------------------------------------------
//��ʼ��SSD1306					    
void cocoguojia_oledInit(void)
{ 	 
 	GPIO_InitTypeDef  GPIO_InitStructure;
 

	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);	 //ʹ��PC,D,G�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//�ٶ�50MHz
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	  				//��ʼ��GPIOD3,6
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;	 //PD3,PD6�������  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//�ٶ�50MHz
 	GPIO_Init(GPIOC, &GPIO_InitStructure);	  				//��ʼ��GPIOD3,6
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	 //PD3,PD6�������  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//�ٶ�50MHz
 	GPIO_Init(GPIOD, &GPIO_InitStructure);	  				//��ʼ��GPIOD3,6
	
 	GPIO_SetBits(GPIOA,GPIO_Pin_15);	
	GPIO_SetBits(GPIOC,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);
	GPIO_SetBits(GPIOD,GPIO_Pin_2);
	


    OLED_RST_Set();
	delay_oled();
	OLED_RST_Clr();
	delay_oled();
	OLED_RST_Set(); 
					  
	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WR_Byte(0x02,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WR_Byte(0x81,OLED_CMD); //�Աȶ�����
	OLED_WR_Byte(0xfF,OLED_CMD); //1~255;Ĭ��0X7F (��������,Խ��Խ��)
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);//
	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
	
	OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/ 
	OLED_Clear();
	OLED_Set_Pos(0,0); 	
} 


void cocoguojia_oledShowShareuavLogoAndID(void)
{
	uint32_t g_ChipUniqueID[3];
	
	//--------------------------------------------------------------------------
	//��оƬΨһID
	//g_ChipUniqueID[0] = *(__IO u32 *)(0X1FFFF7F0); // ���ֽ�
	//g_ChipUniqueID[1] = *(__IO u32 *)(0X1FFFF7EC); // 
	g_ChipUniqueID[2] = *(__IO u32 *)(0X1FFFF7E8); // ���ֽ�  bcd 8���ֽ�
	
    OLED_ShowCHinese(0,0,1);		//��
	OLED_ShowCHinese(18,0,2);		//��
	OLED_ShowCHinese(36,0,3);		//��
	OLED_ShowCHinese(54,0,4);		//��
	OLED_ShowCHinese(72,0,5);		//��   
    OLED_ShowString(95,0,"V2.0");
    
	OLED_DrawBMP(48,2,80,7,BMP3); 
    

	//--------------------------------------
	OLED_ShowString(5,6,"ID:");
	OLED_ShowNum(30,6,g_ChipUniqueID[2],9+1,16);
}



void cocoguojia_oledShowSystemInit(void)
{
	static uint8_t i=0;
	
	OLED_ShowCHinese2(0,2,13);		//ϵ
	OLED_ShowCHinese2(18,2,14);		//ͳ
	OLED_ShowCHinese2(36,2,15);		//��
	OLED_ShowCHinese2(54,2,16);		//ʼ
	OLED_ShowCHinese2(72,2,17);		//��
	if(0==i)
	{
		OLED_ShowString(90,2,".  ");
		i++;		
	}
	else if(1==i)
	{
		OLED_ShowString(90,2,".. ");
		i++;		
	}
	else if(2==i)
	{
		OLED_ShowString(90,2,"...");
		i=0;		
	}
	else
	{
		i=0;	
	}
}

	


void cocoguojia_oledShowShuting(void)
{
	static uint8_t i=0;
	
	OLED_ShowCHinese2(0,2,6);		//��
	OLED_ShowCHinese2(18,2,1);		//��
	OLED_ShowCHinese2(36,2,5);		//��
	
	
	if(1==g_readingFlag)
	{	
		OLED_ShowString(56,2,"USB");
		if(0==i)
		{
			OLED_ShowString(82,2,".  ");
			i++;		
		}
		else if(1==i)
		{
			OLED_ShowString(82,2,".. ");
			i++;		
		}
		else if(2==i)
		{
			OLED_ShowString(82,2,"...");
			i=0;
		}
		else
		{
			i=0;	
		}		
	}
	else
	{
		if(0==i)
		{
			OLED_ShowString(54,2,".       ");
			i++;		
		}
		else if(1==i)
		{
			OLED_ShowString(54,2,"..      ");
			i++;		
		}
		else if(2==i)
		{
			OLED_ShowString(54,2,"...     ");
			i=0;
		}
		else
		{
			i=0;	
		}
	}
}

void cocoguojia_oledShowOpening(void)
{
	static uint8_t i=0;
		
	OLED_ShowCHinese2(0,2,2);		//��
	OLED_ShowCHinese2(18,2,1);		//��
	OLED_ShowCHinese2(36,2,5);		//��
	
	if(1==g_readingFlag)
	{	
		OLED_ShowString(56,2,"USB");
		if(0==i)
		{
			OLED_ShowString(82,2,".  ");
			i++;		
		}
		else if(1==i)
		{
			OLED_ShowString(82,2,".. ");
			i++;		
		}
		else if(2==i)
		{
			OLED_ShowString(82,2,"...");
			i=0;
		}
		else
		{
			i=0;	
		}		
	}
	else
	{
		if(0==i)
		{
			OLED_ShowString(54,2,".       ");
			i++;		
		}
		else if(1==i)
		{
			OLED_ShowString(54,2,"..      ");
			i++;		
		}
		else if(2==i)
		{
			OLED_ShowString(54,2,"...     ");
			i=0;
		}
		else
		{
			i=0;	
		}
	}
}

//---------------------------------------------------------------------
//��������ʱ��ʾ ��һҳ
//model=1ʱ Ҫ��ʾ������PWM��
void cocoguojia_oledRunShowFor1(uint8_t model,uint8_t offOnFlag)
{
	static uint8_t k=0;
	uint8_t i;
	unsigned short tim5PlauseUsTemp=0;
	uint16_t tim5CatchNumTemp;
	
	//-------------------------------------------------
	//1-1
	if(0==offOnFlag)
	{
		OLED_ShowCHinese2(0,0,6);		//��
		OLED_ShowCHinese2(18,0,1);		//��   
	}
	else
	{
		OLED_ShowCHinese2(0,0,2);		//��
		OLED_ShowCHinese2(18,0,1);		//��
	}	
    
	if(1==g_readingFlag)
	{
		if(0==k)
		{
			OLED_ShowString(38,0,"USB.     ");	
		}
		else if(1==k)
		{
			OLED_ShowString(38,0,"USB..    ");	
		}
		else
		{
			OLED_ShowString(38,0,"USB...   ");	
		}
		
		k++;
		if(3==k)
		{
			k=0;
		}
	}
	else
	{
		k=0;
		OLED_ShowString(38,0,"      ");	
	}
    
    if(0x8000<=g_ds18b20Temp)
    {
        OLED_ShowString(88,0,"-");
        OLED_ShowNum(96,0,(g_ds18b20Temp&(~0x8000)),2,16);
        if(READ_NOING){FAN_OFF;} 
    }
    else
    { 
         OLED_ShowNum(96,0,g_ds18b20Temp,2,16);			
			 //-------------------------------------------
			 //����¶ȴ������ֵ
			 if(TEMPEREATURE_MAX<=g_ds18b20Temp)
			 { 
					FAN_ON;
			 }
			 else
			 {
					FAN_OFF;  
			 }                 
    }
    OLED_ShowCHinese2(112,0,20);		//��C
    		
	//-------------------------------------------------
	//1-2
	if(0==g_pwmOrTtlModelFlag)
	{
		OLED_ShowString(0,2,"PWM:      ");	
		if(1==model)
		{
			tim5PlauseUsTemp=g_tim5TriggerPlauseUs;
		}
		else
		{		
			tim5PlauseUsTemp=g_tim5PlauseUs;
		}
		if(40000<=tim5PlauseUsTemp)
		{
			tim5PlauseUsTemp=40000;
		}	
		if(10000<=tim5PlauseUsTemp)
		{
			i=5;
		}	
		if(1000<=tim5PlauseUsTemp)
		{
			i=4;
		}
		else if(100<=tim5PlauseUsTemp)
		{
			i=3;
		}
		else if(10<=tim5PlauseUsTemp)
		{
			i=2;
		}
		else 
		{
			i=1;
		}		
		OLED_ShowNum(41,2,tim5PlauseUsTemp,i+1,16);	
	}	
	else
	{
		OLED_ShowString(0,2,"TTL:      ");	
		if(1==model)
		{
			tim5PlauseUsTemp=g_ttlTriggerNum;
		}
		else
		{		
			tim5PlauseUsTemp=g_ttlCurrentNum;
		}
		if(40000<=tim5PlauseUsTemp)
		{
			tim5PlauseUsTemp=40000;
		}	
		if(10000<=tim5PlauseUsTemp)
		{
			i=5;
		}	
		if(1000<=tim5PlauseUsTemp)
		{
			i=4;
		}
		else if(100<=tim5PlauseUsTemp)
		{
			i=3;
		}
		else if(10<=tim5PlauseUsTemp)
		{
			i=2;
		}
		else 
		{
			i=1;
		}		
		OLED_ShowNum(41,2,tim5PlauseUsTemp,i+1,16);	
	}
	
	
	//-------------------------------------------------
	//1-3	
	OLED_ShowString(0,4,"P:       ");	 
	tim5CatchNumTemp=g_tim5CatchNum;
	if(10000<=tim5CatchNumTemp)
	{
		i=5;
	}	
	if(1000<=tim5CatchNumTemp)
	{
		i=4;
	}
	else if(100<=tim5CatchNumTemp)
	{
		i=3;
	}
	else if(10<=tim5CatchNumTemp)
	{
		i=2;
	}
	else 
	{
		i=1;
	}		
	OLED_ShowNum(41,4,tim5CatchNumTemp,i+1,16);	
	
	//-------------------------------------------------
	//1-4
	OLED_ShowString(0,6,"PIC1:      ");//һ������д16���ַ�
	if(10000<=g_photoPicNum[1])
	{
		i=5;
	}	
	if(1000<=g_photoPicNum[1])
	{
		i=4;
	}
	else if(100<=g_photoPicNum[1])
	{
		i=3;
	}
	else if(10<=g_photoPicNum[1])
	{
		i=2;
	}
	else 
	{
		i=1;
	}		
	OLED_ShowNum(41,6,g_photoPicNum[1],i+1,16);
}


//---------------------------------------------------------------------
//��������ʱ��ʾ �ڶ�ҳ
void cocoguojia_oledRunShowFor2(void)
{
	uint8_t i;
	
	OLED_ShowString(0,0,"PIC2:           ");//һ������д16���ַ�

    //--------------------------------------------------------------
	//2-1
	if(10000<=g_photoPicNum[2])
	{
		i=5;
	}	
	if(1000<=g_photoPicNum[2])
	{
		i=4;
	}
	else if(100<=g_photoPicNum[2])
	{
		i=3;
	}
	else if(10<=g_photoPicNum[2])
	{
		i=2;
	}
	else 
	{
		i=1;
	}		
	OLED_ShowNum(41,0,g_photoPicNum[2],i+1,16);
	
	//--------------------------------------------------------------
	//2-2
	OLED_ShowString(0,2,"PIC3:      ");//һ������д16���ַ�
	if(10000<=g_photoPicNum[3])
	{
		i=5;
	}	
	if(1000<=g_photoPicNum[3])
	{
		i=4;
	}
	else if(100<=g_photoPicNum[3])
	{
		i=3;
	}
	else if(10<=g_photoPicNum[3])
	{
		i=2;
	}
	else 
	{
		i=1;
	}		
	OLED_ShowNum(41,2,g_photoPicNum[3],i+1,16);	
	
	//--------------------------------------------------------------
	//2-3	
	OLED_ShowString(0,4,"PIC4:      ");//һ������д16���ַ�
	if(10000<=g_photoPicNum[4])
	{
		i=5;
	}	
	if(1000<=g_photoPicNum[4])
	{
		i=4;
	}
	else if(100<=g_photoPicNum[4])
	{
		i=3;
	}
	else if(10<=g_photoPicNum[4])
	{
		i=2;
	}
	else 
	{
		i=1;
	}		
	OLED_ShowNum(41,4,g_photoPicNum[4],i+1,16);	
	
	//--------------------------------------------------------------
	//2-4
	OLED_ShowString(0,6,"PIC5:       ");//һ������д16���ַ�
	if(10000<=g_photoPicNum[5])
	{
		i=5;
	}	
	if(1000<=g_photoPicNum[5])
	{
		i=4;
	}
	else if(100<=g_photoPicNum[5])
	{
		i=3;
	}
	else if(10<=g_photoPicNum[5])
	{
		i=2;
	}
	else 
	{
		i=1;
	}		
	OLED_ShowNum(41,6,g_photoPicNum[5],i+1,16);	
}

void cocoguojia_oledShowShutWrong(void)
{
	uint8_t n;
	if(1==g_readingFlag)
	{	
		OLED_ShowString(0,0,"USB");		
	}
	else
	{	
		OLED_ShowString(0,0,"   ");		
	}
	
	OLED_ShowCHinese2(0,2,6);		//��
	OLED_ShowCHinese2(18,2,1);		//��	
	OLED_ShowCHinese2(36,2,5);		//��
	OLED_ShowString(0,4,"WRONG:  ");//һ������д16���ַ�
	for(n=1;n<=5;n++)
	{
		if(1==g_camPowerFlagTable[n])
		{
			OLED_ShowChar((n-1)*16,6,'0'+n);
		}
		else
		{
			OLED_ShowChar((n-1)*16,6,' ');
		}	
	}
}

void cocoguojia_oledShowOpenWrong(void)
{
	uint8_t n;
	if(1==g_readingFlag)
	{	
		OLED_ShowString(0,0,"USB");		
	}
	else
	{	
		OLED_ShowString(0,0,"   ");		
	}
	
	OLED_ShowCHinese2(0,2,2);		//��
	OLED_ShowCHinese2(18,2,1);		//��
	OLED_ShowCHinese2(36,2,5);		//��
	OLED_ShowString(0,4,"WRONG  ");//һ������д16���ַ�
	for(n=1;n<=5;n++)
	{
		if(0==g_camPowerFlagTable[n])
		{
			OLED_ShowChar((n-1)*16,6,'0'+n);
		}
		else
		{
			OLED_ShowChar((n-1)*16,6,' ');
		}	
	}
}

//------------------------------------------------
//���ȱʧ ���� ��һ�°����������˴�����
void cocoguojia_oledShowLoseWrong(void)
{
	uint8_t n;
	if(1==g_readingFlag)
	{	
		OLED_ShowString(0,0,"USB");		
	}
	else
	{	
		OLED_ShowString(0,0,"   ");		
	}
	
	OLED_ShowCHinese2(0,2,0);		//��
	OLED_ShowCHinese2(18,2,1);		//��	
	OLED_ShowCHinese2(36,2,18);		//ȱ
    OLED_ShowCHinese2(54,2,19);		//ʧ
	OLED_ShowString(0,4,"LOSE:  "); //һ������д16���ַ�
	for(n=1;n<=5;n++)
	{
		if(0==g_camLoseFlagTable[n])
		{
			OLED_ShowChar((n-1)*16,6,'0'+n);
		}
		else
		{
			OLED_ShowChar((n-1)*16,6,' ');
		}	
	}
}

//-----------------------------------------------------------------------------------
void cocoguojia_oledShowSetModel1(void)
{
    if(0!=g_currentRowNum)
    {
        OLED_ShowCHinese2FanBai(0,0,7);		    //��
        OLED_ShowCHinese2FanBai(18,0,8);		//��
        OLED_ShowCHinese2FanBai(36,0,9);		//ģ
        OLED_ShowCHinese2FanBai(54,0,10);		//ʽ
        
        OLED_ShowCHinese2(92,0,21);		//��
        OLED_ShowCHinese2(110,0,22);    //��
    }
    else //if(0==g_currentRowNum) �����޸�ģʽ ����ģʽ
    {
        OLED_ShowCHinese2(0,0,7);		//��
        OLED_ShowCHinese2(18,0,8);		//��
        OLED_ShowCHinese2(36,0,9);		//ģ
        OLED_ShowCHinese2(54,0,10);		//ʽ
        
        OLED_ShowCHinese2FanBai(92,0,21);	  //��
        OLED_ShowCHinese2FanBai(110,0,22);    //��
    }    
        
	
	if(2==g_currentRowNum)
	{
		OLED_ShowStringFanBai(0,2,"PWM  ");//һ������д16���ַ�
	}
	else
	{
		OLED_ShowString(0,2,"PWM  ");//һ������д16���ַ�
	}
	
	if(4==g_currentRowNum)
	{
		OLED_ShowStringFanBai(0,4,"TTL-H ");//һ������д16���ַ�
	}
	else
	{
		OLED_ShowString(0,4,"TTL-H  ");//һ������д16���ַ�
	}

	if(6==g_currentRowNum)
	{
		OLED_ShowStringFanBai(0,6,"TTL-L  ");//һ������д16���ַ�
	}
	else
	{
		OLED_ShowString(0,6,"TTL-L  ");//һ������д16���ַ�
	}	    
}


//--------------------------------------------------------
//curr=1 ��ʾ��ǰ����  curr=0 ��ʾ���ý��
void cocoguojia_oledShowSetModel3(uint8_t curr)
{
	if(1==curr)
	{
		OLED_ShowCHinese2(18,0,11);		//��
		OLED_ShowCHinese2(36,0,12);		//ǰ
		OLED_ShowCHinese2(54,0,7);		//��
		OLED_ShowCHinese2(72,0,8);		//��
	}
	else
	{	
		OLED_ShowCHinese2(18,0,7);		//��
		OLED_ShowCHinese2(36,0,8);		//��
		OLED_ShowCHinese2(54,0,3);		//��
		OLED_ShowCHinese2(72,0,4);		//��
	}
	
	if(0==g_pwmOrTtlModelFlag)
	{
		OLED_ShowStringFanBai(0,2,"PWM  ");//һ������д16���ַ�
	}
	else
	{
		if(1==g_pwmOrTtlModelFlag)
		{
			OLED_ShowStringFanBai(0,2,"TTL-H  ");//һ������д16���ַ�
		}
		else
		{
			OLED_ShowStringFanBai(0,2,"TTL-L  ");//һ������д16���ַ�
		}
	}	
}

//--------------------------------------------------------
void cocoguojia_oledShowRepairModel(uint8_t step)
{
     static uint8_t i=0;
	 uint8_t n;
     OLED_ShowCHinese2(18,0,21);		//��
     OLED_ShowCHinese2(36,0,22);		//��
     OLED_ShowCHinese2(54,0,9);		    //ģ
     OLED_ShowCHinese2(72,0,10);		//ʽ  
    
    if(3>=step)
    {
        for(n=1;n<=5;n++)
        {
            if(1==g_camPowerFlagTable[n])
            {
                OLED_ShowChar(40+(n-1)*12,2,'0'+n);
                OLED_ShowChar(40+(n-1)*12,4,' ');
            }
            else
            {
                OLED_ShowChar(40+(n-1)*12,4,'0'+n);
                OLED_ShowChar(40+(n-1)*12,2,' ');
            }	
        }
    }
        
      if(1==step)
      {
        OLED_ShowCHinese2FanBai(0,2,2);		//��
		OLED_ShowCHinese2FanBai(18,2,1);	//��
		OLED_ShowCHinese2(0,4,6);		    //��
		OLED_ShowCHinese2(18,4,1);          //�� 
        //������...
        OLED_ShowCHinese2(0,6,2);		//��
        OLED_ShowCHinese2(18,6,1);		//��
        OLED_ShowCHinese2(36,6,5);		//�� 
        if(0==i)
		{
			OLED_ShowString(54,6,".       ");
			i++;		
		}
		else if(1==i)
		{
			OLED_ShowString(54,6,"..      ");
			i++;		
		}
		else if(2==i)
		{
			OLED_ShowString(54,6,"...     ");
			i=0;
		}
		else
		{
			i=0;	
		}
      }
      else if(2==step)
      {
        OLED_ShowCHinese2(0,2,2);		    //��
		OLED_ShowCHinese2(18,2,1);	        //��
		OLED_ShowCHinese2(0,4,6);		    //��
		OLED_ShowCHinese2(18,4,1);          //��
        //�̰�=OK ����=�ػ�
        OLED_ShowCHinese2(0,6,24);		//��
        OLED_ShowCHinese2(16,6,25);		//��
        OLED_ShowString(32,6,"=OK");  
        OLED_ShowCHinese2(56,6,23);		//��
        OLED_ShowCHinese2(72,6,25);		//��
        OLED_ShowString(88,6,"=NEXT");          
      } 
      else if(3==step)
      {
        OLED_ShowCHinese2(0,2,2);		    //��
		OLED_ShowCHinese2(18,2,1);	        //��
		OLED_ShowCHinese2FanBai(0,4,6);		//��
		OLED_ShowCHinese2FanBai(18,4,1);    //�� 
        //�ػ���... 
        OLED_ShowCHinese2(0,6,6);		//��
        OLED_ShowCHinese2(18,6,1);		//��
        OLED_ShowCHinese2(36,6,5);		//��
        if(0==i)
		{
			OLED_ShowString(54,6,".       ");
			i++;		
		}
		else if(1==i)
		{
			OLED_ShowString(54,6,"..      ");
			i++;		
		}
		else if(2==i)
		{
			OLED_ShowString(54,6,"...     ");
			i=0;
		}
		else
		{
			i=0;	
		}          
      } 
      else if(4==step)
      {
         //�������豸��Դ...
         OLED_ShowCHinese2(0,2,26);		    //��          
         OLED_ShowCHinese2(18,2,27);		//��
         OLED_ShowCHinese2(36,2,28);		//��
         OLED_ShowCHinese2(54,2,29);		//��  
         OLED_ShowCHinese2(72,2,30);		//��
         OLED_ShowCHinese2(90,2,31);		//��
         OLED_ShowCHinese2(108,2,32);		//��
         if(0==i)
		 {
			OLED_ShowString(0,4,".       ");
			i++;		
		 }
         else if(1==i)
        {
                OLED_ShowString(0,4,"..      ");
                i++;		
         }
        else if(2==i)
        {
            OLED_ShowString(0,4,"...     ");
            i=0;
        }
        else
        {
            i=0;	
        }           
      
      } 
      else
      {
            ;
      }      
}


static void delay_oled(void)
{
	uint32_t i=10000000;
	while(i--)
	{
		;
	}
}













