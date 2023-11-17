#ifndef __BMS_ENERGY_H__
#define __BMS_ENERGY_H__


#include <rtthread.h>

#include "bms_type.h"


typedef struct
{
	float SocStopChg;			// ֹͣ���SOCֵ
	float SocStartChg;			// �������SOCֵ
	float SocStopDsg;			// ֹͣ�ŵ�SOCֵ
	float SocStartDsg;			// �����ŵ�SOCֵ
	
	float BalanceStartVoltage;	// ������ʼ��ѹ(V)
	float BalanceDiffeVoltage;	// ��������ѹ(V)
	uint32_t BalanceCycleTime;	// ��������ʱ��(s)
	BMS_CellIndexTypedef BalanceRecord;	// �����¼,���ھ���Ļᱻλ����
	
}BMS_EnergyDataTypedef;


extern rt_sem_t BalanceSem;
extern BMS_EnergyDataTypedef BMS_EnergyData;


void BMS_EnergyInit(void);



#endif

