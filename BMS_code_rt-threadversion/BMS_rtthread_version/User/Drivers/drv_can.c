#include "drv_can.h"

#include <rtthread.h>
#include <string.h>

#include "main.h"
#include "can.h"



void CAN_Initialize(void)
{
	CAN_FilterTypeDef filter = {0};

	CAN_PwrState(CAN_ENABLE_PWR);
	
    filter.FilterActivation = ENABLE;				// �Ƿ�ʹ��ɸѡ��
    filter.FilterMode = CAN_FILTERMODE_IDMASK;		// ɸѡ��ģʽ
    filter.FilterScale = CAN_FILTERSCALE_32BIT;		// ɸѡ����λ��
    filter.FilterBank = 0;							// ɸѡ�����
    filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;	// ���þ���ɸѡ�����ݴ洢���ĸ����� FIFO
    filter.FilterIdLow = 0;			// CAN_FxR1 �Ĵ����ĵ� 16 λ
    filter.FilterIdHigh = 0;		// CAN_FxR1 �Ĵ����ĸ� 16 λ
    filter.FilterMaskIdLow = 0;		// CAN_FxR2 �Ĵ����ĵ� 16 λ
	filter.FilterMaskIdHigh = 0;	// CAN_FxR2 �Ĵ����ĸ� 16 λ

	// �����˲���
	if (HAL_CAN_ConfigFilter(&hcan, &filter) != HAL_OK)
	{
		rt_kprintf("CAN Config Filter Fail!\r\n");
	}

	// ��CAN RX0�ж�
	HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);  

	// ����CAN
	if (HAL_CAN_Start(&hcan) != HAL_OK)
	{
		rt_kprintf("CAN Start Fail!\r\n");
	}
}


void CAN_SendData(uint32_t IdType,uint32_t StdId,uint32_t ExtId,uint8_t *pData,uint32_t length)
{
    uint32_t TxMailbox;
	uint8_t buffer[8] = {0};
	
    CAN_TxHeaderTypeDef TxHeader;
    
    TxHeader.StdId = StdId;			// ��׼ID
    TxHeader.ExtId = ExtId;			// ��չID
    TxHeader.IDE = IdType;			// ID����
    TxHeader.RTR = CAN_RTR_DATA;	// ����֡/Զ��֡
    TxHeader.DLC = length;			// ���ݳ���
	TxHeader.TransmitGlobalTime = DISABLE;
	
	memcpy(buffer, pData, length);
 
	if (HAL_CAN_AddTxMessage(&hcan, &TxHeader, buffer, &TxMailbox) != HAL_OK)
	{
		rt_kprintf("CAN Send Data Fail!\r\n");
	}
}


void CAN_PwrState(CAN_PwrStateTypedef NewState)
{
	HAL_GPIO_WritePin(CAN_PWREN_GPIO_Port, CAN_PWREN_Pin, (GPIO_PinState)NewState);
}







void CAN_SendTest(void)
{
	static uint8_t send_data[8] = {0, 1, 2, 3, 4, 5, 6, 7};
	static uint8_t id_type_std = 0;


	send_data[0]++;
	send_data[1]++;
	send_data[2]++;
	send_data[3]++;
	send_data[4]++;
	send_data[5]++;
	send_data[6]++;
	send_data[7]++;
	
	if(id_type_std == 1)
	{
		CAN_SendData(CAN_ID_STD, 0x123, 0x321, send_data, 8);
		id_type_std = 0;
	}
	else
	{
		CAN_SendData(CAN_ID_EXT, 0x123, 0x321, send_data, 8);
		id_type_std = 1;
	}
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	uint8_t buffer[8], i;
	CAN_RxHeaderTypeDef hCAN1_RxHeader;

    if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &hCAN1_RxHeader, buffer) == HAL_OK)
    {
        rt_kprintf("\nGet Rx Message Success!!\r\nData:");
        
        for(i=0; i<8; i++)
        {
            rt_kprintf("%d", buffer[i]);
       	}
    }
}

