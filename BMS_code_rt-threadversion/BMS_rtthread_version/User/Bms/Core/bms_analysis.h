#ifndef __BMS_ANALYSIS_H__
#define __BMS_ANALYSIS_H__


#include <stdint.h>




typedef struct
{
	// ������ֵĿǰ���ù�
	uint8_t SOH;	// ��ذ�SOHֵ			ʵ������/�����
	uint8_t SOP;	// ��ذ�SOPֵ	
	uint8_t SOE;	// ��ذ�SOEֵ


	// Ŀǰ������ֵ��δ��,�Ⱥ�����ʵ����
	uint32_t LoopCount;			// ��ذ�ѭ������(������һ����Ź���+1)
	float CapacityLoop;			// ��ذ�ѭ������(A/H)
	

	// �����ֵ�Ѿ�ʵ����
	float SOC;					// ��ذ�SOCֵ(ʣ������ٷֱ�)

	float AverageVoltage;		// ����ƽ����ѹֵ(V)
	float MaxVoltageDifference;	// �����о����ѹ��(V)
	float PowerReal;			// ��ذ�ʵʱ����(W)
	float CellVoltMax;			// �����о����ѹ
	float CellVoltMin;			// �����о��С��ѹ
	
	float CapacityRated;		// ��ذ������(A/H)
	float CapacityReal;			// ��ذ�ʵʱ����(A/H)  		���㷽���ý���һ�������ĳ�ŵ����
	float CapacityRemain;		// ��ذ�ʣ������(A/H)
}BMS_AnalysisDataTypedef;


extern BMS_AnalysisDataTypedef BMS_AnalysisData;



void BMS_AnalysisInit(void);



#endif

