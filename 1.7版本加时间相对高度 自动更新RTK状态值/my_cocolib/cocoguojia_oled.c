 
///////////////////////////////////////////////////
//自身头文件
#include "cocoguojia_oled.h"
#include "cocoguojia_oledfont.h"



extern uint16_t g_ds18b20ToShow;     //ds18b20 温度值 最高位位1则是负数


//向SSD1106写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
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


//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x02,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //更新显示
}


//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//得到偏移后的值			
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

//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
static void OLED_ShowCharFanBai(u8 x,u8 y,u8 chr)
{      	
	unsigned char c=0,i=0;	
	c=chr-' ';//得到偏移后的值			
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


//m^n函数
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//显示2个数字
//x,y :起点坐标	 
//num:数值(0~4294967295);
//len :数字的位数
//size:字体大小16 
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
//显示一个字符号串
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

//显示一个字符号串
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


//显示汉字
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


//显示汉字
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

//反白显示汉字
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



/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
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
static void delay_oled(void);//静态 延时函数声明

//----------------------------------------------------------------------------------------------------------
//初始化SSD1306					    
void cocoguojia_oledInit(void)
{ 	 
 	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PC,D,G端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//速度50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  				//初始化GPIOD3,6
	
	GPIO_SetBits(GPIOB,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9);

    OLED_RST_Set();
	delay_oled();
	//osDelay (10);
	OLED_RST_Clr();
	delay_oled();
	//osDelay (10);
	OLED_RST_Set(); 
					  
	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WR_Byte(0x02,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WR_Byte(0x81,OLED_CMD);//对比度设置
	OLED_WR_Byte(0xfF,OLED_CMD); //1~255;默认0X7F (亮度设置,越大越亮)
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

//----------------------------------------------------------------------------------------------------------
//OLED 显示LOGO和ID
void cocoguojia_oledShowShareuavLogoAndID(void)
{
	uint32_t g_ChipUniqueID[3];
	
	//--------------------------------------------------------------------------
	//读芯片唯一ID
	//g_ChipUniqueID[0] = *(__IO u32 *)(0X1FFFF7F0); // 高字节
	//g_ChipUniqueID[1] = *(__IO u32 *)(0X1FFFF7EC); // 
	g_ChipUniqueID[2] = *(__IO u32 *)(0X1FFFF7E8); // 低字节  BCD 8个字节
	
    OLED_ShowCHinese(0,0,1);		//赛
	OLED_ShowCHinese(18,0,2);		//尔
	OLED_ShowCHinese(36,0,3);		//无
	OLED_ShowCHinese(54,0,4);		//人
	OLED_ShowCHinese(72,0,5);		//机   
    OLED_ShowString(95,0,"V1.7");   //cocoguojia banben
	OLED_DrawBMP(48,2,80,7,BMP3); 
	//--------------------------------------
	OLED_ShowString(5,6,"ID:");
	OLED_ShowNum(30,6,g_ChipUniqueID[2],9+1,16);
}

//----------------------------------------------------------------------------------------------------------
//OLED 显示系统初始化
void cocoguojia_oledShowSystemInit(void)
{
	static uint8_t i=0;
	
	OLED_ShowCHinese2(0,2,13);		//系
	OLED_ShowCHinese2(18,2,14);		//统
	OLED_ShowCHinese2(36,2,15);		//初
	OLED_ShowCHinese2(54,2,16);		//始
	OLED_ShowCHinese2(72,2,17);		//化
    
    //------------------------------------------------------------------
    //三个小点依次推进效果 结合大循环里的延时 以大循环里的延时为单位
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

//----------------------------------------------------------------------------------------------------------
//OLED 显示关机中
void cocoguojia_oledShowShuting(void)
{
	static uint8_t i=0;
	
	OLED_ShowCHinese2(0,2,6);		//关
	OLED_ShowCHinese2(18,2,1);		//机
	OLED_ShowCHinese2(36,2,5);		//中
	
	//-------------------------------------------
    //如果读取USB中
	if(1==g_readingFlag)
	{	
        //显示 USB...
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
        //显示...
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

//----------------------------------------------------------------------------------------------------------
//OLED 显示开机中
void cocoguojia_oledShowOpening(void)
{
	static uint8_t i=0;
		
	OLED_ShowCHinese2(0,2,2);		//开
	OLED_ShowCHinese2(18,2,1);		//机
	OLED_ShowCHinese2(36,2,5);		//中
	
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
//正常运行时显示 第一页
void cocoguojia_oledRunShowFor1(uint8_t offOnFlag)
{
	static uint8_t k=0;
	uint8_t i;
	uint16_t tim5CatchNumTemp;
	
	//-------------------------------------------------
	//1-1 第一页第一行显示
	if(0==offOnFlag)
	{
		OLED_ShowCHinese2(0,0,6);		//关
		OLED_ShowCHinese2(18,0,1);		//机   
	}
	else
	{
        if(1==g_openCamStateFlag)
        {
            OLED_ShowCHinese2(0,0,33);		//待
            OLED_ShowCHinese2(18,0,2);		//开
        }
        else if(2==g_openCamStateFlag)
        {
            OLED_ShowCHinese2(0,0,2);		//开
            OLED_ShowCHinese2(18,0,1);		//机
        }
        else if(3==g_openCamStateFlag)
        {
            OLED_ShowCHinese2(0,0,33);		//待
            OLED_ShowCHinese2(18,0,6);		//关
        }
        else
        {
            ;
        }
            
	}	
    
    //-------------------------------------------------
	//如果读取USB中 则显示 USB...
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
	else//否则 显示空
	{
		k=0;
		OLED_ShowString(38,0,"      ");	
	}
    
    //-----------------------------------------------------------------
    //DS18B20 显示
    if(0x8000<=g_ds18b20Temp)//温度为负数
    {
        OLED_ShowNum(96,0,(g_ds18b20Temp&(~0x8000)),2,16);
        g_fanDs18b20Flag=0;
        
        g_ds18b20ToShow=g_ds18b20Temp+0x8000;
    }
    else//温度为正数
    {      
        OLED_ShowNum(96,0,g_ds18b20Temp,2,16);
        g_ds18b20ToShow=g_ds18b20Temp;
             //-------------------------------------------
             //如果温度大于最大值
             if(TEMPEREATURE_MAX<=g_ds18b20Temp)
             { 
                g_fanDs18b20Flag=1;
             }
             else
             {
                g_fanDs18b20Flag=0;  
             }   
    }
    OLED_ShowCHinese2(112,0,20);		//显示 °C
    		
	//-------------------------------------------------
	//1-2 第一页第二行显示 g_ppsRtkHealthNum
    if(2==g_signalOrAutoModelFlag)
    {
        OLED_ShowString(0,2,"SIGNAL  RTK=    ");	
    }	
    else if(4==g_signalOrAutoModelFlag)
    {
        OLED_ShowString(0,2,"AUTO=2s RTK=    ");	
    }
    else
    {
        OLED_ShowString(0,2,"AUTO=3s RTK=    ");	
    }
    OLED_ShowNum(98,2,g_ppsRtkHealthNum,2,16);	
  		
	//-------------------------------------------------
	//1-3 第一页第三行显示	
	OLED_ShowString(0,4,"P:              ");	 
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
	//1-4 第一页第四行显示
	OLED_ShowString(0,6,"PIC1:           ");//一共可以写16个字符
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
//正常运行时显示 第二页
void cocoguojia_oledRunShowFor2(void)
{
	uint8_t i;
	
	OLED_ShowString(0,0,"PIC2:            ");//一共可以写16个字符

    //--------------------------------------------------------------
	//2-1 第二页第一行显示
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
	//2-2 第二页第二行显示
	OLED_ShowString(0,2,"PIC3:           ");//一共可以写16个字符
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
	//2-3 第二页第三行显示
	OLED_ShowString(0,4,"PIC4:         ");//一共可以写16个字符
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
	//2-4 第二页第四行显示
	OLED_ShowString(0,6,"PIC5:           ");//一共可以写16个字符
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

//---------------------------------------------------------------------
//关机出错时显示
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
	
	OLED_ShowCHinese2(0,2,6);		//关
	OLED_ShowCHinese2(18,2,1);		//机	
	OLED_ShowCHinese2(36,2,5);		//中
	OLED_ShowString(0,4,"WRONG:  ");//一共可以写16个字符
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

//---------------------------------------------------------------------
//开机出错时显示
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
	
	OLED_ShowCHinese2(0,2,2);		//开
	OLED_ShowCHinese2(18,2,1);		//机
	OLED_ShowCHinese2(36,2,5);		//中
	OLED_ShowString(0,4,"WRONG  ");//一共可以写16个字符
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

//-------------------------------------------------------------------------------------
//相机缺失 报错 按一下按键则跳过此处报错
//上电后检测一次是否缺失 如果不缺失 之后的逻辑都是建立在不缺失的基础上的
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
	
    //------------------------------------------------
	OLED_ShowCHinese2(0,2,0);		//相
	OLED_ShowCHinese2(18,2,1);		//机	
	OLED_ShowCHinese2(36,2,18);		//缺
    OLED_ShowCHinese2(54,2,19);		//失
	OLED_ShowString(0,4,"LOSE:  "); //一共可以写16个字符
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
//设置模式时候 显示第一页
void cocoguojia_oledShowSetModel1(void)
{
    if(0!=g_currentRowNum)
    {
        OLED_ShowCHinese2FanBai(0,0,7);		    //设
        OLED_ShowCHinese2FanBai(18,0,8);		//置
        OLED_ShowCHinese2FanBai(36,0,9);		//模
        OLED_ShowCHinese2FanBai(54,0,10);		//式
        
        OLED_ShowCHinese2(92,0,21);		//修
        OLED_ShowCHinese2(110,0,22);    //复
    }
    else //if(0==g_currentRowNum) 进入修复模式 特殊模式
    {
        OLED_ShowCHinese2(0,0,7);		//设
        OLED_ShowCHinese2(18,0,8);		//置
        OLED_ShowCHinese2(36,0,9);		//模
        OLED_ShowCHinese2(54,0,10);		//式
        
        OLED_ShowCHinese2FanBai(92,0,21);	  //修
        OLED_ShowCHinese2FanBai(110,0,22);    //复
    }    
        
	
	if(2==g_currentRowNum)
	{
		OLED_ShowStringFanBai(0,2,"SIGNAL   ");//一共可以写16个字符
	}
	else
	{
		OLED_ShowString(0,2,"SIGANL   ");//一共可以写16个字符
	}
	
	if(4==g_currentRowNum)
	{
		OLED_ShowStringFanBai(0,4,"AUTO 2S  ");//一共可以写16个字符
	}
	else
	{
		OLED_ShowString(0,4,"AUTO 2S   ");//一共可以写16个字符
	}

	if(6==g_currentRowNum)
	{
		OLED_ShowStringFanBai(0,6,"AUTO 3S  ");//一共可以写16个字符
	}
	else
	{
		OLED_ShowString(0,6,"AUTO 3S   ");//一共可以写16个字符
	}	    
}

/*
//-----------------------------------------------------------------------------------
//设置模式时候 显示第二页
void cocoguojia_oledShowSetModel22(void)
{

     OLED_ShowCHinese2FanBai(0,0,7);		//设
     OLED_ShowCHinese2FanBai(18,0,8);		//置

	if(1==g_rtkModelFlag)
	{
		OLED_ShowStringFanBai(0,2,"RTK        ");//一共可以写16个字符
        OLED_ShowString(0,4,"NO RTK     ");//一共可以写16个字符
	}
	else
	{
		OLED_ShowString(0,2,"RTK        ");//一共可以写16个字符
        OLED_ShowStringFanBai(0,4,"NO RTK     ");//一共可以写16个字符
	}	
	
}
*/


//--------------------------------------------------------
//curr=1 显示当前设置  上电后显示一下当前设置 
//curr=0 显示设置结果  设置模式时候 显示第二页 
void cocoguojia_oledShowSetModel2(uint8_t curr)
{
	if(1==curr)
	{
		OLED_ShowCHinese2(18,0,11);		//当
		OLED_ShowCHinese2(36,0,12);		//前
		OLED_ShowCHinese2(54,0,7);		//设
		OLED_ShowCHinese2(72,0,8);		//置
	}
	else
	{	
		OLED_ShowCHinese2(18,0,7);		//设
		OLED_ShowCHinese2(36,0,8);		//置
		OLED_ShowCHinese2(54,0,3);		//结
		OLED_ShowCHinese2(72,0,4);		//果
	}
 /*   
    if(1==g_rtkModelFlag)
	{
		OLED_ShowStringFanBai(0,2,"RTK   ");//一共可以写16个字符
	}
	else
	{
		OLED_ShowStringFanBai(0,2,"NO RTK ");//一共可以写16个字符
	}	
*/	
	
	if(2==g_signalOrAutoModelFlag)
	{
		OLED_ShowStringFanBai(0,2,"SIGNAL   ");//一共可以写16个字符
	}
	else
	{
		if(4==g_signalOrAutoModelFlag)
		{
			OLED_ShowStringFanBai(0,2,"AUTO 2S   ");//一共可以写16个字符
		}
		else
		{
			OLED_ShowStringFanBai(0,2,"AUTO 3S   ");//一共可以写16个字符
		}
	}

  
}

//--------------------------------------------------------
//从设置模式进入到修复模式
void cocoguojia_oledShowRepairModel(uint8_t step)
{
     static uint8_t i=0;
	 uint8_t n;
     OLED_ShowCHinese2(18,0,21);		//修
     OLED_ShowCHinese2(36,0,22);		//复
     OLED_ShowCHinese2(54,0,9);		    //模
     OLED_ShowCHinese2(72,0,10);		//式  
    
    if(3>=step)
    {
        //-------------------------------------------
        //分别显示5个相机的开关机状态
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
        //开机反白
        OLED_ShowCHinese2FanBai(0,2,2);		//开
		OLED_ShowCHinese2FanBai(18,2,1);	//机
		OLED_ShowCHinese2(0,4,6);		    //关
		OLED_ShowCHinese2(18,4,1);          //机 
          
        //开机中...
        OLED_ShowCHinese2(0,6,2);		//开
        OLED_ShowCHinese2(18,6,1);		//机
        OLED_ShowCHinese2(36,6,5);		//中 
          
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
      else if(2==step)//此时 按键和OK键为一体了
      {
        OLED_ShowCHinese2(0,2,2);		    //开
		OLED_ShowCHinese2(18,2,1);	        //机
		OLED_ShowCHinese2(0,4,6);		    //关
		OLED_ShowCHinese2(18,4,1);          //机
        //短按=OK 长按=关机
        OLED_ShowCHinese2(0,6,24);		//短
        OLED_ShowCHinese2(16,6,25);		//按
        OLED_ShowString(32,6,"=OK");  
        OLED_ShowCHinese2(56,6,23);		//长
        OLED_ShowCHinese2(72,6,25);		//按
        OLED_ShowString(88,6,"=NEXT");          
      } 
      else if(3==step)//关机反白
      {
        OLED_ShowCHinese2(0,2,2);		    //开
		OLED_ShowCHinese2(18,2,1);	        //机
		OLED_ShowCHinese2FanBai(0,4,6);		//关
		OLED_ShowCHinese2FanBai(18,4,1);    //机 
          
        //关机中... 
        OLED_ShowCHinese2(0,6,6);		//关
        OLED_ShowCHinese2(18,6,1);		//机
        OLED_ShowCHinese2(36,6,5);		//中
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
         //请重启设备电源...
         OLED_ShowCHinese2(0,2,26);		    //请          
         OLED_ShowCHinese2(18,2,27);		//重
         OLED_ShowCHinese2(36,2,28);		//启
         OLED_ShowCHinese2(54,2,29);		//设  
         OLED_ShowCHinese2(72,2,30);		//备
         OLED_ShowCHinese2(90,2,31);		//电
         OLED_ShowCHinese2(108,2,32);		//请
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

//---------------------------------------
//延时函数
static void delay_oled(void)
{
	uint32_t i=1000000;
	while(i--)
	{
		;
	}
}























