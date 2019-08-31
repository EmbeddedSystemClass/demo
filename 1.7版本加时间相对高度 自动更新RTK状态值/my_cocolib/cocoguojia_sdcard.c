 
 
#include "cocoguojia_sdcard.h"
#include <stdlib.h>

//-----------------------------------------------------
//我们定义 磁盘为0 我们都是对这个磁盘进行操作的
#define MYDISK0  "0:"

uint8_t g_nameFlag=0;//0=还没确认过文本名字的日期   1=确认过文本名字的日期 2=创建过表头


//----------------------------------------------------------------------------------------------
//全局变量
//根据实际情况来改变文件txt的名称
TCHAR g_cocoguojia_sdcard_txtname20[20]				={"000"};					//TCHAR系统定义为char  另: FatFs  const TCHAR *fileName
			


BYTE  g_cocoguojia_sdcard_myWriteBuffer200[200] 	={"123456789 abcdef GHIJKL\r\n"};		//写缓冲区

#ifdef I_WANT_TO_READ_SDCARD
BYTE  g_cocoguojia_sdcard_myReadBuffer1024[1024] 	;//读缓冲区 如果不读 这个可以屏蔽
#endif

//--------------------------------------------------
//内部变量
FATFS 		myFatFs;			//FatFs文件系统对象
FIL  		myFile;				//文件对象
UINT 		myFileNum;         	//文件成功读写数量
FRESULT 	myResetFlash;		//文件操作结果

DIR dirs;
FILINFO finfo;
char txt_name[20] = {0};
int now_jc = 0;
int MAX_JC = 0;

u8 g_registeredWorkspaceN=0;//总共少次 写TF卡进入到不稳定状态


//延时nms

void cocoguojia_SdioFlash_ms(u16 nms)
{	 		  	  
	u32 temp;	
	u16 ms;		
	for(ms=0;ms<nms;ms++)
	{
		temp=30000;
		while(temp--)
		{
			;
		}
	}
} 



 
uint8_t gpsRtkTempTableTemp[200]={0,0,0,0};
uint8_t gpsRtkTempLen=0;


u8 g_gpsTableFlag=1;
u8 g_gps001Table[200];
u8 g_gps002Table[200];


uint8_t len;
//-----------------------------------------------------
//cocoguojiapsdk TF卡 待改
uint16_t g_sdWriteForGpsRtkNum=1;
void cocoguojia_sdWrireGpsRtk(void)
{
    uint8_t i=0;  
    uint8_t *str;
    //application.c将数据填充到了g_gps001TableTemp
    if(1==g_gpsTableFlag)      //application.c 如果等于1就填充到002     保证一个读一个填充
    {
        for(i=0;i<185;i++)
        {       
            if(0!=g_gps001Table[i])
            {
                gpsRtkTempTableTemp[i]=g_gps001Table[i];
            }
            else
            {
                gpsRtkTempTableTemp[i]=0;
                i++;
                break;
            }
        }
    }
    else  //application.c如果等于二 就填充到001
    {
        for(i=0;i<185;i++)
        {       
					if(0!=g_gps002Table[i])  //判断某个字符是不是为空  
            {
                gpsRtkTempTableTemp[i]=g_gps002Table[i];
            }
            else
            {
							gpsRtkTempTableTemp[i]=0;  //当那一位的数据为空，就退出那个循环
                i++;
                break;
            }
        }
    }
  
    str=&gpsRtkTempTableTemp[0];
    len=i-1;
    
    if(180>=len)
    {


        //#################################
        taskENTER_CRITICAL();           								    //进入临界区
        //在磁盘MYDISK0打开文件fileName如果没有则创建文件 
        //并且写入字符串 这个字符串在g_cocoguojia_sdcard_myWriteBuffer200数组里
        cocoguojia_WriteFileSystem(g_cocoguojia_sdcard_txtname20); 
        cocoguojia_SdioFlash_ms(2);
        cocoguojia_CancleWorkspace();//在FatFs模块上注销一个工作区(文件系统对象)   注销的磁盘为: MYDISK0 
        cocoguojia_SdioFlash_ms(1);
        cocoguojia_CancleWorkspace();//在FatFs模块上注销一个工作区(文件系统对象)   注销的磁盘为: MYDISK0 
        cocoguojia_SdioFlash_ms(1);
        
        taskEXIT_CRITICAL();            								    //退出临界区
        //##################################
    }
    else
    {
        ;//长度超了 忽略
    }   
}


