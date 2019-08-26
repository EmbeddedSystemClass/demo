

#ifndef __24C02_COCOGUOJIA_H__
#define __24C02_COCOGUOJIA_H__



#include "myinclude.h"
#include <inttypes.h>


#define EEPROM_I2C_WR	0		/* 写控制bit */
#define EEPROM_I2C_RD	1		/* 读控制bit */


/* 定义I2C总线连接的GPIO端口, 用户只需要修改下面4行代码即可任意改变SCL和SDA的引脚 */
#define EEPROM_GPIO_PORT_I2C	GPIOA			/* GPIO端口 */
#define EEPROM_RCC_I2C_PORT 	RCC_APB2Periph_GPIOA		/* GPIO端口时钟 */
#define EEPROM_I2C_SCL_PIN		GPIO_Pin_7			/* 连接到SCL时钟线的GPIO */
#define EEPROM_I2C_SDA_PIN		GPIO_Pin_6			/* 连接到SDA数据线的GPIO */


/* 定义读写SCL和SDA的宏，已增加代码的可移植性和可阅读性 */
#if 0	/* 条件编译： 1 选择GPIO的库函数实现IO读写 */
	#define EEPROM_I2C_SCL_1()  GPIO_SetBits(EEPROM_GPIO_PORT_I2C, EEPROM_I2C_SCL_PIN)		/* SCL = 1 */
	#define EEPROM_I2C_SCL_0()  GPIO_ResetBits(EEPROM_GPIO_PORT_I2C, EEPROM_I2C_SCL_PIN)		/* SCL = 0 */
	
	#define EEPROM_I2C_SDA_1()  GPIO_SetBits(EEPROM_GPIO_PORT_I2C, EEPROM_I2C_SDA_PIN)		/* SDA = 1 */
	#define EEPROM_I2C_SDA_0()  GPIO_ResetBits(EEPROM_GPIO_PORT_I2C, EEPROM_I2C_SDA_PIN)		/* SDA = 0 */
	
	#define EEPROM_I2C_SDA_READ()  GPIO_ReadInputDataBit(EEPROM_GPIO_PORT_I2C, EEPROM_I2C_SDA_PIN)	/* 读SDA口线状态 */
#else	/* 这个分支选择直接寄存器操作实现IO读写 */
    /*　注意：如下写法，在IAR最高级别优化时，会被编译器错误优化 */
	#define EEPROM_I2C_SCL_1()  EEPROM_GPIO_PORT_I2C->BSRR = EEPROM_I2C_SCL_PIN				/* SCL = 1 */
	#define EEPROM_I2C_SCL_0()  EEPROM_GPIO_PORT_I2C->BRR = EEPROM_I2C_SCL_PIN				/* SCL = 0 */
	
	#define EEPROM_I2C_SDA_1()  EEPROM_GPIO_PORT_I2C->BSRR = EEPROM_I2C_SDA_PIN				/* SDA = 1 */
	#define EEPROM_I2C_SDA_0()  EEPROM_GPIO_PORT_I2C->BRR = EEPROM_I2C_SDA_PIN				/* SDA = 0 */
	
	#define EEPROM_I2C_SDA_READ()  ((EEPROM_GPIO_PORT_I2C->IDR & EEPROM_I2C_SDA_PIN) != 0)	/* 读SDA口线状态 */
#endif


void i2c_Start(void);
void i2c_Stop(void);
void i2c_SendByte(uint8_t _ucByte);
uint8_t i2c_ReadByte(void);
uint8_t i2c_WaitAck(void);
void i2c_Ack(void);
void i2c_NAck(void);
uint8_t i2c_CheckDevice(uint8_t _Address);

/* 
 * AT24C02 2kb = 2048bit = 2048/8 B = 256 B
 * 32 pages of 8 bytes each
 *
 * Device Address
 * 1 0 1 0 A2 A1 A0 R/W
 * 1 0 1 0 0  0  0  0 = 0XA0
 * 1 0 1 0 0  0  0  1 = 0XA1 
 */

/* AT24C01/02每页有8个字节 
 * AT24C04/08A/16A每页有16个字节 
 */
	

#define EEPROM_DEV_ADDR			  0xA0		/* 24xx02的设备地址    */
#define EEPROM_PAGE_SIZE		  8			/* 24xx02的页面大小    */
#define EEPROM_SIZE				  256		/* 24xx02总容量 		  */



///////////////////////////////////////////////////////////////////////////
//函 数 名: cocoguojia_eeCheckOk
//功能说明: 判断串行EERPOM是否正常
//形    参：无
//返 回 值: 1 表示正常， 0 表示不正常
//必须用一次这个 因为这里面有I2C的GPIO初始化
extern uint8_t cocoguojia_eeCheckOk(void);


///////////////////////////////////////////////////////////////////////////
//	函 数 名: cocoguojia_eeReadBytes
//	功能说明: 从串行EEPROM指定地址处开始读取若干数据
//	形    参：_usAddress : 起始地址
//			 _usSize : 数据长度，单位为字节
//			 _pReadBuf : 存放读到的数据的缓冲区指针
//	返 回 值: 0 表示失败，1表示成功
extern uint8_t cocoguojia_eeReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize);


///////////////////////////////////////////////////////////////////////////
//	函 数 名: cocoguojia_eeWriteBytes
//	功能说明: 向串行EEPROM指定地址写入若干数据，采用页写操作提高写入效率
//	形    参：_usAddress : 起始地址
//			 _usSize : 数据长度，单位为字节
//			 _pWriteBuf : 存放读到的数据的缓冲区指针
//	返 回 值: 0 表示失败，1表示成功
extern uint8_t cocoguojia_eeWriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize);


////////////////////////////////////////////////////
//全部擦除EEPROM
extern void cocoguojia_eeErase(void);


////////////////////////////////////////////////////
//读参数
extern void cocoguojia_readSetParameter(void);


////////////////////////////////////////////////////
//写参数
extern void cocoguojia_writeSetParameter(void);

#endif



