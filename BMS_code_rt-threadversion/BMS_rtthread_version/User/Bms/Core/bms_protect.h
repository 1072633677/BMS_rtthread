#ifndef __BMS_PROTECT_H__
#define __BMS_PROTECT_H__



#include "bms_config.h"




// ��������صĲ����ṹ��
typedef struct
{
	float ShoutdownVoltage;	// �ػ���ѹ(V)
	
	float OVProtect;		// ����ѹ������ѹ(V)					��Ӳ�����
	float OVRelieve;		// ����ѹ�ָ���ѹ(V)
	float UVProtect;		// �ŵ�Ƿѹ������ѹ(V)					��Ӳ�����
	float UVRelieve;		// �ŵ�Ƿѹ�ָ���ѹ(V)
	
	float OCCProtect;				// ��������ֵ(A)
	float OCDProtect;				// �ŵ������ֵ(A)				��Ӳ�����
	
	BMS_OVDelayTypedef OVDelay;		// ����ѹ������ʱ				��Ӳ�����
	BMS_UVDelayTypedef UVDelay;		// �ŵ�Ƿѹ������ʱ				��Ӳ�����
	BMS_OCDDelayTypedef OCDDelay;	// �ŵ����������ʱ				��Ӳ�����
	BMS_SCDDelayTypedef SCDDelay;	// �ŵ��·������ʱ				��Ӳ�����
	
	uint8_t OCDRelieve;		// �ŵ�����ָ�(S)
	uint8_t SCDRelieve;		// �ŵ��·�ָ�(S)
	
	uint8_t OCCDelay;		// ��������ʱ(S)
	uint8_t OCCRelieve;		// �������ָ�(S)
	
	float OTCProtect;		// �����±���(��)
	float OTCRelieve;		// �����½��(��)
	float OTDProtect;		// �ŵ���±���(��)
	float OTDRelieve;		// �ŵ���½��(��)
	
	float LTCProtect;		// �����±���(��)
	float LTCRelieve;		// �����½��(��)
	float LTDProtect;		// �ŵ���±���(��)
	float LTDRelieve;		// �ŵ���½��(��)
}BMS_ProtectParamTypedef;




// ����ö����
typedef enum
{
	FlAG_ALERT_NO	= 0x0000,	// �ޱ�������
	FlAG_ALERT_OV	= 0X0001,	// ����ѹ��������λ
	FlAG_ALERT_UV	= 0X0002,	// �ŵ�Ƿѹ��������λ
	FlAG_ALERT_OCC	= 0X0004,	// ��������������λ
	FlAG_ALERT_OCD	= 0X0008,	// �ŵ������������λ
	FlAG_ALERT_SCD	= 0X0010,	// �ŵ��·��������λ
	FlAG_ALERT_OTC	= 0X0020,	// �����±�������λ
	FlAG_ALERT_OTD	= 0X0040,	// �ŵ���±�������λ
	FlAG_ALERT_LTC	= 0X0080,	// �����±�������λ
	FlAG_ALERT_LTD	= 0X0100,	// �ŵ���±�������λ
}BMS_ProtectAlertTypedef;



// ��������״̬
typedef enum
{
	PROTECT_STATE_MONITOR,		// ���״̬
	PROTECT_STATE_RELIEVE_WAIT,	// �ȴ����ػָ�����
	PROTECT_STATE_RELIEVE,		// ִ�лָ�
}BMS_ProtectStateTypedef;



extern BMS_ProtectParamTypedef BMS_ProtectParam;
extern BMS_ProtectAlertTypedef BMS_ProtectAlert;


void BMS_ProtectInit(void);

void BMS_ProtectHwOCD(void);
void BMS_ProtectHwSCD(void);
void BMS_ProtectHwOV(void);
void BMS_ProtectHwUV(void);


void BMS_ProtectHwDevice(void);
void BMS_ProtectHwOvrd(void);



#endif


