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
#include "stm32_keyboard.h"
#include "stm32_dc_ac.h"
#include "oled.h"
#include "flt_sogi.h"

namespace stm32_test
{
  Hardware_STM32_HRTIM_PWM g_hrtimer_pwm_handler;
  Hardware_STM32_ADC g_adc_handler;
  Hardware_STM32_ADC_Wrapper g_adc_wrapper;
  Hardware_STM32_Relay g_relay_handler;
  Hardware_STM32_Message g_message_handler;
  Hardware_STM32_Message g_modbus_message_handler;     //低压侧
  Hardware_STM32_Message g_modbus_message_handler_in;  //高压侧
  Algorithim_DC_Buck<Hardware_STM32_HRTIM_PWM, Hardware_STM32_ADC_Wrapper> g_dc_buck_adc_handler;
  Algorithim_DC_Buck<Hardware_STM32_HRTIM_PWM, Hardware_MK1031_Wrapper> g_dc_buck_sensor_handler;
  Algorithim_DC_Boost<Hardware_STM32_HRTIM_PWM, Hardware_MK1031_Wrapper> g_dc_boost_sensor_handler;
  Algorithim_PID g_voltage_pid;
  Algorithim_PID g_current_pid;
  Algorithim_PID g_pll_pid;
  Hardware_STM32_DAC<200> g_dac_ch1_handler;
  Hardware_STM32_DAC<200> g_dac_ch2_handler;
  Hardware_STM32_DAC<400> g_dac_ch1_20khz_handler;
  Hardware_STM32_DAC<400> g_dac_ch2_20khz_handler;
  Hardware_STM32_US_Timer g_us_timer_handler;
  Flt_Sogi g_flt_sogi_handler;
  Flt_Fir_Hilbert g_filiter_hilbert_handler;
  Hardware_MK1031 g_mk1031_sensor_handler;      //低压侧
  Hardware_MK1031 g_mk1031_sensor_handler_in;   //高压侧
  Hardware_MK1031_Wrapper g_mk1031_wrapper_handler;
  Hardware_STM32_Keyboard g_keyboard_handler;
  Algorithim_DC_AC<Hardware_STM32_HRTIM_PWM, Hardware_MK1031_Wrapper> g_dc_ac_sensor_handler;

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

  struct target_oled_set_value_t
  {
    float target_voltage;
    float target_current;
    float target_phase;
  };


  enum oled_menu_mode_t
  {
    OLED_MENU_BUCK,
    OLED_MENU_BOOST,
    OLED_MENU_PAGE_NUM
  };

  enum oled_menu_isSelcet_bool_t
  {
    OLED_MENU_NO_SELECT,
    OLED_MENU_SELECT,
    OLED_MENU_SELCET_NUM
  };

  oled_menu_mode_t g_oled_menu_mode = OLED_MENU_BUCK;
  oled_menu_isSelcet_bool_t g_oled_menu_isSelect = OLED_MENU_NO_SELECT;
  target_oled_set_value_t g_target_vofa_set={0};

  //  target_vofa_set_value_t g_target_vofa_set={0};
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
	//	printf("Vin:%f\n",g_dc_buck_adc_handler.dataWrapper_->readVin());
	//	printf("Vout:%f\n",g_dc_buck_adc_handler.dataWrapper_->readVout());
	//	printf("Current:%f\n",g_dc_buck_adc_handler.dataWrapper_->readCurrent());
	//	printf("%d\n",g_dc_buck_adc_handler.isEnable());


