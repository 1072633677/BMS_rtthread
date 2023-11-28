#define BMS_DBG_TAG "Analysis"

#include <stdio.h>
#include <stdlib.h>
#include <rtthread.h>

#include "bms_analysis.h"

#include "bms_monitor.h"
#include "bms_protect.h"

#include "bms_utils.h"
#include "bms_global.h"
#include "bms_debug.h"





// thread config
#define ANALYSISI_TASK_STACK_SIZE	512
#define ANALYSISI_TASK_PRIORITY		11
#define ANALYSISI_TASK_TIMESLICE	25

#define ANALYSISI_TASK_PERIOD		1000



#define TEMP_CAP_RATE_LIMITH_HIGH   1050
#define TEMP_CAP_RATE_LIMITL_LOW    750







BMS_AnalysisDataTypedef BMS_AnalysisData =
{
	.CapacityRated = BMS_BATTERY_CAPACITY,
};




// ��Ԫ﮵�� SOC��·��ѹ���������ݱ�
// ֧��������ﮡ������Ҳ����һ�������
uint16_t SocOcvTab[101]=
{
	3282, // 0%~1%	
	3309, 3334, 3357, 3378, 3398, 3417, 3434, 3449, 3464, 3477,	// 0%~10%
	3489, 3500, 3510, 3520, 3528, 3536, 3543, 3549, 3555, 3561,	// 11%~20%
	3566, 3571, 3575, 3579, 3583, 3586, 3590, 3593, 3596, 3599,	// 21%~30%
	3602, 3605, 3608, 3611, 3615, 3618, 3621, 3624, 3628, 3632,	// 31%~40%
	3636, 3640, 3644, 3648, 3653, 3658, 3663, 3668, 3674, 3679,	// 41%~50%
	3685, 3691, 3698, 3704, 3711, 3718, 3725, 3733, 3741, 3748,	// 51%~60%
	3756, 3765, 3773, 3782, 3791, 3800, 3809, 3818, 3827, 3837,	// 61%~70%
	3847, 3857, 3867, 3877, 3887, 3897, 3908, 3919, 3929, 3940,	// 71%~80%
	3951, 3962, 3973, 3985, 3996, 4008, 4019, 4031, 4043, 4055,	// 81%~90%
	4067, 4080, 4092, 4105, 4118, 4131, 4145, 4158, 4172, 4185,	// 91~100%
};


static void BMS_AnalysisTaskEntry(void *paramter);


static void BMS_AnalysisEasy(void);
static void BMS_AnalysisCalCap(void);
static void BMS_AnalysisSocCheck(void);
static void BMS_AnalysisCapAndSocInit(void);

//SOH��SOP���㺯��
static void BMS_AnalysisSOHCheck(void);
static void BMS_AnalysisSOPCheck(void);



// ���״̬����ģ���ʼ��
void BMS_AnalysisInit(void)
{
	rt_thread_t thread;



	thread = rt_thread_create("analysis",
							   BMS_AnalysisTaskEntry,
							   NULL,
							   ANALYSISI_TASK_STACK_SIZE,
							   ANALYSISI_TASK_PRIORITY,
							   ANALYSISI_TASK_TIMESLICE);

	if (thread == NULL)
	{
		BMS_ERROR("Create Task Fail");
	}

	rt_thread_startup(thread);

}


// ���״̬���������߳����
static void BMS_AnalysisTaskEntry(void *paramter)
{
	BMS_AnalysisCapAndSocInit();

	while(1)
	{			
		BMS_AnalysisEasy();
		BMS_AnalysisCalCap();
		BMS_AnalysisSocCheck();

        BMS_AnalysisSOHCheck();
        BMS_AnalysisSOPCheck();

		rt_thread_mdelay(ANALYSISI_TASK_PERIOD);
	}
}


