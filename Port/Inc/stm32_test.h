/*
 * stm32_test.h
 *
 *  Created on: Mar 23, 2025
 *      Author: GAOTIANHAO
 */

#ifndef INC_STM32_TEST_H_
#define INC_STM32_TEST_H_

#include "stm32_us_timer.h"
#include "stm32_dc_dc.h"
#include "stm32_message.h"
#include "stm32_dac.h"
#include "stm32_mk1031.h"
#include "hw_port_mk1031_wrapper.h"
#include "flt_fir_hilbert.h"
#include "SEGGER_SYSVIEW.h"
#include "SEGGER_SYSVIEW_Conf.h"


namespace stm32_test
{
  Hardware_STM32_HRTIM_PWM g_hrtimer_pwm_handler;
  Hardware_STM32_ADC g_adc_handler;
  Hardware_STM32_ADC_Wrapper g_adc_wrapper;
  Hardware_STM32_Relay g_relay_handler;
  Hardware_STM32_Message g_message_handler;
  Hardware_STM32_Message g_modbus_message_handler;
  Algorithim_DC_Buck<Hardware_STM32_HRTIM_PWM, Hardware_STM32_ADC_Wrapper> g_dc_buck_adc_handler;
  Algorithim_DC_Buck<Hardware_STM32_HRTIM_PWM, Hardware_MK1031_Wrapper> g_dc_buck_sensor_handler;
  Algorithim_PID g_voltage_pid;
  Algorithim_PID g_current_pid;
  Algorithim_PID g_pll_pid;
  Hardware_STM32_DAC<200> g_dac_ch1_handler;
  Hardware_STM32_DAC<200> g_dac_ch2_handler;
  Hardware_STM32_US_Timer g_us_timer_handler;
  Flt_Fir_Hilbert g_filiter_hilbert_handler;
  Hardware_MK1031 g_mk1031_sensor_handler;
  Hardware_MK1031_Wrapper g_mk1031_wrapper_handler;

  enum power_control_mode_t
  {
    VOLTAGE_CLOSE_LOOP,
    CURRENT_CLOSE_LOOP
  };

  enum vofa_isResetPID_bool_t
  {
    NO_RESET_PID,
    RESET_PID
  };

  enum vofa_isOutput_bool_t
  {
    OUTPUT_STOP,
    OUTPUT_START
  };

  struct target_vofa_set_value_t
  {
    float target_voltage;
    float target_current;
    float target_phase;
  };


  target_vofa_set_value_t g_target_vofa_set={0};
  vofa_isResetPID_bool_t g_bool_isResetPID = NO_RESET_PID;
  vofa_isOutput_bool_t g_bool_isOutput = OUTPUT_STOP;
  power_control_mode_t g_power_control_mode = VOLTAGE_CLOSE_LOOP;
  Algorithim_PID g_voltage_pid_vofa_set;
  Algorithim_PID g_current_pid_vofa_set;
  Algorithim_PID g_pll_pid_vofa_set;


