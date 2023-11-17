#ifndef __BMS_CONFIG_H__
#define __BMS_CONFIG_H__



#include "bms_hal_config.h"




// ���֧�ֶ��ٽڵ�о
// BQ76920:3~5
// BQ76930:6~10
// BQ76940:9~15
#define BMS_CELL_MAX	5


// ���֧�ּ�·�¶�
// BQ76920:1
// BQ76930:2
// BQ76940:3
#define BMS_TEMP_MAX	1


// �¶Ȳ�����Χ,����ֵ��Ҫ�������������BQоƬ������Χ����
#define BMS_TEMP_MEASURE_MAX	125
#define	BMS_TEMP_MEASURE_MIN	-55

// �������������¶�ֵ���������Χʱ,�������Чֵ������
#define BMS_TEMP_INVALID_VALUE	255

// Ĭ�ϵ�ض����ֵ(A/H)
// ���ֵû��ʵ������������У׼��,�����ҿ�ͷ˵��
#define BMS_BATTERY_CAPACITY	2.2

// �ڴ���ģʽ�¾�ֹ����ʱ�����˯��ģʽ(Min),˯�ߵ͹��Ĵ�����δ����
#define BMS_ENTRY_SLEEP_TIME	60



/***************************** ��ر�����ز��� ***********************************/
// ��Ԫ﮵��(Ternary lithium battery)Ĭ�ϲ���
#define TLB_OV_PROTECT			4.20	// �����ѹ������ѹ
#define TLB_OV_RELIEVE			4.18	// �����ѹ�ָ���ѹ
#define TLB_UV_PROTECT			3.10	// ����Ƿѹ������ѹ
#define TLB_UV_RELIEVE			3.15	// ����Ƿѹ�ָ���ѹ
#define TLB_SHUTDOWN_VOLTAGE	3.08	// �Զ��ػ���ѹ
#define TLB_BALANCE_VOLTAGE		3.30	// ������ʼ��ѹ



// ������﮵��(lithium iron phosphate battery)Ĭ�ϲ���
#define LIPB_OV_PROTECT			3.60	// �����ѹ������ѹ
#define LIPB_OV_RELIEVE			3.55	// �����ѹ�ָ���ѹ
#define LIPB_UV_PROTECT			2.60	// ����Ƿѹ������ѹ
#define LIPB_UV_RELIEVE			2.65	// ����Ƿѹ�ָ���ѹ
#define LIPB_SHUTDOWN_VOLTAGE	2.50 	// �Զ��ػ���ѹ
#define LIPB_BALANCE_VOLTAGE	3.00	// ������ʼ��ѹ


// ����﮵��(Lithium titanate battery)Ĭ�ϲ���
#define LTB_OV_PROTECT			2.70	// �����ѹ������ѹ
#define LTB_OV_RELIEVE			2.65	// �����ѹ�ָ���ѹ
#define LTB_UV_PROTECT			1.80	// ����Ƿѹ������ѹ
#define LTB_UV_RELIEVE			1.85	// ����Ƿѹ�ָ���ѹ
#define LTB_SHUTDOWN_VOLTAGE	1.70	// �Զ��ػ���ѹ
#define LTB_BALANCE_VOLTAGE		2.30	// ������ʼ��ѹ



// ��ʼĬ��ֵ
#define INIT_OV_PROTECT			TLB_OV_PROTECT			// �����ѹ������ѹ(V)(ע��BQ769X0 OV��Χ��3.15~4.70V)
#define INIT_OV_RELIEVE			TLB_OV_RELIEVE			// �����ѹ�ָ���ѹ(V)
#define INIT_UV_PROTECT			TLB_UV_PROTECT			// ����Ƿѹ������ѹ(V)(ע��BQ769X0 UV��Χ��1.58~3.10V)
#define INIT_UV_RELIEVE			TLB_UV_RELIEVE			// ����Ƿѹ�ָ���ѹ(V)

#define INIT_SHUTDOWN_VOLTAGE	TLB_SHUTDOWN_VOLTAGE	// �Զ��ػ���ѹ(V),δ����Ԥ��
#define INIT_BALANCE_VOLTAGE	TLB_BALANCE_VOLTAGE		// ������ʼ��ѹ(V)

#define INIT_BALANCE_CURRENT_MAX	0.6		// ���������(A),δ����Ԥ��
#define	INIT_OCC_MAX				2.2		// ��������(A)
#define	INIT_OCD_MAX				2.2		// ���ŵ����(A)


#define INIT_OV_DELAY		BMS_OV_DELAY_2s		// ����ѹ������ʱʱ��	OV:Over	Voltage
#define INIT_UV_DELAY 		BMS_UV_DELAY_4s		// �ŵ�Ƿѹ������ʱʱ�� UV:Under Voltage

#define INIT_OCD_DELAY		BMS_OCD_DELAY_320ms // �ŵ������ʱʱ��(S) OCD:Over Current Discharge
#define INIT_OCD_RELIEVE	60					// �ŵ�������ʱ��(S)

#define INIT_SCD_DELAY		BMS_SCD_DELAY_100us	// �ŵ��·��ʱʱ��(us) SCD:Short Circuit Discharge
#define INIT_SCD_RELIEVE	60					// �ŵ��·���ʱ��(S)

#define INIT_OCC_DELAY		1		// ��������ʱʱ��(S) OCC:Over Current Charge
#define INIT_OCC_RELIEVE	60		// ���������ʱ��(S)

#define INIT_OTC_PROTECT	70		// �����±���(��) OTC:Over Temperature Charge
#define INIT_OTC_RELIEVE	60		// �����½��(��)

#define INIT_OTD_PROTECT	70		// �ŵ���±���(��) OTD:Over Temperature Discharge
#define INIT_OTD_RELIEVE	60		// �ŵ���½��(��)

#define INIT_LTC_PROTECT	-20		// �����±���(��) LTC:Low Temperature Charge
#define INIT_LTC_RELIEVE	-10		// �����½��(��)

#define INIT_LTD_PROTECT	-20		// �ŵ���±���(��) LTD:Low Temperature Discharge
#define INIT_LTD_RELIEVE	-10		// �ŵ���½��(��)	









#define SOC_STOP_CHG_VALUE		1		// ֹͣ���SOCֵ
#define SOC_START_CHG_VALUE		0.99	// �������SOCֵ
#define SOC_STOP_DSG_VALUE		0		// ֹͣ�ŵ�SOCֵ
#define SOC_START_DSG_VALUE		0.01	// �����ŵ�SOCֵ

#define BALANCE_DIFFE_VOLTAGE	0.05	// ��������ѹ(V)
#define BALANCE_CYCLE_TIME		30		// ��������ʱ��(s)
#define BALANCE_VOLT_RISE_DELAY 5000	// �����ѹ������ʱ(ms)
/*************************************************************************************/


#endif