// �򵥷���,ͨ������ֱ�ӽ��м�����ܵõ���
static void BMS_AnalysisEasy(void)
{
	uint8_t index;

	// ����ѹ��
	BMS_AnalysisData.MaxVoltageDifference = BMS_MonitorData.CellData[BMS_GlobalParam.Cell_Real_Number - 1].CellVoltage - BMS_MonitorData.CellData[0].CellVoltage;
	
	
	// ƽ����ѹ
	for (index = 0, BMS_AnalysisData.AverageVoltage = 0; index < BMS_GlobalParam.Cell_Real_Number; index++)
	{
		BMS_AnalysisData.AverageVoltage += BMS_MonitorData.CellVoltage[index];
	}
	BMS_AnalysisData.AverageVoltage /= BMS_GlobalParam.Cell_Real_Number;
	
	
	// ʵʱ����
	BMS_AnalysisData.PowerReal = BMS_MonitorData.BatteryVoltage * BMS_MonitorData.BatteryCurrent;	


	// ������С��ѹ
	BMS_AnalysisData.CellVoltMax = BMS_MonitorData.CellData[BMS_GlobalParam.Cell_Real_Number - 1].CellVoltage;
	BMS_AnalysisData.CellVoltMin = BMS_MonitorData.CellData[0].CellVoltage;
}





// �¶�У׼,﮵�ػ���Ϊ�¶ȵı仯��Ӱ��������
static void BMS_AnalysisTempCal(void)
{
	static int16_t LastTemp = 0;

	uint8_t  Ratio; 	// У׼����
	uint16_t RateTemp;	
	int16_t MinTemp = BMS_MonitorData.CellTemp[0] * 10;


	if (BMS_MonitorData.CellTempEffectiveNumber == 0)
	{
		return;
	}


	// �ж��¶ȱ仯�Ƿ񳬹�1��
	if( MinTemp > LastTemp)  
	{
		if (MinTemp - LastTemp >= 10)
		{
			LastTemp = MinTemp;
		}
		else
		{
			return;
		}
	}
	else
	{
		if (LastTemp - MinTemp >= 10) 
		{
			LastTemp = MinTemp;
		}
		else
		{
			return;
		}
	}

	
	// ȷ��У׼����
	if (MinTemp >= 250)                             
	{
		Ratio = 1;
	}
	else if (MinTemp >= 100 && MinTemp < 250)   
	{   
		Ratio = 2;
	}
	else if (MinTemp >= 0 && MinTemp < 100)      
	{   
		Ratio = 3;
	}
	else if (MinTemp >= -200 && MinTemp < -10)    
	{   
		Ratio = 4;
	}
	else if (MinTemp >= -300 && MinTemp < -200)    
	{   
		Ratio = 5;
	}
	else
	{
		Ratio = 6;                                       
	}


	RateTemp = 1000 + Ratio * (MinTemp - 250) / 10;
	if(RateTemp > TEMP_CAP_RATE_LIMITH_HIGH )
	{
		RateTemp = TEMP_CAP_RATE_LIMITH_HIGH;
	}
	else if(RateTemp < TEMP_CAP_RATE_LIMITL_LOW)
	{
		RateTemp = TEMP_CAP_RATE_LIMITL_LOW;
	}

	// ʵʱ����
	BMS_AnalysisData.CapacityReal = BMS_AnalysisData.CapacityRated * RateTemp / 1000;

	// ʣ������
	BMS_AnalysisData.CapacityRemain = BMS_AnalysisData.CapacityReal * BMS_AnalysisData.SOC;
}



// ʵʱУ׼�����漰����:�¶ȡ�������ŵ硢�ϻ��ȵ�
static void BMS_AnalysisCalCap(void)
{	
	BMS_AnalysisTempCal();
}


// ���ݵ����о��͵�ѹ�����socֵ,�����ϵ�ͳ�ʱ�侲ֹ״̬�µ�У׼
static uint16_t BMS_AnalysisOcvToSoc(uint16_t voltage)
{
	uint16_t soc = 0;
	
	if (voltage <= SocOcvTab[0])
	{
		soc  = 0;
	}
	else if (voltage >= SocOcvTab[100])
	{
		soc = 1000;
	}
	else
	{
		uint16_t index = right_bound(SocOcvTab, 0, 100, voltage);

		if (voltage == SocOcvTab[index])
		{
			// ����SOCֵ
			soc = index * 10;
		}
		else
		{
			// ����ٷֱȺ��С����
			soc = index * 10 + (( SocOcvTab[index] - voltage) * 10) / (SocOcvTab[index] - SocOcvTab[index + 1]);		
		}
	}
	
	return soc;
}

