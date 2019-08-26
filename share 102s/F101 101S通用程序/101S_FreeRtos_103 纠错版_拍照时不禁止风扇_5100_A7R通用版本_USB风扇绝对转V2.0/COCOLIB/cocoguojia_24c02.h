

#ifndef __24C02_COCOGUOJIA_H__
#define __24C02_COCOGUOJIA_H__



#include "myinclude.h"
#include <inttypes.h>


#define EEPROM_I2C_WR	0		/* д����bit */
#define EEPROM_I2C_RD	1		/* ������bit */


/* ����I2C�������ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����4�д��뼴������ı�SCL��SDA������ */
#define EEPROM_GPIO_PORT_I2C	GPIOA			/* GPIO�˿� */
#define EEPROM_RCC_I2C_PORT 	RCC_APB2Periph_GPIOA		/* GPIO�˿�ʱ�� */
#define EEPROM_I2C_SCL_PIN		GPIO_Pin_7			/* ���ӵ�SCLʱ���ߵ�GPIO */
#define EEPROM_I2C_SDA_PIN		GPIO_Pin_6			/* ���ӵ�SDA�����ߵ�GPIO */


/* �����дSCL��SDA�ĺ꣬�����Ӵ���Ŀ���ֲ�ԺͿ��Ķ��� */
#if 0	/* �������룺 1 ѡ��GPIO�Ŀ⺯��ʵ��IO��д */
	#define EEPROM_I2C_SCL_1()  GPIO_SetBits(EEPROM_GPIO_PORT_I2C, EEPROM_I2C_SCL_PIN)		/* SCL = 1 */
	#define EEPROM_I2C_SCL_0()  GPIO_ResetBits(EEPROM_GPIO_PORT_I2C, EEPROM_I2C_SCL_PIN)		/* SCL = 0 */
	
	#define EEPROM_I2C_SDA_1()  GPIO_SetBits(EEPROM_GPIO_PORT_I2C, EEPROM_I2C_SDA_PIN)		/* SDA = 1 */
	#define EEPROM_I2C_SDA_0()  GPIO_ResetBits(EEPROM_GPIO_PORT_I2C, EEPROM_I2C_SDA_PIN)		/* SDA = 0 */
	
	#define EEPROM_I2C_SDA_READ()  GPIO_ReadInputDataBit(EEPROM_GPIO_PORT_I2C, EEPROM_I2C_SDA_PIN)	/* ��SDA����״̬ */
#else	/* �����֧ѡ��ֱ�ӼĴ�������ʵ��IO��д */
    /*��ע�⣺����д������IAR��߼����Ż�ʱ���ᱻ�����������Ż� */
	#define EEPROM_I2C_SCL_1()  EEPROM_GPIO_PORT_I2C->BSRR = EEPROM_I2C_SCL_PIN				/* SCL = 1 */
	#define EEPROM_I2C_SCL_0()  EEPROM_GPIO_PORT_I2C->BRR = EEPROM_I2C_SCL_PIN				/* SCL = 0 */
	
	#define EEPROM_I2C_SDA_1()  EEPROM_GPIO_PORT_I2C->BSRR = EEPROM_I2C_SDA_PIN				/* SDA = 1 */
	#define EEPROM_I2C_SDA_0()  EEPROM_GPIO_PORT_I2C->BRR = EEPROM_I2C_SDA_PIN				/* SDA = 0 */
	
	#define EEPROM_I2C_SDA_READ()  ((EEPROM_GPIO_PORT_I2C->IDR & EEPROM_I2C_SDA_PIN) != 0)	/* ��SDA����״̬ */
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

/* AT24C01/02ÿҳ��8���ֽ� 
 * AT24C04/08A/16Aÿҳ��16���ֽ� 
 */
	

#define EEPROM_DEV_ADDR			  0xA0		/* 24xx02���豸��ַ    */
#define EEPROM_PAGE_SIZE		  8			/* 24xx02��ҳ���С    */
#define EEPROM_SIZE				  256		/* 24xx02������ 		  */



///////////////////////////////////////////////////////////////////////////
//�� �� ��: cocoguojia_eeCheckOk
//����˵��: �жϴ���EERPOM�Ƿ�����
//��    �Σ���
//�� �� ֵ: 1 ��ʾ������ 0 ��ʾ������
//������һ����� ��Ϊ��������I2C��GPIO��ʼ��
extern uint8_t cocoguojia_eeCheckOk(void);


///////////////////////////////////////////////////////////////////////////
//	�� �� ��: cocoguojia_eeReadBytes
//	����˵��: �Ӵ���EEPROMָ����ַ����ʼ��ȡ��������
//	��    �Σ�_usAddress : ��ʼ��ַ
//			 _usSize : ���ݳ��ȣ���λΪ�ֽ�
//			 _pReadBuf : ��Ŷ��������ݵĻ�����ָ��
//	�� �� ֵ: 0 ��ʾʧ�ܣ�1��ʾ�ɹ�
extern uint8_t cocoguojia_eeReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize);


///////////////////////////////////////////////////////////////////////////
//	�� �� ��: cocoguojia_eeWriteBytes
//	����˵��: ����EEPROMָ����ַд���������ݣ�����ҳд�������д��Ч��
//	��    �Σ�_usAddress : ��ʼ��ַ
//			 _usSize : ���ݳ��ȣ���λΪ�ֽ�
//			 _pWriteBuf : ��Ŷ��������ݵĻ�����ָ��
//	�� �� ֵ: 0 ��ʾʧ�ܣ�1��ʾ�ɹ�
extern uint8_t cocoguojia_eeWriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize);


////////////////////////////////////////////////////
//ȫ������EEPROM
extern void cocoguojia_eeErase(void);


////////////////////////////////////////////////////
//������
extern void cocoguojia_readSetParameter(void);


////////////////////////////////////////////////////
//д����
extern void cocoguojia_writeSetParameter(void);

#endif