  /*函数名: vofaReceiveCallback
   * 闭环控制参数设置回调函数
   * */
  void vofaReceiveCallback(uint8_t *data,uint16_t len)
  {
    float temp;
    char s[20];
    memcpy(s,data,len);
    s[len]='\0';
    if (sscanf(s, "VP=%f", &temp) >0)
      {
	g_voltage_pid_vofa_set.kp=temp;
	printf("VP: %f\n", g_voltage_pid_vofa_set.kp);
      }
    else if (sscanf(s, "VI=%f", &temp) >0)
      {
	g_voltage_pid_vofa_set.ki=temp;
	printf("VI: %f\n",g_voltage_pid_vofa_set.ki);
      }
    else if (sscanf(s, "VD=%f", &temp) >0)
      {
	g_voltage_pid_vofa_set.kd=temp;
	printf("VD: %f\n",g_voltage_pid_vofa_set.kd);
      }
    else if (sscanf(s, "CP=%f", &temp) >0)
      {
	g_current_pid_vofa_set.kp=temp;
	printf("CP: %f\n", g_current_pid_vofa_set.kp);
      }
    else if (sscanf(s, "CI=%f", &temp) >0)
      {
	g_current_pid_vofa_set.ki=temp;
	printf("CI: %f\n", g_current_pid_vofa_set.ki);
      }
    else if (sscanf(s, "CD=%f", &temp) >0)
      {
	g_current_pid_vofa_set.kd=temp;
	printf("CD: %f\n", g_current_pid_vofa_set.kd);
      }
    else if (sscanf(s, "V=%f", &temp)>0)
      {
	g_target_vofa_set.target_voltage = temp;
	printf("V: %f\n", g_target_vofa_set.target_voltage);
      }
    else if (sscanf(s, "C=%f", &temp)>0)
      {
	g_target_vofa_set.target_current = temp;
	printf("C: %f\n", g_target_vofa_set.target_current);
      }
    else if (sscanf(s, "VL=%f", &temp)>0)
      {
	g_voltage_pid_vofa_set.integral_limit=temp;
	printf("VL: %f\n", g_voltage_pid_vofa_set.integral_limit);
      }
    else if (sscanf(s, "CL=%f", &temp) >0)
      {
	g_current_pid_vofa_set.integral_limit=temp;
	printf("CL: %f\n", g_current_pid_vofa_set.integral_limit);
      }
    else if (sscanf(s, "PP=%f", &temp)>0)
      {
	g_pll_pid_vofa_set.kp = temp;
	printf("PP: %f\n", g_pll_pid_vofa_set.kp);
      }
    else if (sscanf(s, "PI=%f", &temp)>0)
      {
	g_pll_pid_vofa_set.ki = temp;
	printf("PI: %f\n", g_pll_pid_vofa_set.ki);
      }
    else if (sscanf(s, "PD=%f", &temp)>0)
      {
	g_pll_pid_vofa_set.kd=temp;
	printf("PD: %f\n", g_pll_pid_vofa_set.kd);
      }
    else if (sscanf(s, "PL=%f", &temp) >0)
      {
	g_pll_pid_vofa_set.integral_limit=temp;
	printf("PL: %f\n", g_pll_pid_vofa_set.integral_limit);
      }
    else if (sscanf(s, "PHA=%f", &temp) >0)
      {
	g_target_vofa_set.target_phase=temp;
	printf("PHA: %f\n", g_target_vofa_set.target_phase);
      }
    else if (sscanf(s, "MODE=%f", &temp) >0)
      {
	if (temp == 0)
	  {
	    printf("VOLTAGE MODE\n");
	    g_power_control_mode = VOLTAGE_CLOSE_LOOP;
	  }
	else if (temp == 1)
	  {
	    printf("CURRENT MODE\n");
	    g_power_control_mode = CURRENT_CLOSE_LOOP;
	  }
      }
    else if (sscanf(s, "EN=%f", &temp) >0)
      {

	if (temp == 0)
	  {
	    g_bool_isOutput = OUTPUT_STOP;
	    printf("OUTPUT STOP\n");
	  }
	else if (temp == 1)
	  {
	    g_bool_isOutput = OUTPUT_START;
	    printf("OUTPUT START\n");
	  }
      }
    else if (strcmp (s, "RESET") == 0)
      {
	printf("RESET\n");
	HAL_NVIC_SystemReset();
      }
    else if (strcmp (s, "CLEAR") == 0)
      {
	g_voltage_pid_vofa_set.kp = 0;
	g_voltage_pid_vofa_set.ki = 0;
	g_voltage_pid_vofa_set.kd = 0;
	g_voltage_pid_vofa_set.integral_limit = 0;
	g_current_pid_vofa_set.kp = 0;
	g_current_pid_vofa_set.ki = 0;
	g_current_pid_vofa_set.kd = 0;
	g_bool_isResetPID = RESET_PID;
	printf("RESET PID\n");
      }
    else if(strcmp (s, "DEBUG") == 0)
      {
	//数据回显
	printf("Vin:%f\n",g_dc_buck_adc_handler.dataWrapper_->readVin());
	printf("Vout:%f\n",g_dc_buck_adc_handler.dataWrapper_->readVout());
	printf("Current:%f\n",g_dc_buck_adc_handler.dataWrapper_->readCurrent());
	printf("%d\n",g_dc_buck_adc_handler.isEnable());
      }
  }