//---------------------------------------------------------------------------------------------------------------------------------------------
//TF卡底层操作函数
//---------------------------------------------------------------------------------------------------------------------------------------------
//在FatFs模块上注册一个工作区(文件系统对象)  创建一个磁盘MYDISK0
void cocoguojia_RegisteredWorkspace(void)
{
	
	//----------------------------------------------------------------------------------------------------------------------------
  //在FatFs模块上注册/注销一个工作区(文件系统对象) 在外部SD卡挂载文件系统，文件系统挂载时会对SDIO设备初始化
	//f_mount函数有三个形式参数
	//FATFS* fs,			// 指向一个文件系统 (如果为NULL 则是挂起文件系统系统)
	//const TCHAR* path,	// 要注册或者注销的逻辑驱动号(0 1 2...) 其实这里的逻辑驱动号就相当于 计算机的磁盘 C D E F... 				
	//BYTE opt=0 会在后面才执行加载操作，也就是说不管你是否外挂了设备，调用f_mount后，都会返回成功的
    //BYTE opt=1 强制的进行加载操作
	//指定磁盘号 支持2种方式
	//f_mount(&fs, "1:", 1);
    //f_mount(&fs, "1:/", 1);
	
	myResetFlash = f_mount(&myFatFs,MYDISK0,1);
	
	if(myResetFlash == FR_NO_FILESYSTEM)//如果没有有效的FAT卷
	{
		myResetFlash=f_mkfs(MYDISK0,0,0);	//格式化							
		if(myResetFlash == FR_OK)			//格式化成功
		{
			myResetFlash = f_mount(NULL,MYDISK0,1); 		//先注销					
			myResetFlash = f_mount(&myFatFs,MYDISK0,1);		//在注册
		}
		else
		{
			//系统复位
			//cocoguojia_systemSoftReset();
			g_registeredWorkspaceN++;
			cocoguojia_SdioFlash_ms(1);			
		}
	}
	else if(myResetFlash!=FR_OK)//如果有其它原因导致 SD卡注册工作区失败
	{
		//while(1);//暂时是无限循环 得改成复位
		//系统复位
		//cocoguojia_systemSoftReset();
		g_registeredWorkspaceN++;
		cocoguojia_SdioFlash_ms(1);
	}
	else
	{
		;
	} 
}

//------------------------------------------------------------------------------
//在FatFs模块上注销一个工作区(文件系统对象)   注销的磁盘为: MYDISK0 
void cocoguojia_CancleWorkspace(void)
{
	f_mount(NULL,MYDISK0,1);//不再使用文件系统，取消挂载文件系统
}

