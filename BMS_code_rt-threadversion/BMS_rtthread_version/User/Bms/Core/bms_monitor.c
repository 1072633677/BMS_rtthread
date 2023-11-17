#define BMS_DBG_TAG "Monitor"

#include <stdio.h>
#include <rtthread.h>

#include "bms_monitor.h"

#include "bms_hal_monitor.h"

#include "bms_energy.h"
#include "bms_global.h"
#include "bms_debug.h"





// thread config
#define MONITOR_TASK_STACK_SIZE	512
#define MONITOR_TASK_PRIORITY	9
#define MONITOR_TASK_TIMESLICE	25

#define MONITOR_TASK_PERIOD		250



// sample time config  MS
#define UPDATE_CELL_VOLTAGE_CYCLE	250
#define UPDAYE_BAT_VOLTAGE_CYCLE	250
#define UPDATE_CELL_TEMP_CYCLE		2000
#define UPDATE_BAT_CURRENT_CYCLE	1000






BMS_MonitorDataTypedef BMS_MonitorData;



static bool FlagSampleIntCur = false;

static bool FlagCellVoltage = true;
static bool FlagBatVoltage = true;
static bool FlagCellTemp = true;
static bool FlagBatCurrent = true;


static uint16_t CountCellVoltage = 0;
static uint16_t CountBatVoltage = 0;
static uint16_t CountCellTemp = 0;
//static uint16_t CountBatCurrent = 0;



static void BMS_MonitorTaskEntry(void *paramter);
static void BMS_MonitorBattery(void);
static void BMS_MonitorSysMode(void);


void BMS_MonitorInit(void)
{
	rt_thread_t thread;
	
	thread = rt_thread_create("monitor", 
							   BMS_MonitorTaskEntry, 
							   NULL,
							   MONITOR_TASK_STACK_SIZE,
							   MONITOR_TASK_PRIORITY,
							   MONITOR_TASK_TIMESLICE);

   if (thread == NULL)
   {
	   BMS_ERROR("Create Task Fail");
   }

	rt_thread_startup(thread);
}

static void BMS_MonitorTaskEntry(void *paramter)
{
	while (1)
	{
		BMS_MonitorBattery();
		BMS_MonitorSysMode();
		rt_thread_mdelay(MONITOR_TASK_PERIOD);
	}
}


// ��ص�ظ�������
static void BMS_MonitorBattery(void)
{
	// �����о��ѹ
	CountCellVoltage += MONITOR_TASK_PERIOD;
	if (FlagCellVoltage == true && CountCellVoltage >= UPDATE_CELL_VOLTAGE_CYCLE)
	{
		Bms_HalMonitorCellVoltage();
		CountCellVoltage = 0;
	}
	else if (FlagCellVoltage == false)
	{
		CountCellVoltage = 0;
	}
	
	// ������ѹ
	CountBatVoltage += MONITOR_TASK_PERIOD;
	if (FlagBatVoltage == true && CountBatVoltage >= UPDAYE_BAT_VOLTAGE_CYCLE)
	{
		Bms_HalMonitorBatteryVoltage();
		CountBatVoltage = 0;		
	}
	else if (FlagBatVoltage == false)
	{
		CountBatVoltage = 0;
	}

	// ����¶�
	CountCellTemp += MONITOR_TASK_PERIOD;
	if (FlagCellTemp == true && CountCellTemp++ >= UPDATE_CELL_TEMP_CYCLE)
	{
		Bms_HalMonitorCellTemperature();
		CountCellTemp = 0;
	}
	else if (FlagCellTemp == false)
	{
		CountCellTemp = 0;
	}


	/* ����������������� 
	CountBatCurrent += MONITOR_TASK_PERIOD;
	if (FlagBatCurrent == true && CountBatCurrent >= UPDATE_BAT_CURRENT_CYCLE)
	{
		CountBatCurrent = 0;
		Bms_HalMonitorBatteryCurrent();
	}
	else if (FlagBatCurrent == false)
	{
		CountBatCurrent = 0;
	}
	*/


	/* ����������Ӳ���жϴ���,̫�鷳��,ÿ����д�������¸�BQ�������ϵ� */
	if (FlagSampleIntCur == true && FlagBatCurrent == true)
	{
		Bms_HalMonitorBatteryCurrent();
		FlagSampleIntCur = false;		
	}
}