  /*函数名: timerA_pwm_test
   * 测试高级定时器a互补pwm是否正常输出
   * */
  void timerA_pwm_test ()
  {
    g_hrtimer_pwm_handler=stm32_hrtim_pwm::getTimerAOutput();
    g_hrtimer_pwm_handler.setDutyCycle(0.3);
    g_hrtimer_pwm_handler.setOutput();
    g_hrtimer_pwm_handler.setFrequency(5000);
  }

  /*函数名: timerB_pwm_test
   * 测试高级定时器B互补pwm是否正常输出
   * */
  void timerB_pwm_test ()
  {
    g_hrtimer_pwm_handler=stm32_hrtim_pwm::getTimerBOutput();
    g_hrtimer_pwm_handler.setDutyCycle(0.8);
    g_hrtimer_pwm_handler.setOutput();
  }

  /*函数名: timerC_pwm_test
   * 测试高级定时器C互补pwm是否正常输出
   * */
  void timerC_pwm_test ()
  {
    g_hrtimer_pwm_handler=stm32_hrtim_pwm::getTimerCOutput();
    g_hrtimer_pwm_handler.setDutyCycle(0.3);
    g_hrtimer_pwm_handler.setOutput();
  }


  /*函数名: adc1_dma_test
   * 测试adc规则组dma是否正常工作
   * */
  void adc1_dma_test ()
  {
    g_message_handler=stm32_message::getUART1();
    g_message_handler.startReceive();
    g_adc_handler=stm32_adc::getADC1();
    g_adc_handler.startSample();
    while(1)
      {
	//0:vin 1:vout 2:current
	static float data[3]={0};
	g_adc_handler.read3Channel(data,3);
	printf("Vin:%f\n",data[0]);
	printf("Vout:%f\n",data[1]);
	printf("Current:%f\n",data[2]);
      }
  }


  /*函数名: adc_it_test
   * 测试adc注入组中断是否正常工作
   * */
  void adc1_it_test()
  {
    //adc注入组测试需要先开启PWM输出
    timerA_pwm_test();
    g_adc_handler=stm32_adc::getADC1();
    g_adc_handler.startSample_IT();
    while(1)
      {
	static float data[3] = {0};
	g_adc_handler.read3Channel_IT(data,3);
      }
  }

  /*函数名: adc2_dma_test
   * 测试adc2+dma是否正常工作
   * */
  void adc2_dma_test ()
  {
    g_adc_handler = stm32_adc::getADC2 ();
    g_adc_handler.startSample ();
    while (1)
      {
	//0:vin 1:vout 2:current
	static float data[3]={0};
	g_adc_handler.read3Channel(data,3);
	printf("Vin:%f\n",data[0]);
	printf("Vout:%f\n",data[1]);
	printf("Current:%f\n",data[2]);
      }
  }

  /*函数名: adc2_it_test
   * 测试adc注入组中断是否正常工作
   * */
  void adc2_it_test()
  {
    //adc注入组测试需要先开启PWM输出
    timerB_pwm_test();
    g_adc_handler = stm32_adc::getADC2 ();
    g_adc_handler.startSample_IT ();
    while(1)
      {
	static float data[3] = {0};
	g_adc_handler.read3Channel_IT(data,3);
      }
  }

