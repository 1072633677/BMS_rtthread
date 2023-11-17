#define BMS_DBG_TAG "Info"

#include <stdio.h>
#include <rtthread.h>

#include "main.h"

#include "bms_info.h"

#include "bms_hal_monitor.h"
#include "bms_hal_control.h"

#include "bms_monitor.h"
#include "bms_energy.h"
#include "bms_analysis.h"
#include "bms_global.h"
#include "bms_debug.h"




// thread config
#define INFO_TASK_STACK_SIZE	512	
#define INFO_TASK_PRIORITY		13
#define INFO_TASK_TIMESLICE		25

#define INFO_TASK_PERIOD		2000






static void BMS_InfoTaskEntry(void *paramter);

static void BMS_InfoPrintf(void);




void BMS_InfoInit(void)
{
	rt_thread_t thread;


	thread = rt_thread_create("info", 
							   BMS_InfoTaskEntry, 
							   NULL,
							   INFO_TASK_STACK_SIZE,
							   INFO_TASK_PRIORITY,
							   INFO_TASK_TIMESLICE);

	if (thread == NULL)
	{
		BMS_ERROR("Create Task Fail");
	}

	rt_thread_startup(thread);

}



static void BMS_InfoTaskEntry(void *paramter)
{
	while(1)
	{
		BMS_InfoPrintf();		
		rt_thread_mdelay(INFO_TASK_PERIOD);
	}
}


// ����ָʾ��
static void BMS_BattLow(void)
{
	if (BMS_AnalysisData.SOC == 0)
	{
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET);		
	}
	else if (BMS_AnalysisData.SOC <= 0.25)
	{
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);		
	}
	else if (BMS_AnalysisData.SOC <= 0.5)
	{
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);	
	}
	else if (BMS_AnalysisData.SOC <= 0.75)
	{
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);
	}
	else if (BMS_AnalysisData.SOC <= 1)
	{
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);
	}	
}


// ʵʱ��ӡBMS��Ϣ
static void BMS_InfoPrintf(void)
{
	uint8_t index;
	char str[64];

	BMS_INFO("/*************************************************************/");
	
	// ��ذ�ʵʱ����
	sprintf(str, "Battery Real Capacity = %0.1fA/H", BMS_AnalysisData.CapacityReal);
	BMS_INFO("%s", str);	

	// ��ذ�ʣ������
	sprintf(str, "Battery Remain Capacity = %0.3fA/H", BMS_AnalysisData.CapacityRemain);
	BMS_INFO("%s", str);




	rt_kprintf("\r\n");

	
	
	
	BMS_BattLow();

	
	// SOC
	sprintf(str, "Battery SOC = %0.1f%%", BMS_AnalysisData.SOC * 100);
	BMS_INFO("%s", str);

	/*
	// SOH
	sprintf(str, "Battery SOH = %0.1f%", BMS_AnalysisData.SOH * 100);
	BMS_INFO("%s", str);

	// SOE
	sprintf(str, "Battery SOE = %0.1f%", BMS_AnalysisData.SOE * 100);
	BMS_INFO("%s", str);

	// SOP
	sprintf(str, "Battery SOP = %0.1f%", BMS_AnalysisData.SOP * 100);
	BMS_INFO("%s", str);
	*/	




	rt_kprintf("\r\n");


	// �����о����ѹ
	sprintf(str, "Cell Max Voltage = %0.3fV", BMS_AnalysisData.CellVoltMax);
	BMS_INFO("%s", str);


	// �����о��С��ѹ
	sprintf(str, "Cell Min Voltage = %0.3fV", BMS_AnalysisData.CellVoltMin);
	BMS_INFO("%s", str);

	

	// ����ѹ��
	sprintf(str, "Cell Max Voltage Difference = %0.3fV", BMS_AnalysisData.MaxVoltageDifference);
	BMS_INFO("%s", str);



	// ƽ����ѹ
	sprintf(str, "Cell Average Voltage = %0.3fV", BMS_AnalysisData.AverageVoltage);
	BMS_INFO("%s", str);



	// ʵʱ����
	sprintf(str, "Battery Real Power = %0.3fW", BMS_AnalysisData.PowerReal);
	BMS_INFO("%s", str);




	rt_kprintf("\r\n");




	// Battery Voltage
	sprintf(str, "Battery Voltage = %0.3fV", BMS_MonitorData.BatteryVoltage);
	BMS_INFO("%s", str);	




	// Battery Current
	sprintf(str, "Battery Current = %0.3fA", BMS_MonitorData.BatteryCurrent);
	BMS_INFO("%s", str);	




	// Cell Temperature
	for (index = 0; index < BMS_MonitorData.CellTempEffectiveNumber; index++)
	{
		sprintf(str, "Tempature %d = %0.1f", index + 1, BMS_MonitorData.CellTemp[index]);
		BMS_INFO("%s", str);
	}





	rt_kprintf("\r\n");





	// Cell Voltage
	for (index = 0; index < BMS_GlobalParam.Cell_Real_Number; index++)
	{
		sprintf(str, "Cell%-2d Voltage = %-5.3fV %s",
		index + 1, 
		BMS_MonitorData.CellVoltage[index],
		(BMS_EnergyData.BalanceRecord &  (1 << index)) > 0 ? "--->" : "");
		BMS_INFO("%s", str);
	}


	
	BMS_INFO("/*************************************************************/\r\n\r\n");
}