	//	printf("current:%f\n",stm32_test::g_dc_buck_sensor_handler.dataWrapper_->readCurrent());
	printf("Vol_ouput:%f\n",g_dc_boost_sensor_handler.cv_pid_->lastOutput);
	printf("current:%f\n",stm32_test::g_dc_boost_sensor_handler.dataWrapper_->readCurrent());
	printf("Vin:%f\n", stm32_test::g_dc_boost_sensor_handler.dataWrapper_->readVin());
      }
  }


  /*函数名: timerA_pwm_test
   * 测试高级定时器a互补pwm是否正常输出
   * */
  void timerA_pwm_test ()
  {
    g_hrtimer_pwm_handler=stm32_hrtim_pwm::getTimerAOutput();
    g_hrtimer_pwm_handler.setDutyCycle(0.5);
    g_hrtimer_pwm_handler.setOutput();
    g_hrtimer_pwm_handler.setFrequency(40000);
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
   * 测试高级定时器C互补pwm的左对齐是否正常输出
   * */
  void timerC_pwm_test ()
  {
    g_hrtimer_pwm_handler=stm32_hrtim_pwm::getTimerCOutput();
    g_hrtimer_pwm_handler.setDutyCycle(0.3);//频率为50khz
    g_hrtimer_pwm_handler.setOutput();
  }


  /*函数名: timerC_pwm_test
   * 测试高级定时器C互补pwm的中心对齐是否正常输出
   * */
  void timerC_center_align_pwm_test ()
  {
    g_hrtimer_pwm_handler=stm32_hrtim_pwm::getTimerCOutput();
    g_hrtimer_pwm_handler.setDutyCycle(0.3);//频率为25khz
    g_hrtimer_pwm_handler.setOutput_IT();
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
    //请注意，该实验使用TIM6和TIM7来触发DAC的正弦表更新，需要检查对应定时器的频率
    g_dac_ch1_handler=stm32_dac::getDAC1_CH1();
    g_dac_ch1_handler.enable();
    g_dac_ch2_handler=stm32_dac::getDAC1_CH2();
    g_dac_ch2_handler.enable();
    while(1){}
  }

  void dac1_dma_test_20khz()
  {
    //请注意，该实验使用TIM6和TIM7来触发DAC的正弦表更新，需要检查对应定时器的频率，并检查对应的中断回调函数
    g_dac_ch1_20khz_handler=stm32_dac::getDAC1_CH1_20khz();
    g_dac_ch1_20khz_handler.enable();
    g_dac_ch2_20khz_handler=stm32_dac::getDAC1_CH2_20khz();
    g_dac_ch2_20khz_handler.enable();
    float f0=50; //正弦波频率
    float fs=20000; //采样频率
    float sogi_k=1.0f;
    g_flt_sogi_handler.begin(f0,fs,sogi_k);
    while(1){}
  }

  void filiter_sogi_it_test ()
  {
    float sogi_output[2];
    g_flt_sogi_handler.filter(g_dac_ch1_20khz_handler.getOutputValue(),sogi_output);
//    g_dac_ch1_20khz_handler.update_diy(static_cast<uint16_t>(sogi_output[0]));
    g_dac_ch2_20khz_handler.update_diy(static_cast<uint16_t>(sogi_output[0]));
  }

  void filiter_sogi_it_uart_test ()
  {
    float sogi_output[2];
    g_flt_sogi_handler.filter(g_dac_ch1_20khz_handler.getOutputValue(),sogi_output);
    printf("samples:%f,%f\n",sogi_output[0],sogi_output[1]);
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
    //    HAL_TIM_Base_Start_IT(&htim1);

    static float voltage=0;

    while(1)
      {
	g_mk1031_sensor_handler.readRegisters(MK1031_VOLTAGE,3);
	HAL_Delay(10);
	g_modbus_message_handler.processHandler();
	voltage=g_mk1031_wrapper_handler.readVout();
      }
  }

  /*函数名: dc_dc_voltageClosedLoop_test
   * 测试dcdc降压是否正常工作
   * */
  void dc_dc_current_voltage_ClosedLoop_test()
  {
    //蓝牙调试串口抽象层初始化
    g_message_handler=stm32_message::getUART1();
    g_message_handler.attachEvent(vofaReceiveCallback,PINGPONG_BUFFER);
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
    //定时器初始化
    g_hrtimer_pwm_handler=stm32_hrtim_pwm::getTimerAOutput();

    //定时事件处理，处理串口空闲中断接收到的数据
    HAL_TIM_Base_Start_IT(&htim1);
    //TO DO
    g_dc_buck_sensor_handler=stm32_dc_dc::getDCBuckMK1031(&g_hrtimer_pwm_handler,&g_mk1031_wrapper_handler);
    g_current_pid.begin(1.35, 0.0098, 0, 0.0535);
    g_dc_buck_sensor_handler.setCC_PID(&g_current_pid);

    g_voltage_pid.begin(0.0529, 0.0140, 0, 0.0173);
    g_dc_buck_sensor_handler.setCV_PID(&g_voltage_pid);


    g_hrtimer_pwm_handler.setOutput();
    g_dc_buck_sensor_handler.enable();
    g_dc_buck_sensor_handler.setVout(0);
    while (1)
      {
	HAL_Delay(1);
	printf("%f\n",stm32_test::g_dc_buck_sensor_handler.dataWrapper_->readVout());
	g_dc_buck_sensor_handler.setVout(g_target_vofa_set.target_voltage);
	if (g_bool_isResetPID == RESET_PID)
	  {
	    //	    g_dc_buck_sensor_handler.cc_pid_->kp=g_current_pid_vofa_set.kp;
	    //	    g_dc_buck_sensor_handler.cc_pid_->ki=g_current_pid_vofa_set.ki;
	    //	    g_dc_buck_sensor_handler.cc_pid_->kd=g_current_pid_vofa_set.kd;
	    //	    g_dc_buck_sensor_handler.cc_pid_->integral_limit=g_current_pid_vofa_set.integral_limit;
	    g_dc_buck_sensor_handler.cv_pid_->kp=g_voltage_pid_vofa_set.kp;
	    g_dc_buck_sensor_handler.cv_pid_->ki=g_voltage_pid_vofa_set.ki;
	    g_dc_buck_sensor_handler.cv_pid_->kd=g_voltage_pid_vofa_set.kd;
	    g_dc_buck_sensor_handler.cv_pid_->integral_limit=g_voltage_pid_vofa_set.integral_limit;
	  }
      }
  }

  void dc_dc_currentClosedLoop_test()
  {
    //蓝牙调试串口抽象层初始化
    g_message_handler=stm32_message::getUART1();
    g_message_handler.attachEvent(vofaReceiveCallback,PINGPONG_BUFFER);
    g_message_handler.startReceive();
    //mk1031传感器初始化 //低压侧 电池
    g_modbus_message_handler=stm32_message::getUART2();
    g_mk1031_sensor_handler=stm32_mk1031::getMK1031(&g_modbus_message_handler, 1);
    //mk1031传感器初始化 //高压侧 电源
    g_modbus_message_handler_in=stm32_message::getUART3();
    g_mk1031_sensor_handler_in=stm32_mk1031::getMK1031(&g_modbus_message_handler_in, 1);
    //在对应的串口中断回调使用stm32_message的callbackhandler，并注册回调函数 低压侧
    g_modbus_message_handler.attachEvent([](uint8_t *data, uint16_t len)
					 {
      g_mk1031_sensor_handler.responseHandler(data,len);
					 },PINGPONG_BUFFER);
    g_modbus_message_handler.startReceive();

    //在对应的串口中断回调使用stm32_message的callbackhandler，并注册回调函数  高压侧
    g_modbus_message_handler_in.attachEvent([](uint8_t *data, uint16_t len)
					    {
      g_mk1031_sensor_handler_in.responseHandler(data,len);
					    },PINGPONG_BUFFER);
    g_modbus_message_handler_in.startReceive();

    g_mk1031_wrapper_handler.init(&g_mk1031_sensor_handler, &g_mk1031_sensor_handler_in);

    //定时器初始化
    g_hrtimer_pwm_handler=stm32_hrtim_pwm::getTimerAOutput();

    //定时事件处理，处理串口空闲中断接收到的数据
    HAL_TIM_Base_Start_IT(&htim1);
    //TO DO
    g_dc_buck_sensor_handler=stm32_dc_dc::getDCBuckMK1031(&g_hrtimer_pwm_handler,&g_mk1031_wrapper_handler);
    g_current_pid.begin(0.429,0.131,0,0.1109);
    g_dc_buck_sensor_handler.setCC_PID(&g_current_pid);



    g_dc_buck_sensor_handler.setCurrent(0);

    g_hrtimer_pwm_handler.setOutput();
    g_dc_buck_sensor_handler.enable();



    while (1)
      {
	//   	HAL_Delay(1);
	//   	printf("%f\n",stm32_test::g_dc_buck_sensor_handler.dataWrapper_->readCurrent());
	g_dc_buck_sensor_handler.setCurrent(g_target_vofa_set.target_current);

	if (g_bool_isResetPID == RESET_PID)
	  {
	    g_dc_buck_sensor_handler.cc_pid_->kp=g_current_pid_vofa_set.kp;
	    g_dc_buck_sensor_handler.cc_pid_->ki=g_current_pid_vofa_set.ki;
	    g_dc_buck_sensor_handler.cc_pid_->kd=g_current_pid_vofa_set.kd;
	    g_dc_buck_sensor_handler.cc_pid_->integral_limit=g_current_pid_vofa_set.integral_limit;
	    // 	   g_dc_buck_sensor_handler.cv_pid_->kp=g_voltage_pid_vofa_set.kp;
	    // 	   g_dc_buck_sensor_handler.cv_pid_->ki=g_voltage_pid_vofa_set.ki;
	    // 	   g_dc_buck_sensor_handler.cv_pid_->kd=g_voltage_pid_vofa_set.kd;
	    // 	   g_dc_buck_sensor_handler.cv_pid_->integral_limit=g_voltage_pid_vofa_set.integral_limit;
	  }
      }
  }


  void boost_currentClosedLoop_test()
  {
    //蓝牙调试串口抽象层初始化
    g_message_handler=stm32_message::getUART1();
    g_message_handler.attachEvent(vofaReceiveCallback,PINGPONG_BUFFER);
    g_message_handler.startReceive();
    //mk1031传感器初始化 //低压侧 电池
    g_modbus_message_handler=stm32_message::getUART2();
    g_mk1031_sensor_handler=stm32_mk1031::getMK1031(&g_modbus_message_handler, 1);
    //mk1031传感器初始化 //高压侧 电源
    g_modbus_message_handler_in=stm32_message::getUART3();
    g_mk1031_sensor_handler_in=stm32_mk1031::getMK1031(&g_modbus_message_handler_in, 1);
    //在对应的串口中断回调使用stm32_message的callbackhandler，并注册回调函数 低压侧
    g_modbus_message_handler.attachEvent([](uint8_t *data, uint16_t len)
					 {
      g_mk1031_sensor_handler.responseHandler(data,len);
					 },PINGPONG_BUFFER);
    g_modbus_message_handler.startReceive();

    //在对应的串口中断回调使用stm32_message的callbackhandler，并注册回调函数  高压侧
    g_modbus_message_handler_in.attachEvent([](uint8_t *data, uint16_t len)
					    {
      g_mk1031_sensor_handler_in.responseHandler(data,len);
					    },PINGPONG_BUFFER);
    g_modbus_message_handler_in.startReceive();

    g_mk1031_wrapper_handler.init(&g_mk1031_sensor_handler, &g_mk1031_sensor_handler_in);

    //定时器初始化
    g_hrtimer_pwm_handler=stm32_hrtim_pwm::getTimerAOutput();

    //定时事件处理，处理串口空闲中断接收到的数据
    HAL_TIM_Base_Start_IT(&htim1);
    //TO DO
    g_dc_boost_sensor_handler=stm32_dc_dc::getDCBoostMK1031(&g_hrtimer_pwm_handler,&g_mk1031_wrapper_handler);
    g_current_pid.begin(0,0,0,0);
    g_dc_boost_sensor_handler.setCC_PID(&g_current_pid);



    g_dc_boost_sensor_handler.setCurrent(0.3);

    g_hrtimer_pwm_handler.setOutput();
    g_dc_boost_sensor_handler.enable();



    while (1)
      {
	// 	HAL_Delay(1);
	//	 printf("Vin:%f\n", stm32_test::g_dc_boost_sensor_handler.dataWrapper_->readVin());
	// 	printf("%f\n",stm32_test::g_dc_boost_sensor_handler.dataWrapper_->readCurrent());
	g_dc_boost_sensor_handler.setCurrent(g_target_vofa_set.target_current);

	// 	g_dc_boost_sensor_handler.setCurrent(0.7);
	if (g_bool_isResetPID == RESET_PID)
	  {
	    g_dc_boost_sensor_handler.cc_pid_->kp=g_current_pid_vofa_set.kp;
	    g_dc_boost_sensor_handler.cc_pid_->ki=g_current_pid_vofa_set.ki;
	    g_dc_boost_sensor_handler.cc_pid_->kd=g_current_pid_vofa_set.kd;
	    g_dc_boost_sensor_handler.cc_pid_->integral_limit=g_current_pid_vofa_set.integral_limit;
	    // 	   g_dc_boost_sensor_handler.cv_pid_->kp=g_voltage_pid_vofa_set.kp;
	    // 	   g_dc_boost_sensor_handler.cv_pid_->ki=g_voltage_pid_vofa_set.ki;
	    // 	   g_dc_boost_sensor_handler.cv_pid_->kd=g_voltage_pid_vofa_set.kd;
	    // 	   g_dc_boost_sensor_handler.cv_pid_->integral_limit=g_voltage_pid_vofa_set.integral_limit;
	  }
      }
  }

  void boost_VoltageClosedLoop_test()
  {

    //蓝牙调试串口抽象层初始化
    g_message_handler=stm32_message::getUART1();
    g_message_handler.attachEvent(vofaReceiveCallback,PINGPONG_BUFFER);
    g_message_handler.startReceive();
    //mk1031传感器初始化 //低压侧 电池
    g_modbus_message_handler=stm32_message::getUART2();
    g_mk1031_sensor_handler=stm32_mk1031::getMK1031(&g_modbus_message_handler, 1);
    //mk1031传感器初始化 //高压侧 电源
    g_modbus_message_handler_in=stm32_message::getUART3();
    g_mk1031_sensor_handler_in=stm32_mk1031::getMK1031(&g_modbus_message_handler_in, 1);
    //在对应的串口中断回调使用stm32_message的callbackhandler，并注册回调函数 低压侧
    g_modbus_message_handler.attachEvent([](uint8_t *data, uint16_t len)
					 {
      g_mk1031_sensor_handler.responseHandler(data,len);
					 },PINGPONG_BUFFER);
    g_modbus_message_handler.startReceive();

    //在对应的串口中断回调使用stm32_message的callbackhandler，并注册回调函数  高压侧
    g_modbus_message_handler_in.attachEvent([](uint8_t *data, uint16_t len)
					    {
      g_mk1031_sensor_handler_in.responseHandler(data,len);
					    },PINGPONG_BUFFER);
    g_modbus_message_handler_in.startReceive();

    g_mk1031_wrapper_handler.init(&g_mk1031_sensor_handler, &g_mk1031_sensor_handler_in);

    //定时器初始化
    g_hrtimer_pwm_handler=stm32_hrtim_pwm::getTimerAOutput();

    //定时事件处理，处理串口空闲中断接收到的数据
    HAL_TIM_Base_Start_IT(&htim1);
    //TO DO
    g_dc_boost_sensor_handler=stm32_dc_dc::getDCBoostMK1031(&g_hrtimer_pwm_handler,&g_mk1031_wrapper_handler);
    //     g_current_pid.begin(0.0197,0.0228,0,0.1109);
    //     g_dc_boost_sensor_handler.setCC_PID(&g_current_pid);

    g_voltage_pid.begin(0.0139, 0.0027, 0, 0.0009);
    g_dc_boost_sensor_handler.setCV_PID(&g_voltage_pid);



    //    g_dc_boost_sensor_handler.setCurrent(0.3);
    g_dc_boost_sensor_handler.setVin(30);
    g_hrtimer_pwm_handler.setOutput();
    g_dc_boost_sensor_handler.enable();



    while (1)
      {
	// 	HAL_Delay(1);
	//	 printf("Vin:%f\n", stm32_test::g_dc_boost_sensor_handler.dataWrapper_->readVin());
	// 	printf("%f\n",stm32_test::g_dc_boost_sensor_handler.dataWrapper_->readCurrent());
	g_dc_boost_sensor_handler.setVin(g_target_vofa_set.target_voltage);

	// 	g_dc_boost_sensor_handler.setCurrent(0.7);
	if (g_bool_isResetPID == RESET_PID)
	  {
	    // 	   g_dc_boost_sensor_handler.cc_pid_->kp=g_current_pid_vofa_set.kp;
	    // 	   g_dc_boost_sensor_handler.cc_pid_->ki=g_current_pid_vofa_set.ki;
	    // 	   g_dc_boost_sensor_handler.cc_pid_->kd=g_current_pid_vofa_set.kd;
	    // 	   g_dc_boost_sensor_handler.cc_pid_->integral_limit=g_current_pid_vofa_set.integral_limit;
	    g_dc_boost_sensor_handler.cv_pid_->kp=g_voltage_pid_vofa_set.kp;
	    g_dc_boost_sensor_handler.cv_pid_->ki=g_voltage_pid_vofa_set.ki;
	    g_dc_boost_sensor_handler.cv_pid_->kd=g_voltage_pid_vofa_set.kd;
	    g_dc_boost_sensor_handler.cv_pid_->integral_limit=g_voltage_pid_vofa_set.integral_limit;
	  }
      }
  }

  void boost_CurrentVoltageClosedLoop_test()
  {
    //蓝牙调试串口抽象层初始化
    g_message_handler=stm32_message::getUART1();
    g_message_handler.attachEvent(vofaReceiveCallback,PINGPONG_BUFFER);
    g_message_handler.startReceive();
    //mk1031传感器初始化 //低压侧 电池
    g_modbus_message_handler=stm32_message::getUART2();
    g_mk1031_sensor_handler=stm32_mk1031::getMK1031(&g_modbus_message_handler, 1);
    //mk1031传感器初始化 //高压侧 电源
    g_modbus_message_handler_in=stm32_message::getUART3();
    g_mk1031_sensor_handler_in=stm32_mk1031::getMK1031(&g_modbus_message_handler_in, 1);
    //在对应的串口中断回调使用stm32_message的callbackhandler，并注册回调函数 低压侧
    g_modbus_message_handler.attachEvent([](uint8_t *data, uint16_t len)
					 {
      g_mk1031_sensor_handler.responseHandler(data,len);
					 },PINGPONG_BUFFER);
    g_modbus_message_handler.startReceive();

    //在对应的串口中断回调使用stm32_message的callbackhandler，并注册回调函数  高压侧
    g_modbus_message_handler_in.attachEvent([](uint8_t *data, uint16_t len)
					    {
      g_mk1031_sensor_handler_in.responseHandler(data,len);
					    },PINGPONG_BUFFER);
    g_modbus_message_handler_in.startReceive();

    g_mk1031_wrapper_handler.init(&g_mk1031_sensor_handler, &g_mk1031_sensor_handler_in);

    //定时器初始化
    g_hrtimer_pwm_handler=stm32_hrtim_pwm::getTimerAOutput();

    //定时事件处理，处理串口空闲中断接收到的数据
    HAL_TIM_Base_Start_IT(&htim1);
    //TO DO
    g_dc_boost_sensor_handler=stm32_dc_dc::getDCBoostMK1031(&g_hrtimer_pwm_handler,&g_mk1031_wrapper_handler);
    g_current_pid.begin(0.0429,0.0131,0,0.1109);
    g_dc_boost_sensor_handler.setCC_PID(&g_current_pid);

    g_voltage_pid.begin(0, 0, 0, 0);
    g_dc_boost_sensor_handler.setCV_PID(&g_voltage_pid);




    g_dc_boost_sensor_handler.setVin(25);
    g_hrtimer_pwm_handler.setOutput();
    g_dc_boost_sensor_handler.enable();



    while (1)
      {
	// 	HAL_Delay(1);
	//	 printf("Vin:%f\n", stm32_test::g_dc_boost_sensor_handler.dataWrapper_->readVin());
	// 	printf("%f\n",stm32_test::g_dc_boost_sensor_handler.dataWrapper_->readCurrent());
	g_dc_boost_sensor_handler.setVin(g_target_vofa_set.target_voltage);

	// 	g_dc_boost_sensor_handler.setCurrent(0.7);
	if (g_bool_isResetPID == RESET_PID)
	  {
	    // 	   g_dc_boost_sensor_handler.cc_pid_->kp=g_current_pid_vofa_set.kp;
	    // 	   g_dc_boost_sensor_handler.cc_pid_->ki=g_current_pid_vofa_set.ki;
	    // 	   g_dc_boost_sensor_handler.cc_pid_->kd=g_current_pid_vofa_set.kd;
	    // 	   g_dc_boost_sensor_handler.cc_pid_->integral_limit=g_current_pid_vofa_set.integral_limit;
	    g_dc_boost_sensor_handler.cv_pid_->kp=g_voltage_pid_vofa_set.kp;
	    g_dc_boost_sensor_handler.cv_pid_->ki=g_voltage_pid_vofa_set.ki;
	    g_dc_boost_sensor_handler.cv_pid_->kd=g_voltage_pid_vofa_set.kd;
	    g_dc_boost_sensor_handler.cv_pid_->integral_limit=g_voltage_pid_vofa_set.integral_limit;
	  }
      }
  }

  void boost_openLoop_test()
  {
    //蓝牙调试串口抽象层初始化
    g_message_handler=stm32_message::getUART1();
    g_message_handler.attachEvent(vofaReceiveCallback,PINGPONG_BUFFER);
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
    //定时器初始化
    g_hrtimer_pwm_handler=stm32_hrtim_pwm::getTimerAOutput();


    //定时事件处理，处理串口空闲中断接收到的数据
    HAL_TIM_Base_Start_IT(&htim1);
    //TO DO
    g_dc_boost_sensor_handler=stm32_dc_dc::getDCBoostMK1031(&g_hrtimer_pwm_handler,&g_mk1031_wrapper_handler);
    g_dc_boost_sensor_handler.setVin(10);
    g_dc_boost_sensor_handler.setVout(20);
    g_hrtimer_pwm_handler.setOutput();
    g_dc_boost_sensor_handler.enable();
    while (1)
      {
	//	HAL_Delay(1);
	//	printf("%f\n",stm32_test::g_dc_boost_sensor_handler.dataWrapper_->readCurrent());
	//	if(g_bool_isOutput == OUTPUT_START)
	//	  {
	//	    g_dc_buck_sensor_handler.enable();
	//	  }
	//	else
	//	  {
	//	    g_dc_buck_sensor_handler.disable();
	//	  }
	//	g_dc_buck_sensor_handler.setVout(g_target_vofa_set.target_voltage);
	g_dc_boost_sensor_handler.openVoltageLoopControl();
      }
  }




  /*函数名: dc_dc_openLoop_test
   * 测试dcdc降压开环是否正常工作
   * */
  void dc_dc_openLoop_test()
  {
    //蓝牙调试串口抽象层初始化
    g_message_handler=stm32_message::getUART1();
    g_message_handler.attachEvent(vofaReceiveCallback,PINGPONG_BUFFER);
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
    //定时器初始化
    g_hrtimer_pwm_handler=stm32_hrtim_pwm::getTimerAOutput();


    //定时事件处理，处理串口空闲中断接收到的数据
    HAL_TIM_Base_Start_IT(&htim1);
    //TO DO
    g_dc_buck_sensor_handler=stm32_dc_dc::getDCBuckMK1031(&g_hrtimer_pwm_handler,&g_mk1031_wrapper_handler);
    g_dc_buck_sensor_handler.setVin(10);
    g_dc_buck_sensor_handler.setVout(5);
    g_hrtimer_pwm_handler.setOutput();
    g_dc_buck_sensor_handler.enable();
    while (1)
      {
	HAL_Delay(1);
	printf("%f\n",stm32_test::g_dc_buck_sensor_handler.dataWrapper_->readCurrent());
	//	if(g_bool_isOutput == OUTPUT_START)
	//	  {
	//	    g_dc_buck_sensor_handler.enable();
	//	  }
	//	else
	//	  {
	//	    g_dc_buck_sensor_handler.disable();
	//	  }
	//	g_dc_buck_sensor_handler.setVout(g_target_vofa_set.target_voltage);
	g_dc_buck_sensor_handler.openVoltageLoopControl();
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

  void dc_ac_openLoop_test()
  {
    g_hrtimer_pwm_handler=stm32_hrtim_pwm::getTimerCOutput();

    g_dc_ac_sensor_handler=stm32_dc_ac::getDCACMK1031(&g_hrtimer_pwm_handler);

    g_dc_ac_sensor_handler.enable();
    g_hrtimer_pwm_handler.setOutput_IT();

    while(1)
      {

      }
    //修改占空比的部分在中断回调中实现
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
    //    printf("AT+ROLE=2\r\n");
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

  /* 校赛测试结束 */

  void keyboard_test()
  {
    g_keyboard_handler=stm32_keyboard::getKeyboard(1);
    g_keyboard_handler.attach(3, [](){
      printf("KEY3\n");
    });
    while(1)
      {
	g_keyboard_handler.processHandler();
      }
  }


  void oled_menu_test()
  {
    float temp=0;
    static uint8_t swim=0;
    switch (g_oled_menu_mode)
    {
      case OLED_MENU_BUCK:
	if (g_oled_menu_isSelect == OLED_MENU_SELECT)
	  {
	    OLED_Printf(10, 40-swim, 16, ">I_set=%.2f",g_target_vofa_set.target_current);
	  }
	else
	  {
	    OLED_Printf(10, 40-swim, 16, "I_set=%.2f",g_target_vofa_set.target_current );
	  }
	OLED_Printf(10, 57-swim, 16, "I_out=%.2f", g_mk1031_wrapper_handler.readCurrent());
	OLED_Printf(10, 74-swim, 16, "U_batt=%.2f", g_mk1031_wrapper_handler.readVout());
	OLED_Printf(10, 91-swim, 16, "W=%.2f", g_mk1031_wrapper_handler.readCurrent() * g_mk1031_wrapper_handler.readVout());
	OLED_Reflash();
	OLED_GRAM_CLR();
	swim=(swim+1)%128;
	break;
      case OLED_MENU_BOOST:
	OLED_Printf(10, 10, 16, "I_out=%.2f",g_mk1031_wrapper_handler.readCurrent_in() );
	OLED_Printf(10, 27, 16, "U2=%.2f", g_mk1031_wrapper_handler.readVin());
	OLED_Printf(10, 43, 16, "W=%.2f", g_mk1031_wrapper_handler.readCurrent_in() * g_mk1031_wrapper_handler.readVin());
	OLED_Reflash();
	break;
      default:
	break;
    }
  }

  void oled_test()
  {
    OLED_Init();//OLED初始化
    g_keyboard_handler=stm32_keyboard::getKeyboard(1);

    g_keyboard_handler.attach(13, [](){
      //13 - 切换模式
      OLED_GRAM_CLR();
      if(g_oled_menu_mode == OLED_MENU_BUCK)
	{
	  g_oled_menu_mode = OLED_MENU_BOOST;
	  g_relay_handler.on();
	}
      else if(g_oled_menu_mode == OLED_MENU_BOOST)
	{
	  g_oled_menu_mode = OLED_MENU_BUCK;
	  g_relay_handler.off();
	}
    });

    g_keyboard_handler.attach(14, [](){
      //14 - 在buck下选中设置电流
      if(g_oled_menu_mode == OLED_MENU_BUCK)
	{
	  g_oled_menu_isSelect = static_cast<oled_menu_isSelcet_bool_t>(((static_cast<int>(g_oled_menu_isSelect))  + 1)% OLED_MENU_SELCET_NUM);
	}
    });

    g_keyboard_handler.attach(15, [](){
      //15 - 选中后增加值
      if(g_oled_menu_mode == OLED_MENU_BUCK && g_oled_menu_isSelect == OLED_MENU_SELECT)
	{
	  if(g_target_vofa_set.target_current < 2)
	    {
	      g_target_vofa_set.target_current+=0.1;//增加电流
	      g_dc_buck_sensor_handler.setCurrent(g_target_vofa_set.target_current);//设置Buck充电电流
	    }
	}
    });

    g_keyboard_handler.attach(16, [](){
      //15 - 选中后减少值
      if(g_oled_menu_mode == OLED_MENU_BUCK && g_oled_menu_isSelect == OLED_MENU_SELECT)
	{
	  if(g_target_vofa_set.target_current > 0.0001)
	    {
	      g_target_vofa_set.target_current-=0.1; //减少电流
	      g_dc_buck_sensor_handler.setCurrent(g_target_vofa_set.target_current);//设置Buck充电电流
	    }
	}
    });
    /*交互初始化结束*/

    /*控制初始化开始*/

    /*硬件抽象层初始化开始*/
    g_relay_handler=stm32_relay::getRelay1();//获得继电器
    while(1)
      {
	oled_menu_test();
	g_keyboard_handler.processHandler();//TO DO ，移动定时器里去轮询

      }
  }

  void nuedc_2015_init_test()
  {
    /*交互初始化开始*/
    OLED_Init();//OLED初始化
    g_keyboard_handler=stm32_keyboard::getKeyboard(1);

    g_keyboard_handler.attach(13, [](){
      //13 - 切换模式
      OLED_GRAM_CLR();
      if(g_oled_menu_mode == OLED_MENU_BUCK)
	{
	  g_oled_menu_mode = OLED_MENU_BOOST;
	  g_relay_handler.on();
	}
      else if(g_oled_menu_mode == OLED_MENU_BOOST)
	{
	  g_oled_menu_mode = OLED_MENU_BUCK;
	  g_relay_handler.off();
	}
    });

    g_keyboard_handler.attach(14, [](){
      //14 - 在buck下选中设置电流
      if(g_oled_menu_mode == OLED_MENU_BUCK)
	{
	  g_oled_menu_isSelect = static_cast<oled_menu_isSelcet_bool_t>(((static_cast<int>(g_oled_menu_isSelect))  + 1)% OLED_MENU_SELCET_NUM);
	}
    });

    g_keyboard_handler.attach(15, [](){
      //15 - 选中后增加值
      if(g_oled_menu_mode == OLED_MENU_BUCK && g_oled_menu_isSelect == OLED_MENU_SELECT)
	{
	  if(g_target_vofa_set.target_current < 2)
	    {
	      g_target_vofa_set.target_current+=0.1;//增加电流
	      g_dc_buck_sensor_handler.setCurrent(g_target_vofa_set.target_current);//设置Buck充电电流
	    }
	}
    });

    g_keyboard_handler.attach(16, [](){
      //15 - 选中后减少值
      if(g_oled_menu_mode == OLED_MENU_BUCK && g_oled_menu_isSelect == OLED_MENU_SELECT)
	{
	  if(g_target_vofa_set.target_current > 0.0001)
	    {
	      g_target_vofa_set.target_current-=0.1; //减少电流
	      g_dc_buck_sensor_handler.setCurrent(g_target_vofa_set.target_current);//设置Buck充电电流
	    }
	}
    });
    /*交互初始化结束*/

    /*控制初始化开始*/

    /*硬件抽象层初始化开始*/
    g_relay_handler=stm32_relay::getRelay1();//获得继电器
    //蓝牙调试串口抽象层初始化
    g_message_handler=stm32_message::getUART1();
    g_message_handler.attachEvent(vofaReceiveCallback,PINGPONG_BUFFER);
    g_message_handler.startReceive();
    //mk1031传感器初始化 //低压侧 电池
    g_modbus_message_handler=stm32_message::getUART2();
    g_mk1031_sensor_handler=stm32_mk1031::getMK1031(&g_modbus_message_handler, 1);
    //mk1031传感器初始化 //高压侧 电源
    g_modbus_message_handler_in=stm32_message::getUART3();
    g_mk1031_sensor_handler_in=stm32_mk1031::getMK1031(&g_modbus_message_handler_in, 1);
    //在对应的串口中断回调使用stm32_message的callbackhandler，并注册回调函数 高压侧
    g_modbus_message_handler.attachEvent([](uint8_t *data, uint16_t len)
					 {
      g_mk1031_sensor_handler.responseHandler(data,len);
					 },PINGPONG_BUFFER);
    g_modbus_message_handler.startReceive();

    //在对应的串口中断回调使用stm32_message的callbackhandler，并注册回调函数  低压侧
    g_modbus_message_handler_in.attachEvent([](uint8_t *data, uint16_t len)
					    {
      g_mk1031_sensor_handler_in.responseHandler(data,len);
					    },PINGPONG_BUFFER);
    g_modbus_message_handler_in.startReceive();

    g_mk1031_wrapper_handler.init(&g_mk1031_sensor_handler, &g_mk1031_sensor_handler_in);

    //定时器初始化
    g_hrtimer_pwm_handler=stm32_hrtim_pwm::getTimerAOutput();

    //定时事件处理，处理串口空闲中断接收到的数据
    HAL_TIM_Base_Start_IT(&htim1);
    /*硬件抽象层初始化结束*/


    //TO DO  修改PID参数
    g_dc_buck_sensor_handler=stm32_dc_dc::getDCBuckMK1031(&g_hrtimer_pwm_handler,&g_mk1031_wrapper_handler);
    g_current_pid.begin(0.0429,0.0131,0,0.1109);
    g_dc_buck_sensor_handler.setCC_PID(&g_current_pid);

    g_dc_boost_sensor_handler=stm32_dc_dc::getDCBoostMK1031(&g_hrtimer_pwm_handler,&g_mk1031_wrapper_handler);
    g_voltage_pid.begin(0.0139, 0.0027, 0, 0.0013);
    g_dc_boost_sensor_handler.setCV_PID(&g_voltage_pid);

    //设置目标值
    //    g_dc_buck_sensor_handler.setCurrent(1);//设置Buck充电电流
    g_dc_boost_sensor_handler.setVin(30);//设置Boost放电输出电压

    //设置输出
    g_hrtimer_pwm_handler.setOutput();
    g_dc_buck_sensor_handler.enable();
    g_dc_boost_sensor_handler.enable();
    /*控制初始化结束*/
  }

  void nuedc_2015_tim1Callback_test()
  {
    //定时器1的频率是2000hz，周期为0.5ms

    /* 采样逻辑开始 */
    constexpr uint16_t clock_div=100;//mk1031的采样周期要大于这里clock_div对应的25
    static uint16_t count=0;
    float u,i,w;
    if(count == 0)
      {
	//发送modbus采样
	stm32_test::g_mk1031_sensor_handler.readRegisters(MK1031_VOLTAGE,3);
	stm32_test::g_mk1031_sensor_handler_in.readRegisters(MK1031_VOLTAGE,3);


	/* 控制逻辑开始 */
	//根据菜单选择控制模式
	switch(g_oled_menu_mode)
	{
	  case OLED_MENU_BUCK:
	    //单电流环
	    //TO DO 控制逻辑
	    u=g_mk1031_wrapper_handler.readVout();
	    i=g_mk1031_wrapper_handler.readCurrent();
	    w=u*i;
	    //	    printf("%.2f,%.2f,%.2f\n",u,i,w);
	    g_dc_buck_sensor_handler.closedCurrentLoopControl();
	    break;

	  case OLED_MENU_BOOST:
	    u=g_mk1031_wrapper_handler.readVin();
	    i=g_mk1031_wrapper_handler.readCurrent();
	    w=u*i;
	    //	    printf("%.2f,%.2f,%.2f\n",u,i,w);
	    g_dc_boost_sensor_handler.closedVoltageLoopControl();
	    break;
	}
	/* 控制逻辑结束 */

      }
    count=(count+1)%clock_div;
    /* 采样逻辑结束 */

    /* 处理串口逻辑开始 */
    stm32_test::g_message_handler.processHandler();//蓝牙调试
    stm32_test:: g_modbus_message_handler.processHandler();//处理modbus接收数据 低压
    stm32_test:: g_modbus_message_handler_in.processHandler();//处理modbus接收数据 高压
    /* 处理串口逻辑结束 */
  }

  void nuedc_2015_loop_test()
  {
    nuedc_2015_init_test();
    float u=0,i=0,w=0;
    while(1)
      {
	oled_menu_test();
	g_dc_buck_sensor_handler.setCurrent(g_target_vofa_set.target_current);
	g_keyboard_handler.processHandler();//TO DO ，移动定时器里去轮询
	//控制逻辑放在定时器里
      }
  }

  /* 校赛测试结束 */



}

#endif /* INC_STM32_TEST_H_ */