  //adc3没有设置注入组，因为adc3只有两个通道，直接用dma就行
  /*函数名: adc3_dma_test
   * 测试adc3+dma是否正常工作
   * */
  void adc3_dma_test ()
  {
    g_adc_handler = stm32_adc::getADC3 ();
    g_adc_handler.startSample ();
    while (1)
      {
	//0:vin 1:vout 2:current
	static float data[2]={0};
	g_adc_handler.read2Channel(data,2);
	printf("V_Battery:%f\n",data[0]);

      }
  }

  void dac1_dma_test()
  {
    g_dac_ch1_handler=stm32_dac::getDAC1_CH1();
    g_dac_ch1_handler.enable();
    g_dac_ch2_handler=stm32_dac::getDAC1_CH2();
    g_dac_ch2_handler.enable();
    while(1){}
  }

  void filiter_hilbert_it_singlePoint_test()
  {
    float hilbert_signal=g_filiter_hilbert_handler.process(g_dac_ch1_handler.getOutputValue() );
    uint32_t output= static_cast<uint16_t>(hilbert_signal);
    g_dac_ch2_handler.update_diy(output);
    g_dac_ch2_handler.enable();
  }

  void us_timer_test()
  {
    g_us_timer_handler=stm32_us_timer::getUsTimer();
    g_us_timer_handler.on();
    while(1)
      {
	static uint32_t us=g_us_timer_handler.getUs();
      }
  }

  void pll_init_test()
  {
    g_message_handler=stm32_message::getUART1();
    g_message_handler.attachEvent(vofaReceiveCallback,PINGPONG_BUFFER);
    g_message_handler.startReceive();

    //    g_pll_pid.begin(0.1,0.001,0,0.0016);
    g_pll_pid.begin(43.59,0.8242,0,0.0847);
    g_dac_ch1_handler=stm32_dac::getDAC1_CH1();

    g_dac_ch2_handler=stm32_dac::getDAC1_CH2();
    g_dac_ch1_handler.enable();
    g_dac_ch2_handler.enable();
  }

  //TO DO :
  //两路DAC的时钟触发源可以是同一个定时器
  //将计算出的相位传到锁相环的实现去
  //将锁相环在这里实现，即锁相环的频率与采样频率一致 - 10khz
  void pll_it_test()
  {
    constexpr uint32_t signal_frequence = 50;
    constexpr uint32_t fs=10000;
    uint32_t point_ref = g_dac_ch1_handler.getOutputValue();
    uint32_t point_pll =g_dac_ch2_handler.getOutputValue();

    //    g_us_timer_handler=stm32_us_timer::getUsTimer();
    //    g_us_timer_handler.on();

    static uint32_t point_ref_last = point_ref;
    static uint32_t point_pll_last = point_pll;
    static float phase=0;
    //    static uint32_t t1=0;
    //    static uint32_t t2=0;
    static uint32_t delta_points=0;
    static bool isFindZero=false;

    if(point_ref > 2047.5 && point_ref_last < 2047.5 && isFindZero==false)
      {
	isFindZero=true;
      }

    if(isFindZero == true)
      {
	delta_points++;
      }

    if(point_pll > 2047.5 && point_pll_last < 2047.5 && isFindZero==true)
      {
	phase = 2 * PI * signal_frequence * delta_points / fs ;
	delta_points=0;
	isFindZero=false;
      }
    //TO DO 计算锁相环 - 锁相环输出为要调整的相位
    if (g_bool_isResetPID == RESET_PID)
      {
	g_pll_pid.kp=g_pll_pid_vofa_set.kp;
	g_pll_pid.ki=g_pll_pid_vofa_set.ki;
	g_pll_pid.kd=g_pll_pid_vofa_set.kd;
	g_pll_pid.integral_limit=g_pll_pid_vofa_set.integral_limit;
      }
    float output_pll=g_pll_pid.cal_absolute(0, phase);
    g_dac_ch1_handler.update_sin(50, 200, g_target_vofa_set.target_phase);
    //    g_dac_ch2_handler.update_sin(abs(output_pll), 200);
    g_dac_ch2_handler.update_sin(abs(output_pll), 200);


    point_ref_last = point_ref;
    point_pll_last = point_pll;
  }
  void filiter_hilbert_test()
  {
    g_dac_ch1_handler=stm32_dac::getDAC1_CH1();
    g_dac_ch1_handler.enable();
    //    g_dac_ch2_handler.begin(&hdac1,&htim7,STM32_DAC_CHANNEL2,200,STM32_DAC_VAR_MODE);
    g_dac_ch2_handler=stm32_dac::getDAC1_CH2();
    g_dac_ch2_handler.enable();
    while(1){}
  }


