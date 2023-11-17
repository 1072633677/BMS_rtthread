#include "bms_app.h"


#include "bms_hal_control.h"

#include "bms_monitor.h"
#include "bms_protect.h"
#include "bms_analysis.h"
#include "bms_energy.h"
#include "bms_info.h"
#include "bms_comm.h"
#include "bms_global.h"

#include "drv_soft_i2c.h"
#include "drv_softi2c_bq769x0.h"



void BMS_SysInitialize(void)
{
	BQ769X0_InitDataTypedef InitData;

	InitData.AlertOps.ocd 	 = BMS_ProtectHwOCD;
	InitData.AlertOps.scd 	 = BMS_ProtectHwSCD;
	InitData.AlertOps.ov	 = BMS_ProtectHwOV;
	InitData.AlertOps.uv 	 = BMS_ProtectHwUV;	

	// ʹ��Ӳ���ж�֪ͨ,�����д���������������µ�һ��BQоƬ���߸�λ
	InitData.AlertOps.cc 	 = BMS_MonitorHwCurrent;
	//InitData.AlertOps.cc 	 = NULL;

	// �������жϻ����ϵͳ����
	// ��һ������ʱ�豸����,��ʾBQоƬ��������
	// �ڶ����������ܴ��ڱ�������źŸ����������,֮ǰ���ֹ�,���˸����ٷ�һ����ֵ�ĵ����û���ֹ���
	InitData.AlertOps.device = BMS_ProtectHwDevice;
	InitData.AlertOps.ovrd 	 = BMS_ProtectHwOvrd;

	InitData.ConfigData.SCDDelay	 = (BQ769X0_SCDDelayTypedef)INIT_SCD_DELAY;
	InitData.ConfigData.OCDDelay	 = (BQ769X0_OCDDelayTypedef)INIT_OCD_DELAY;
	InitData.ConfigData.UVDelay	 	 = (BQ769X0_OVDelayTypedef)INIT_UV_DELAY;
	InitData.ConfigData.OVDelay	 	 = (BQ769X0_UVDelayTypedef)INIT_OV_DELAY;
	InitData.ConfigData.UVPThreshold = INIT_UV_PROTECT * 1000;
	InitData.ConfigData.OVPThreshold = INIT_OV_PROTECT * 1000;


	// Ӳ����ʼ��
	I2C_BusInitialize();
	BQ769X0_Initialize(&InitData);


	// �����ʼ��
	BMS_MonitorInit();	// ��ؼ�س�ʼ��
	BMS_ProtectInit();	// ��ر�����ʼ��
	BMS_AnalysisInit();	// ��ط�����ʼ��
	BMS_EnergyInit();	// ���������ʼ��
	BMS_InfoInit();		// ��Ϣ�����ʼ��
	BMS_CommInit();		// ͨ�Ź����ʼ��1
}


