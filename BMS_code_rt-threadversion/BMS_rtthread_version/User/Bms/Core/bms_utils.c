#include "bms_utils.h"


// ����һ�����Ƿ����������
int binarySearch(uint16_t *nums, uint8_t left, uint8_t right, uint16_t target)  
{
    while (left <= right) 
	{ 
		// ע��
        int mid = (right + left) / 2;
			
        if(nums[mid] == target)
		{
            return mid; 
		}
        else if (nums[mid] < target)
		{
            left = mid + 1; // ע��
		}
        else if (nums[mid] > target)
		{
            right = mid - 1; // ע��
        }
	}
    return -1;
}

// ����һ�����������е��Ҳ�߽�(���ַ�)
// start_pos����ʼλ��
// end_pos������λ��
// ����-1����ʾ�����������
int right_bound(uint16_t *nums, uint8_t start_pos, uint8_t end_pos, uint16_t target) 
{
	uint16_t left = start_pos;
	uint16_t right = end_pos;
		
    while (left < right)
	{
        int mid = (left + right) / 2;
		
        if (nums[mid] == target) 
		{
            left = mid + 1; // ע��
        } 
		else if (nums[mid] < target) 
		{
            left = mid + 1;
        }
		else if (nums[mid] > target) 
		{
            right = mid;
        }
    }
	if ((left - 1) < start_pos)
	{
		return -1;
	}
    return left - 1; // ע��
}

// ����һ�����������е����߽�(���ַ�)
// start_pos����ʼλ��
// end_pos������λ��
// ����-1����ʾ�����������
int left_bound(uint16_t *nums, uint16_t start_pos, uint16_t end_pos, uint16_t target) 
{
	uint16_t left = start_pos;
	uint16_t right = end_pos;
	
    while (left < right) 
	{ 
		// ע��
        int mid = (left + right) / 2;
		
        if (nums[mid] == target) 
		{
            right = mid;
        }
		else if (nums[mid] < target) 
		{
            left = mid + 1;
        } 
		else if (nums[mid] > target) 
		{
            right = mid; // ע��
        }
    }
	
	if (left > end_pos)
	{
		return -1;
	}
    return left;
}



// ð������float����
void BubbleFloat(float a[], uint32_t n)
{
	float t;
    uint32_t i, j;
      
    for (i = 1; i < n; i++)
    {
        for (j = 0; j < n-i; j++)
        {
            if (a[j] > a[j+1])
            {
                t = a[j];
                a[j] = a[j+1];
                a[j+1] = t;
            }
        }
    }
}

/*
void bubble(int a[],int n)
{
    int i,j,t;
    
    for (i = 0; i < n-1; i++)
    {
        for (j = 0; j < n-i-1; j++)
        {
            if (a[j] > a[j+1])
            {
                t = a[j];
                a[j] = a[j+1];
                a[j+1] = t;
            }
        }
    }
}
*/







int cmp_int8_t(const void *e1, const void *e2)
{
    return *(int8_t *)e1 - *(int8_t *)e2;
}

int cmp_uint8_t(const void *e1, const void *e2)
{
    return *(uint8_t *)e1 - *(uint8_t *)e2;
}

int cmp_int16_t(const void *e1, const void *e2)
{
    return *(int16_t *)e1 - *(int16_t *)e2;
}

int cmp_uint16_t(const void *e1, const void *e2)
{
    return *(uint16_t *)e1 - *(uint16_t *)e2;
}

int cmp_float(const void *e1, const void *e2)
{
	if (*(float *)e1 > *(float *)e2)
	{
		return 1;
	}

    return 0;    
}

int cmp_double(const void *e1, const void *e2)
{
	if (*(double *)e1 > *(double *)e2)
	{
		return 1;
	}

    return 0;
}


// ����Ԫ��,��������
void swap(uint8_t *buf1, uint8_t *buf2, uint32_t width)
{
	uint8_t temp;
    uint32_t i;
    
    for (i = 0; i < width; i++)
    {
        temp = *buf1;
        *buf1 = *buf2;
        *buf2 = temp;
        buf1++;
        buf2++;
    }
}


// ð������,��������
// base��	����ַ
// sz:		Ҫ����Ԫ�ظ���
// width:	����Ԫ�صĿ��
// cmp:		����ȷ���͵������,�������ݵĶԱȽ���������û����
//			���e1��e2����cmpӦ���ش���0����,��֮�򷵻�С�ڵ���0����
void BubbleSort(void *base, uint32_t sz, uint32_t width, int (*cmp)(void *e1, void *e2))
{
    uint32_t i = 0, j = 0;

    for (i = 1; i < sz; i++)
    {
        for (j = 0; j < sz - i; j++)
        {
            if (cmp((uint8_t *)base + j * width, (uint8_t *)base + (j + 1) * width) > 0)
            {
                swap((uint8_t *)base + j * width, (uint8_t *)base + (j + 1) * width, width);
            }
        }
    }
}





/*

������������������ð����Ĳ��Դ���



#define BMS_CELL_MAX	5
#define BMS_TEMP_MAX	2


typedef struct
{
	float CellVoltage; 	// ��о��ѹ
	uint8_t CellNumber;	// ��о�ı��
}BMS_CellDataTypedef;

typedef struct
{
	float CellTemp[BMS_TEMP_MAX];					// �����¶�,�¶����ݻ��С��������
	float BatteryVoltage;							// ����ܵ�ѹ
	float BatteryCurrent;							// ��������
	BMS_CellDataTypedef CellData[BMS_CELL_MAX]; 	// ��о��ѹ,��ѹ���ݻ��С��������
}BMS_SampleDataTypedef;


int cmp1(void *e1, void *e2)
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

 
int main()
{
	uint8_t index;
	
	BMS_SampleDataTypedef SampleData;
	
	SampleData.CellData[0].CellNumber = 0;
	SampleData.CellData[1].CellNumber = 1;
	SampleData.CellData[2].CellNumber = 2;
	SampleData.CellData[3].CellNumber = 3;
	SampleData.CellData[4].CellNumber = 4;
	
	
	SampleData.CellData[0].CellVoltage = 3.32;
	SampleData.CellData[1].CellVoltage = 3.12;
	SampleData.CellData[2].CellVoltage = 3.59;
	SampleData.CellData[3].CellVoltage = 3.57;
	SampleData.CellData[4].CellVoltage = 4.20;
	
	
	BubbleSort(SampleData.CellData, BMS_CELL_MAX, sizeof(BMS_CellDataTypedef), cmp1);
	
	for (index = 0; index < BMS_CELL_MAX; index++)
	{
		printf("%f\r\n", SampleData.CellData[index].CellVoltage);
	}
	printf("\r\n");
	
    return 0;
}

*/
