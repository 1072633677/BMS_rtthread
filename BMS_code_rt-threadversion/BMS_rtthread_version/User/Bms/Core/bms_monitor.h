#ifndef __BMS_MONITOR_H__
#define __BMS_MONITOR_H__


#include <stdbool.h>

#include "bms_type.h"


typedef struct
{
	float CellVoltage; 		// ��о��ѹ
	uint32_t CellNumber;	// ��о�ı��
}BMS_CellDataTypedef;


typedef struct
{
	float CellTemp[BMS_TEMP_MAX];					// �����¶�,�¶����ݻ��С��������
	float BatteryVoltage;							// ����ܵ�ѹ
	float BatteryCurrent;							// ��������
	BMS_CellDataTypedef CellData[BMS_CELL_MAX]; 	// ��о����,��ѹ���ݻ��С��������
	float CellVoltage[BMS_CELL_MAX]; 				// ��о��ѹ,δ�����
	uint32_t CellTempEffectiveNumber;				// ��Чֵ���¶�����
}BMS_MonitorDataTypedef;


extern BMS_MonitorDataTypedef BMS_MonitorData;


void BMS_MonitorInit(void);
void BMS_MonitorStateCellVoltage(BMS_StateTypedef NewState);
void BMS_MonitorStateBatVoltage(BMS_StateTypedef NewState);
void BMS_MonitorStateBatCurrent(BMS_StateTypedef NewState);
void BMS_MonitorStateCellTemp(BMS_StateTypedef NewState);

void BMS_MonitorHwCurrent(void);


#endif