// ��·��ѹ��soc����
static void BMS_AnalysisOcvSocCalculate(void)
{
	// ����˯�ߵ�����:����һ��ʱ��������û�е���ھ���
	if (BMS_GlobalParam.SysMode == BMS_MODE_SLEEP)
	{
		// �ȴ�һ��ʱ���ѹƽ��,��ֹ����Ÿս���
		rt_thread_mdelay(BALANCE_VOLT_RISE_DELAY);

		// ��·��ѹУ׼
		BMS_AnalysisData.SOC = BMS_AnalysisOcvToSoc(BMS_MonitorData.CellData[0].CellVoltage  * 1000) / 1000.0;

		// ʣ������ = ʵ������ * soc
		BMS_AnalysisData.CapacityRemain = BMS_AnalysisData.CapacityReal * BMS_AnalysisData.SOC;		
	}
}


// ��ʱ���ַ�soc����
// ����ģʽ���ж���͵�ѹֵ�Ƿ���ڵ��ڹ�ѹ����ֵ,������soc = 100%
// ����ģʽ���ж���͵�ѹֵ�Ƿ�С�ڵ���Ƿѹ����ֵ,������soc = 0%
// ���ʱ�Խ��в��������ĵ���ֵ+����
// �ŵ�ʱ�Խ��в��������ĵ���ֵ-����
// soc = ʵʱ���ֵ����� / ��ذ�ʵ������
static void BMS_AnalysisAHSocCalculate(void)
{
	// 3600��﮵�صı�׼������AH���ַ�ÿ�����һ�λ��֣�����ֵ�����ڵ�����ʱ��ĳ˻�
	float CurrentValue = abs((int32_t)(BMS_MonitorData.BatteryCurrent * 1000)) / 1000.0 / 3600;

	
	if (BMS_GlobalParam.SysMode == BMS_MODE_STANDBY)
	{
		if (BMS_MonitorData.CellData[0].CellVoltage >= BMS_ProtectParam.OVProtect)
		{
			BMS_AnalysisData.SOC = 1;
		}
		else if (BMS_MonitorData.CellData[0].CellVoltage <= BMS_ProtectParam.UVProtect)
		{
			BMS_AnalysisData.SOC = 0;
		}
	}

	if (BMS_GlobalParam.SysMode == BMS_MODE_CHARGE)
	{
		if(BMS_AnalysisData.CapacityReal >= (BMS_AnalysisData.CapacityRemain + CurrentValue))
		{
			BMS_AnalysisData.CapacityRemain += CurrentValue;
		}
		else
		{
			BMS_AnalysisData.CapacityRemain = BMS_AnalysisData.CapacityReal;
		}
	}
	else if (BMS_GlobalParam.SysMode == BMS_MODE_DISCHARGE)
	{
		if(BMS_AnalysisData.CapacityRemain >= CurrentValue)
		{
			BMS_AnalysisData.CapacityRemain -= CurrentValue;
		}
		else
		{		
			BMS_AnalysisData.CapacityRemain = 0;
		}
	}

	/*
	else  // �Ƿ��Ǿ�̬ʱ��©�����10MA
	{
		if(BMS_AnalysisData.CapacityRemain >= 0.01)   
		{
		 	BMS_AnalysisData.CapacityRemain -= 0.01;
		}
		else
		{
		 	BMS_AnalysisData.CapacityRemain = 0;
		}
	}
	*/

	BMS_AnalysisData.SOC = BMS_AnalysisData.CapacityRemain / BMS_AnalysisData.CapacityReal;
	if (BMS_AnalysisData.SOC > 1)
	{
		BMS_AnalysisData.SOC = 1;
	}
}

// soc���
static void BMS_AnalysisSocCheck(void)
{
	BMS_AnalysisOcvSocCalculate();
	BMS_AnalysisAHSocCalculate();
}


// ������SOC�ϵ��ʼ��
static void BMS_AnalysisCapAndSocInit(void)
{
	uint16_t temp = BMS_MonitorData.CellData[0].CellVoltage * 1000;
	
	// soc����
	BMS_AnalysisData.SOC = BMS_AnalysisOcvToSoc(BMS_MonitorData.CellData[0].CellVoltage  * 1000) / 1000.0;

	// ʵ����������������,�漰��������ŵ������㡢�ϻ���ġ��¶��������ߡ���Ϣ�洢ģ��
	BMS_AnalysisData.CapacityReal = BMS_AnalysisData.CapacityRated;

	// ʣ������ = ʵ������ * soc
	BMS_AnalysisData.CapacityRemain = BMS_AnalysisData.CapacityReal * BMS_AnalysisData.SOC; 
}