  void dq_pll_test()
  {
    g_dac_ch1_handler=stm32_dac::getDAC1_CH1();
    g_dac_ch1_handler.enable();

    g_dac_ch2_handler.begin(&hdac1,&htim7,STM32_DAC_CHANNEL2,50,200,STM32_DAC_VAR_MODE);
    g_dac_ch2_handler.enable();

    while(1)
      {
	//TO DO
      }
  }

  void mk1031_sensor_test()
  {
    g_message_handler=stm32_message::getUART2();
    g_mk1031_sensor_handler=stm32_mk1031::getMK1031(&g_message_handler, 1);
    //在对应的串口中断回调使用stm32_message的callbackhandler，并注册回调函数
    g_message_handler.attachEvent([](uint8_t *data, uint16_t len)
				  {
      g_mk1031_sensor_handler.responseHandler(data,len);
				  },PINGPONG_BUFFER);
    g_message_handler.startReceive();

    g_mk1031_wrapper_handler.init(&g_mk1031_sensor_handler);

    //定时事件处理，处理串口空闲中断接收到的数据
    HAL_TIM_Base_Start_IT(&htim1);

    static float voltage=0;

    while(1)
      {
	g_mk1031_sensor_handler.readRegisters(MK1031_VOLTAGE,3);
	HAL_Delay(10);
	voltage=g_mk1031_wrapper_handler.readVout();
      }
  }


  /*函数名: dc_dc_openLoop_test
   * 测试dcdc降压开环是否正常工作
   * */
  void dc_dc_openLoop_test()
  {
    g_message_handler=stm32_message::getUART1();
    g_message_handler.startReceive();
    //高级定时器抽象层初始化
    g_hrtimer_pwm_handler=stm32_hrtim_pwm::getTimerAOutput();
    g_hrtimer_pwm_handler.setOutput();
    g_relay_handler=stm32_relay::getRelay1();
    g_dc_buck_adc_handler=stm32_dc_dc::getDCBuckADC(&g_hrtimer_pwm_handler);
    g_dc_buck_adc_handler.setVin(12);

    g_message_handler.attachEvent(vofaReceiveCallback,PINGPONG_BUFFER);
    while (1)
      {
	if(g_bool_isOutput == OUTPUT_START)
	  {
	    g_dc_buck_adc_handler.enable();
	  }
	else
	  {
	    g_dc_buck_adc_handler.disable();
	  }
	g_dc_buck_adc_handler.setVout(g_target_vofa_set.target_voltage);
	g_dc_buck_adc_handler.openVoltageLoopControl();
      }
  }

  /*函数名: dc_dc_voltageClosedLoop_test
   * 测试dcdc降压是否正常工作
   * */
  void dc_dc_voltageClosedLoop_test()
  {
    g_adc_handler=stm32_adc::getADC1();
    g_adc_wrapper.init(&g_adc_handler);
    //    g_adc_wrapper.create_mapping(STM32_ADC_WRAPPER_CHANNEL_ID1, STM32_ADC_WRAPPER_VOUT);
    g_mk1031_wrapper_handler.init(&g_mk1031_sensor_handler);
    //TO DO
    g_dc_buck_adc_handler=stm32_dc_dc::getDCBuckADC(&g_hrtimer_pwm_handler,&g_adc_wrapper);
    g_dc_buck_adc_handler.setVin(5);
    g_dc_buck_adc_handler.setVout(3.3);
    g_voltage_pid.begin(0, 0, 0);
    g_dc_buck_adc_handler.setCV_PID(&g_voltage_pid);
    g_dc_buck_adc_handler.enable();

    //定时事件处理，处理串口空闲中断接收到的数据以及发送modbus采样
    HAL_TIM_Base_Start_IT(&htim1);


    while (1)
      {
	g_dc_buck_adc_handler.closedVoltageLoopControl();
      }
  }

