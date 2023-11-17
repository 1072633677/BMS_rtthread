以朱老师物联网大课堂中的BMS代码为基础做出一定的优化和修改，增添了SOP等部分代码

BMS功能：管理多节锂电池组成的电池包，实现充放电管理，安全保护，信息监控。该项目
使用STM32F103+BQQ76920方案，是一个简易的一体化电池管理系统，目前只能管理5块电
池，原版代码只有SOH部分。


业务逻辑如下：

BMS_MonitorInit();	// 电池监控初始化      
创建一个任务BMS_MonitorTaskEntry

BMS_MonitorTaskEntry                   优先级9
                               
	BMS_MonitorBattery();                       //监控电池各项数据
		Bms_HalMonitorCellVoltage();
            ---->更新每一个电芯电压，再将这些电芯电压       
            按从小到大顺序排好，更新电芯电压的方式是从相关的寄存器中直接读
			BQ76920会自己采集电压模拟值再转化为数字值存在相应寄存器中。
		Bms_HalMonitorBatteryVoltage();
        ---->读取电池组总电压值
            V(BAT) = 4 x GAIN x ADC(cell) + (#Cells x OFFSET)    
            芯片手册规定的
		Bms_HalMonitorCellTemperature();
            --->更新每一个电芯的温度，并按从小到大顺序排序。
            ---->BQ76920需要搭配特定的热敏电阻，BQ76920只能求的热敏电阻的阻值，这个阻值对应的温度要在热敏电阻表中查得。
            ---->读取相应的寄存器根据数据手册换算得到对应的电压值电阻值
				
		Bms_HalMonitorBatteryCurrent();
            ----->读取相关寄存器的值，取补码乘上8.44得到模拟电压值，除以精确电阻得到电流值,获取电流的方法有数据手册规定
				
			
	BMS_MonitorSysMode();                   //系统模式监控
			---------->根据电流的大小进行，休眠或充放电状态的切换。从待机模式进入睡眠模式需要检验是否有电芯在均衡，以及小电流持续一段时间后再睡眠。
    



BMS_ProtectInit();	// 电池保护初始化
创建两个任务BMS_ProtectTimerEntry，BMS_ProtectTaskEntry

BMS_ProtectTaskEntry---         优先级10         
	BMS_ProtectSwMonitor();                //软件保护监控
        充电监控------->对过流，过温，低温情况进行监控
		放电监控------->对过温，低温 情况进行监控
	BMS_ProtectHwMonitor();                //硬件保护监控
        根据alert标志位开启定时器计时，切换protect标志位状态
			
	BMS_ProtectRelieveWait    //保护解除等待或监控恢复条件
        根据alert标志位判断条件，如果已经脱离异常情况，将protect标志位置位relieve情况
			
	BMS_ProtectRelieve   //重新开始充电或放电，再次进入监控模式 
        重新回到充放电状态，再次进入监控模式
			
BMS_ProtectTimerEntry            优先级20  单次触发并要使用软件定时器
        是软件定时器的回调函数，保护计时超时后回调该函数再次进入监控模式       


BMS_AnalysisInit();	// 电池分析初始化
    创建了一个任务BMS_AnalysisTaskEntry

BMS_AnalysisTaskEntry              优先级11
	BMS_AnalysisCapAndSocInit()
    SOC值初始化，通过开路电压法获取，在这里没有考虑SOH，将额定满容量和实际满容量视为一个
			
	BMS_AnalysisEasy();
        计算获得最大电压差，平均电压，实时功率，最大最小电压
			
	BMS_AnalysisCalCap();
        温度校准，根据不同的温度获取电池容量。温度与容量的变化参数是电池本身的特性决定。
			
	BMS_AnalysisSocCheck();
        BMS_AnalysisOcvSocCalculate();         //开路电压法计算SOC
            通过三元锂电池的SOC计算数据表求SOC
		BMS_AnalysisAHSocCalculate();         //安时积分法计算SOC   
            对电流进行时间积分
			


BMS_EnergyInit();	// 能量管理初始化
创建了两个任务  BMS_EnergyTaskEntry，BMS_BalanceTimerEntry

BMS_EnergyTaskEntry             优先级12
	该任务首先要获取电池均衡的信号量，无法获取信号量则该任务挂起。
			
	BMS_EnergyChgDsgManage();   //电池充放电管理
        根据SOC的情况判断何时开始充放电，开始充电前检查是否处于均衡状态，通过串口输入可以快速控制充放电
			
	BMS_EnergyBalanceManage(); //电池均衡管理
        等待电压回升，避免刚充放电或均衡完毕后马上均衡，如果符合均衡条件则获取信号量，检验哪些电芯可以参与均衡，由于BQ76920性质，相邻电芯不能同时参与被动均衡。确定哪些电芯可以参与均衡则将相关变量写到BQ76920的指定寄存器中开始均衡。
			

BMS_BalanceTimerEntry          优先级  20
	均衡计数定时器回调入口，是启动了rt-thread软件计时器情况下，计时时间到了才使用的回调函数(本例程中未使用)
	关闭均衡，清除标志



BMS_InfoInit();		// 信息管理初始化，串口输出实时测量信息
创建一个任务 BMS_InfoTaskEntry,         优先级13
		
BMS_InfoTaskEntry
	BMS_InfoPrintf();  -----------> 实时打印BMS信息



BMS_CommInit();		// 通信管理初始化，可以将485通信或者CAN通信的内容写在这里，暂时不支持
创建一个任务BMS_CommTaskEntry        优先级13
该任务暂时无操作



关于BMS的中断处理：

1.外部中断
BQ76920有能力在出现过流过压等异常时自行触发外部中断，并将不同的标志写入到相应的寄存器中。

层次架构
EXTI15_10_IRQHandler---------->
HAL_GPIO_EXTI_IRQHandler------>  
HAL_GPIO_EXTI_Callback-------->
BQ769X0_AlertHandler----------> 
举例：AlertOps.cc()-----------> BMS_MonitorHwCurrent
通过alert触发外部中断


void EXTI15_10_IRQHandler(void)            //
{

  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);

}