uint8_t sdcard_myWriteBuffer120[120]={"NUM, Lat,    Long,   hight, P,    R,    Y, Relative hight , Q , Data ,  time \r\n"};
//------------------------------------------------------------------------------
//在磁盘MYDISK0打开文件fileName如果没有则创建文件
//并且写入字符串 这个字符串在g_cocoguojia_sdcard_myWriteBuffer200数组里
//形式参数 const TCHAR *fileName  为文件的名称
void cocoguojia_WriteFileSystem(const TCHAR *fileName)
{
	UINT i= 0 ;
	int j = 0;
	static char Flag_DIR = 1;
	// 打开文件，如果文件不存在则创建它
	zzz:	
	if (f_opendir(&dirs, fileName) == FR_OK) //打开文件夹
	{
		if(Flag_DIR )  //if是判断一次 执行一次
   {
			while (f_readdir(&dirs, &finfo) == FR_OK) //按照顺序读文件夹
				{
					if(!finfo.fname[0]) break; //如果文件名为 0,结束
					
					if(finfo.fattrib == AM_ARC) //判断文件属性为存档文件
					{		
						while(finfo.fname[i] != '.')
						{
							txt_name[j] =  finfo.fname[i];
							i++;
							j++;
						}
						txt_name[j] = '\0';  //对应字符0
						now_jc = atoi((const char *)txt_name);
						
						if(now_jc >= MAX_JC)
						{
							MAX_JC = now_jc;
						}
					}	 
	       i = j = 0;					
				}
			
		}
	       Flag_DIR = 0;
		     sprintf(txt_name,"%s/%d.txt",fileName,MAX_JC+1);		    
		     myResetFlash = f_open(&myFile,txt_name,FA_OPEN_ALWAYS|FA_READ|FA_WRITE);		
	}
	else
	{
			f_mkdir(fileName);//创建新的文件夹
	    cocoguojia_SdioFlash_ms(1);
			goto zzz;
	}
	
	if ( myResetFlash == FR_OK )
	{
		f_lseek(&myFile,f_size(&myFile));//移动一个打开的文件对象的文件读 / 写指针  这样文件就得意续写
		//算出来实际长度再写 这样不会有 /0
		while(0!=g_cocoguojia_sdcard_myWriteBuffer200[i])
		{
			i++;
		}
        if(1==g_nameFlag)
        {
            myResetFlash=f_write(&myFile,&sdcard_myWriteBuffer120[0],strlen((const char*)sdcard_myWriteBuffer120),&myFileNum);//将指定存储区内容写入到文件内
            g_nameFlag=2;
        }
        else
        {
            ;
        }    
lll:
		myResetFlash=f_write(&myFile,&g_cocoguojia_sdcard_myWriteBuffer200[0],i,&myFileNum);//将指定存储区内容写入到文件内
		if(myResetFlash==FR_OK)
		{
			g_sdWriteForGpsRtkNum++;//成功
		}
		else
		{
            cocoguojia_SdioFlash_ms(2);
			goto lll;//文件写入失败
		}    
		/* 不再读写，关闭文件 */
		f_close(&myFile);
	}
	else
	{	
		;//打开/创建文件失败
	}
}

#ifdef I_WANT_TO_READ_SDCARD
//------------------------------------------------------------------------------
//读之前先清零缓存区 缓存区指针buff  缓冲器大小 n
static void cocoguojia_BeforeReadClearBuffer(BYTE *buff,unsigned short n)
{
	unsigned short i=0;
	for(i=0;i<n;i++)
	{
		*buff=0;
		buff++;
	}	
}

//------------------------------------------------------------------------------
//在磁盘MYDISK0读文件fileName
//并且将读到的数据写入字符串 这个字符串g_cocoguojia_sdcard_myReadBuffer1024数组里
//形式参数 const TCHAR *fileName  为文件的名称
void cocoguojia_ReadFileSystem(const TCHAR *fileName)
{
	cocoguojia_BeforeReadClearBuffer(&g_cocoguojia_sdcard_myReadBuffer1024[0],sizeof(g_cocoguojia_sdcard_myReadBuffer1024));
	
	myResetFlash = f_open(&myFile, fileName, FA_OPEN_EXISTING | FA_READ); 	 
	if(myResetFlash == FR_OK)
	{
			myResetFlash = f_read(&myFile, &g_cocoguojia_sdcard_myReadBuffer1024[0], sizeof(g_cocoguojia_sdcard_myReadBuffer1024), &myFileNum); 
			if(myResetFlash==FR_OK)
			{
				//文件读取成功,读到字节数myFileNum);
				//读取得的文件数据	
			}
			else
			{
				;//文件读取失败
			}		
	}
	else
	{
		;//打开文件失败
	}
	
	/* 不再读写，关闭文件 */
	f_close(&myFile);	
  
	/* 不再使用文件系统，取消挂载文件系统 */
	f_mount(NULL,MYDISK0,1);
}
#endif