  /*函数名: dc_dc_voltageClosedLoop_test
   * 测试dcdc降压是否正常工作
   * */
  void dc_dc_currentClosedLoop_test()
  {
    //蓝牙调试串口抽象层初始化
    g_message_handler=stm32_message::getUART1();
    g_modbus_message_handler.attachEvent(vofaReceiveCallback,PINGPONG_BUFFER);
    g_message_handler.startReceive();
    //mk1031传感器初始化
    g_modbus_message_handler=stm32_message::getUART2();
    g_mk1031_sensor_handler=stm32_mk1031::getMK1031(&g_modbus_message_handler, 1);
    //在对应的串口中断回调使用stm32_message的callbackhandler，并注册回调函数
    g_modbus_message_handler.attachEvent([](uint8_t *data, uint16_t len)
					 {
      g_mk1031_sensor_handler.responseHandler(data,len);
					 },PINGPONG_BUFFER);
    g_modbus_message_handler.startReceive();

    g_mk1031_wrapper_handler.init(&g_mk1031_sensor_handler);

    //定时事件处理，处理串口空闲中断接收到的数据
    HAL_TIM_Base_Start_IT(&htim1);
    //TO DO
    g_dc_buck_sensor_handler=stm32_dc_dc::getDCBuckMK1031(&g_hrtimer_pwm_handler,&g_mk1031_wrapper_handler);
    g_dc_buck_sensor_handler.setCurrent(1);
    g_voltage_pid.begin(0, 0, 0);
    g_dc_buck_sensor_handler.setCV_PID(&g_current_pid);
    g_dc_buck_sensor_handler.enable();
    while (1)
      {
	if (g_bool_isResetPID == RESET_PID)
	  {
	    g_dc_buck_sensor_handler.cc_pid_->kp=g_current_pid_vofa_set.kp;
	    g_dc_buck_sensor_handler.cc_pid_->ki=g_current_pid_vofa_set.ki;
	    g_dc_buck_sensor_handler.cc_pid_->kd=g_current_pid_vofa_set.kd;
	    g_dc_buck_sensor_handler.cc_pid_->integral_limit=g_current_pid_vofa_set.integral_limit;
	  }

	g_dc_buck_sensor_handler.closedCurrentLoopControl();
      }
  }