// ϵͳģʽ���
// BatteryCurrent > 20mA || BatteryCurrent < -20mA  ���ڷ�˯��ģʽ
// BatteryCurrent < 20mA || BatteryCurrent > -20mA  ���ڴ���ģʽ����˯��ģʽ
// BatteryCurrent <= -20mA ���ڷŵ�ģʽ
// BatteryCurrent >=  20mA ���ڳ��ģʽ
static void BMS_MonitorSysMode(void)
{
	static BMS_SysModeTypedef SysModeBackup = BMS_MODE_NULL;
	static uint32_t StandbyCount = 0;
	
	if (BMS_GlobalParam.SysMode == BMS_MODE_SLEEP)
	{
		if ((BMS_MonitorData.BatteryCurrent >= 0.02) || (BMS_MonitorData.BatteryCurrent <= -0.02))
		{
			// ���Լӻ��Ѵ����߼�
			
			BMS_GlobalParam.SysMode = BMS_MODE_STANDBY;
			BMS_INFO("Wake Up");
		}
		return;
	}

	
	if (BMS_MonitorData.BatteryCurrent < 0.02 && BMS_MonitorData.BatteryCurrent > -0.02)
	{
		BMS_GlobalParam.SysMode = BMS_MODE_STANDBY;	
		
		if (StandbyCount >= BMS_ENTRY_SLEEP_TIME * 60000)
		{
			// û�е�о���ھ��������²Ž���˯��
			if (rt_sem_take(BalanceSem, RT_WAITING_NO) == RT_EOK)
			{
				// ���Լ�˯�ߵ͹��Ĵ����߼�
				
				StandbyCount = 0;
				
				BMS_GlobalParam.SysMode = BMS_MODE_SLEEP;

				rt_sem_release(BalanceSem);
				
				BMS_INFO("Entry Sleep Mode");
			}
		}
		else
		{
			StandbyCount += MONITOR_TASK_PERIOD;
		}


		// ������
		if (SysModeBackup != BMS_MODE_STANDBY)
		{
			SysModeBackup = BMS_MODE_STANDBY;
			BMS_INFO("Entry Standby Mode");
		}
	}
	else if (BMS_MonitorData.BatteryCurrent >= 0.02)
	{
		StandbyCount = 0;
		BMS_GlobalParam.SysMode = BMS_MODE_CHARGE;

		// ������
		if (SysModeBackup != BMS_MODE_CHARGE)
		{
			SysModeBackup = BMS_MODE_CHARGE;
			BMS_INFO("Entry Charge Mode");
		}
	}
	else if (BMS_MonitorData.BatteryCurrent <= -0.02)
	{
		StandbyCount = 0;
		BMS_GlobalParam.SysMode = BMS_MODE_DISCHARGE;

		// ������
		if (SysModeBackup != BMS_MODE_DISCHARGE)
		{
			SysModeBackup = BMS_MODE_DISCHARGE;
			BMS_INFO("Entry Discharge Mode");
		}
	}
}










void BMS_MonitorStateCellVoltage(BMS_StateTypedef NewState)
{
	if (NewState == BMS_STATE_ENABLE)
	{
		FlagCellVoltage = true;
	}
	else if (NewState == BMS_STATE_DISABLE)
	{
		FlagCellVoltage = false;
	}
}

void BMS_MonitorStateBatVoltage(BMS_StateTypedef NewState)
{
	if (NewState == BMS_STATE_ENABLE)
	{
		FlagBatVoltage = true;
	}
	else if (NewState == BMS_STATE_DISABLE)
	{
		FlagBatVoltage = false;
	}
}

void BMS_MonitorStateCellTemp(BMS_StateTypedef NewState)
{
	if (NewState == BMS_STATE_ENABLE)
	{
		FlagCellTemp = true;
	}
	else if (NewState == BMS_STATE_DISABLE)
	{
		FlagCellTemp = false;
	}	
}

void BMS_MonitorStateBatCurrent(BMS_StateTypedef NewState)
{
	if (NewState == BMS_STATE_ENABLE)
	{
		FlagBatCurrent = true;
	}
	else if (NewState == BMS_STATE_DISABLE)
	{
		FlagBatCurrent = false;
	}	
}





void BMS_MonitorHwCurrent(void)
{
	FlagSampleIntCur = true;
}

