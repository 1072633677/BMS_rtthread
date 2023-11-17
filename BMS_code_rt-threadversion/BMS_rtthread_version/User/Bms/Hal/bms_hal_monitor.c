#include "bms_hal_monitor.h"

#include <string.h>

#include "bms_monitor.h"
#include "bms_global.h"
#include "bms_utils.h"

#include "bms_debug.h"

#include "drv_softi2c_bq769x0.h"





// ð������ıȽϳ���,�Ե�ѹ���ݽ��бȽ�
static int compaer_cell(void *e1, void *e2)
{
	float temp1, temp2;
	
	temp1 = (*(BMS_CellDataTypedef *)e1).CellVoltage;
	temp2 = (*(BMS_CellDataTypedef *)e2).CellVoltage;

	if (temp1 > temp2)
	{
		return 1;
	}

    return 0;
}


void Bms_HalMonitorCellVoltage(void)
{	
	BQ769X0_UpdateCellVolt();
	for (uint8_t index = 0; index < BMS_GlobalParam.Cell_Real_Number; index++)
	{
		BMS_MonitorData.CellVoltage[index] = BQ769X0_SampleData.CellVoltage[index];
		BMS_MonitorData.CellData[index].CellVoltage = BQ769X0_SampleData.CellVoltage[index];
		BMS_MonitorData.CellData[index].CellNumber = index;
	}

	// ����ð������
	BubbleSort(BMS_MonitorData.CellData, BMS_GlobalParam.Cell_Real_Number, sizeof(BMS_CellDataTypedef), compaer_cell);
}


void Bms_HalMonitorBatteryVoltage(void)
{
	BQ769X0_UpadteBatVolt();
	BMS_MonitorData.BatteryVoltage = BQ769X0_SampleData.BatteryVoltage;
}


void Bms_HalMonitorBatteryCurrent(void)
{
	BQ769X0_UpdateCurrent();
	BMS_MonitorData.BatteryCurrent = BQ769X0_SampleData.BatteryCurrent;	
}


void Bms_HalMonitorCellTemperature(void)
{	
	uint8_t index1 = 0, index2 = 0;
	
	BQ769X0_UpdateTsTemp();	
	for (; index1 < BMS_GlobalParam.Temp_Real_Number; index1++)
	{
		if (BQ769X0_SampleData.TsxTemperature[index1] >= BMS_TEMP_MEASURE_MIN &&  BQ769X0_SampleData.TsxTemperature[index1] <= BMS_TEMP_MEASURE_MAX)
		{
			BMS_MonitorData.CellTemp[index2++] = BQ769X0_SampleData.TsxTemperature[index1];
		}
	}
	
	BMS_MonitorData.CellTempEffectiveNumber = index2;

	// ����˳������
	BubbleFloat(BMS_MonitorData.CellTemp, index2);
}


bool Bms_HalMonitorLoadDetect(void)
{
	// BQоƬֻ����δ������������²���CHG���ŵ�ѹ����0.7V���ܹ���⵽����
	return BQ769X0_LoadDetect();
}