  /*函数名: dc_dc_doubleMode_closedLoop_test
   * 测试dcdc降压双闭环是否正常工作
   * */
  void dc_dc_doubleMode_closedLoop_test()
  {
    //串口抽象层初始化
    g_message_handler=stm32_message::getUART1();
    g_message_handler.startReceive();

    //高级定时器抽象层初始化
    g_hrtimer_pwm_handler=stm32_hrtim_pwm::getTimerAOutput();
    g_hrtimer_pwm_handler.setOutput();

    //ADC抽象层初始化
    g_adc_handler=stm32_adc::getADC1();
    __HAL_DMA_DISABLE_IT(&hdma_adc1, DMA_IT_HT);
    //继电器抽象层初始化
    g_relay_handler=stm32_relay::getRelay1();

    //算法抽象层初始化
    //    g_adc_wrapper.init(&g_adc_handler);
    //    g_adc_wrapper.create_mapping(STM32_ADC_WRAPPER_CHANNEL_ID1, STM32_ADC_WRAPPER_VOUT);
    //    g_adc_wrapper.create_mapping(STM32_ADC_WRAPPER_CHANNEL_ID2, STM32_ADC_WRAPPER_CURRENT);
    g_dc_buck_adc_handler=stm32_dc_dc::getDCBuckADC(&g_hrtimer_pwm_handler,&g_adc_wrapper);
    g_dc_buck_adc_handler.setVout(5);
    g_voltage_pid.begin(1, 1, 1);
    g_current_pid.begin(2, 2, 2);
    g_dc_buck_adc_handler.setCV_PID(&g_voltage_pid);
    g_dc_buck_adc_handler.setCC_PID(&g_current_pid);

    g_message_handler.attachEvent(vofaReceiveCallback,PINGPONG_BUFFER);
    g_adc_handler.startSample();
    while (1)
      {
	if(g_bool_isOutput == OUTPUT_START)
	  {
	    g_dc_buck_adc_handler.enable();
	    g_relay_handler.on();
	  }
	else
	  {
	    g_dc_buck_adc_handler.disable();
	    g_relay_handler.off();
	  }

	switch(g_power_control_mode)
	{
	  case VOLTAGE_CLOSE_LOOP:
	    if (g_bool_isResetPID == RESET_PID)
	      {
		g_dc_buck_adc_handler.cv_pid_->kp=g_voltage_pid_vofa_set.kp;
		g_dc_buck_adc_handler.cv_pid_->ki=g_voltage_pid_vofa_set.ki;
		g_dc_buck_adc_handler.cv_pid_->kd=g_voltage_pid_vofa_set.kd;
		g_dc_buck_adc_handler.cv_pid_->integral_limit=g_voltage_pid_vofa_set.integral_limit;
	      }

	    g_dc_buck_adc_handler.closedVoltageLoopControl ();

	    break;
	  case CURRENT_CLOSE_LOOP:
	    if (g_bool_isResetPID == RESET_PID)
	      {
		g_dc_buck_adc_handler.cc_pid_->kp=g_current_pid_vofa_set.kp;
		g_dc_buck_adc_handler.cc_pid_->ki=g_current_pid_vofa_set.ki;
		g_dc_buck_adc_handler.cc_pid_->kd=g_current_pid_vofa_set.kd;
		g_dc_buck_adc_handler.cc_pid_->integral_limit=g_current_pid_vofa_set.integral_limit;
	      }
	    g_dc_buck_adc_handler.closedCurrentLoopControl ();
	    break;
	}
      }
  }

  /*函数名: vofa_send_test
   * 测试串口发送是否正常工作
   * */
  void vofa_send_test()
  {
    printf("good\n");
    HAL_Delay(500);
  }

  /*函数名: vofa_receive_callback_test
   * 测试串口空闲中断是否正常工作
   * */
  void vofa_receive_callback_test(uint8_t *data, uint16_t len)
  {
    for (int i = 0; i < len; i++)
      {
	printf("%c\n", *(data + i));
      }
  }

  /*函数名: vofa_receive_test
   * 测试串口接收回调是否正常
   * */
  void vofa_receive_test()
  {
    g_message_handler=stm32_message::getUART1();
    g_message_handler.attachEvent(vofa_receive_callback_test,PINGPONG_BUFFER);
    g_message_handler.startReceive();
    printf("AT+ROLE=2\r\n");
    while(1)
      {
	g_message_handler.processHandler();
      }
  }

  /*函数名: segger_systemview_test_enable
   * 开启segger系统调试
   * */
  void segger_systemview_test_enable()
  {
    SEGGER_SYSVIEW_Conf();            /* Configure and initialize SystemView  */
    SEGGER_SYSVIEW_Start();           /* Starts SystemView recording*/
    SEGGER_SYSVIEW_OnIdle();          /* Tells SystemView that System is currently in "Idle"*/
  }
}

#endif /* INC_STM32_TEST_H_ */
