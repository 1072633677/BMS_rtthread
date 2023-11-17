#ifndef __DRV_SOFT_I2C_H__
#define __DRV_SOFT_I2C_H__

#include <stdio.h>
#include <rtthread.h>
#include "rthw.h"

#include "stm32f1xx_hal.h"


#define I2C_DEBUG_LEVEL 3

#if I2C_DEBUG_LEVEL == 0
#define I2C_INFO(fmt, arg...) 
#define I2C_WARNING(fmt, arg...)
#define I2C_ERROR(fmt, arg...)

#elif I2C_DEBUG_LEVEL == 1
#define I2C_INFO(fmt, arg...)   		rt_kprintf("<<-I2C-INFO->> "fmt"\r\n",##arg)
#define I2C_WARNING(fmt, arg...)
#define I2C_ERROR(fmt, arg...)

#elif I2C_DEBUG_LEVEL == 2
#define I2C_INFO(fmt, arg...)
#define I2C_WARNING(fmt, arg...)		rt_kprintf("<<-I2C-WARNING->> "fmt"\r\n",##arg)
#define I2C_ERROR(fmt, arg...)

#elif I2C_DEBUG_LEVEL == 3
#define I2C_INFO(fmt, arg...)
#define I2C_WARNING(fmt, arg...)
#define I2C_ERROR(fmt, arg...)	 		rt_kprintf("<<-I2C-ERROR->> "fmt"\r\n",##arg)
#endif


#define I2C_WR              0x00       		/* д��־ */
#define I2C_RD              (1 << 0)      	/* ����־ */
#define I2C_ADDR_10BIT      (1 << 1)      	/* 10 λ��ַģʽ */
#define I2C_NO_START        (1 << 2)      	/* �޿�ʼ���� */
#define I2C_IGNORE_NACK     (1 << 3)      	/* ���� NACK */
#define I2C_NO_READ_ACK     (1 << 4)      	/* ����ʱ�򲻷��� ACK */
#define I2C_NO_STOP         (1 << 5) 		/* �����겻����ֹͣ�ź� */

// �������ֱ�־λ���Լ���ӵ�
#define I2C_CONTROL_BYTE    (1 << 6) 		/* ÿ����һ������֮ǰ��Ҫ����һ�������ֽ�������ʾ֮��������ֽ������������(�еĳ������õ����磺ssd1306������) */
#define I2C_SAME_BYTE       (1 << 7) 		/* ��������msg.tLen��msg.sByte�����ֽ�,������ͬ�ֽڻ��õ�����ѭ�����ô��亯�� */




/* i2c��Ϣ���� */
struct I2C_MessageTypeDef
{
	uint8_t   *buf;    		// ����ָ��������������
	uint16_t  addr;    		// ���豸��ַ
	uint16_t  tLen;		 	// ��Ҫ��������ݳ���(���65535),����I2C_CONTROL_BYTE��־�����ϸó���
	uint16_t  rLen; 	 	// ���سɹ���������ݳ���,�������I2C_CONTROL_BYTE��־���������Ͽ����ֽڳ���
	uint8_t   flags;	 	// ��־λ
	uint8_t   cByte;	 	// I2C_CONTROL_BYTE
	uint8_t   sByte;	 	// I2C_SAME_BYTE
};


/* i2c���� */
struct I2C_BusTypeDef
{
	GPIO_TypeDef *gpiox;
	uint32_t gpio_rcc;
	uint16_t sda_gpio_pin;
	uint16_t scl_gpio_pin;
	
	uint32_t retries; // ����ַ����Ӧ���Դ���(����ܳ���255)
	
	void (*udelay)(uint32_t us);
	void (*lockInit)(void);
	void (*lock)(void);
	void (*unlock)(void);
};

extern struct I2C_BusTypeDef i2c1;

int I2C_BusInitialize(void);
uint32_t I2C_TransferMessages(struct I2C_BusTypeDef *bus, struct I2C_MessageTypeDef msgs[], uint32_t num);

#endif