/**
  * @brief  This function handles EXTI interrupt request.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_IRQHandler(uint16_t GPIO_Pin)
{
  /* EXTI line interrupt detected */
  if (__HAL_GPIO_EXTI_GET_IT(GPIO_Pin) != 0x00u)
  {
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);
    HAL_GPIO_EXTI_Callback(GPIO_Pin);
  }
}



void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == BQ769X0_ALERT_Pin)
    {
		BQ769X0_AlertHandler();
    }
}



InitData.AlertOps.ocd 	 = BMS_ProtectHwOCD;
InitData.AlertOps.scd 	 = BMS_ProtectHwSCD;
InitData.AlertOps.ov	 = BMS_ProtectHwOV;
InitData.AlertOps.uv 	 = BMS_ProtectHwUV;	
// 使用硬件中断通知,如果烧写程序后必须重新上下电一次BQ芯片或者复位
InitData.AlertOps.cc 	 = BMS_MonitorHwCurrent;
//InitData.AlertOps.cc 	 = NULL;
// 这两个中断会造成系统故障
// 第一个报警时设备故障,表示BQ芯片有问题了
// 第二个报警可能存在被外界电磁信号干扰造成误判,之前出现过,换了个跟官方一样阻值的电阻就没出现过了
InitData.AlertOps.device = BMS_ProtectHwDevice;
InitData.AlertOps.ovrd 	 = BMS_ProtectHwOvrd;


static BQ769X0_AlertOpsTypedf AlertOps;


//SYS_STAT寄存器的值从哪来
SYS_STAT状态位应该是出现了异常时有硬件去自行改变的。在外部中断函数中只需要去读状态位就可以了。

static void BQ769X0_AlertHandler(void)
{
	uint8_t reg_value = 0, write_value = 0;

	BQ769X0_ReadRegisterByteWithCRC(SYS_STAT, &reg_value);                        //SYS_STAT来自register_map的0x00
	if (reg_value & SYS_STAT_OCD_BIT)
	{
		write_value |= SYS_STAT_OCD_BIT;
		if (AlertOps.ocd != NULL) AlertOps.ocd();
	}

	if (reg_value & SYS_STAT_SCD_BIT)
	{
		write_value |= SYS_STAT_SCD_BIT;
		if (AlertOps.scd != NULL) AlertOps.scd();
	}

	if (reg_value & SYS_STAT_OV_BIT)
	{
		write_value |= SYS_STAT_OV_BIT;
		if (AlertOps.ov != NULL) AlertOps.ov();
	}	

	if (reg_value & SYS_STAT_UV_BIT)
	{
		write_value |= SYS_STAT_UV_BIT;
		if (AlertOps.uv != NULL) AlertOps.uv();
	}

	if (reg_value & SYS_STAT_OVRD_BIT)
	{
		write_value |= SYS_STAT_OVRD_BIT;
		if (AlertOps.ovrd != NULL) AlertOps.ovrd();
	}	

	if (reg_value & SYS_STAT_DEVICE_BIT)
	{
		write_value |= SYS_STAT_DEVICE_BIT;
		if (AlertOps.device != NULL) AlertOps.device();
	}
	
	if (reg_value & SYS_STAT_CC_BIT)
	{
		write_value |= SYS_STAT_CC_BIT;
		if (AlertOps.cc != NULL) AlertOps.cc();
	}		

	BQ769X0_WriteRegisterByteWithCRC(SYS_STAT, write_value);
}

void BMS_MonitorHwCurrent(void)         
//这个中断只改变标志位，想通过外部中断引发的任务可以通过标志位做条件判断
{
	FlagSampleIntCur = true;
}








如侵权请联系1072633677@qq.com
