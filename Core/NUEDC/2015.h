/*
 * 2015.h
 *
 *  Created on: Apr 16, 2025
 *      Author: GAOTIANHAO
 */

#ifndef NUEDC_2015_H_
#define NUEDC_2015_H_

#include <stm32_dc_dc.h>
#include "stm32_hrtim_pwm.h"
#include "stm32_adc.h"
#include "hw_port_adc_wrapper.h"
#include "stm32_relay.h"
#include "stm32_test.h"
#include "stm32_message.h"
#include "alg_dc_buck_boost.h"

namespace nuedc_2015
{

  Hardware_STM32_HRTIM_PWM g_hrtimer_pwm_handler;
  Hardware_STM32_ADC_Wrapper g_adc_wrapper;
  Hardware_STM32_ADC g_adc1_handler;
  Hardware_STM32_ADC g_adc3_handler;
  Hardware_STM32_Relay g_relay_handler;
  Hardware_STM32_Message g_message_handler;
  Algorithim_DC_Buck_Boost<Hardware_STM32_HRTIM_PWM, Hardware_STM32_ADC_Wrapper>g_dc_controler_handler;
  Algorithim_PID g_voltage_pid;
  Algorithim_PID g_current_pid;

  enum power_control_mode_t
  {
    CURRENT_SINGLE_CLOSE_LOOP,
    VOLTAGE_DOUBLE_LOOP,
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
  };


  target_vofa_set_value_t g_target_vofa_set={0};
  vofa_isResetPID_bool_t g_bool_isResetPID = NO_RESET_PID;
  vofa_isOutput_bool_t g_bool_isOutput = OUTPUT_STOP;
  power_control_mode_t g_power_control_mode = CURRENT_SINGLE_CLOSE_LOOP;
  Algorithim_PID g_voltage_pid_vofa_set;
  Algorithim_PID g_current_pid_vofa_set;


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
    else if (sscanf(s, "MODE=%f", &temp) >0)
      {
	if (temp == 0)
	  {
	    printf("BUCK CURRENT MODE\n");
	    g_power_control_mode = CURRENT_SINGLE_CLOSE_LOOP;
	  }
	else if (temp == 1)
	  {
	    printf("BOOST VOLTAGE MODE\n");
	    g_power_control_mode = VOLTAGE_DOUBLE_LOOP;
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
	printf("Vin:%f\n",g_dc_controler_handler.dataWrapper_->readVin());
	printf("Vout:%f\n",g_dc_controler_handler.dataWrapper_->readVout());
	printf("Current:%f\n",g_dc_controler_handler.dataWrapper_->readCurrent());
	printf("%d\n",g_dc_controler_handler.isEnable());
      }
  }

  void hardware_init()
  {
    //串口抽象层初始化
    g_message_handler=stm32_message::getUART1();
    g_message_handler.startReceive();

    //高级定时器抽象层初始化
    g_hrtimer_pwm_handler=stm32_hrtim_pwm::getTimerAOutput();
    g_hrtimer_pwm_handler.setOutput();

    //ADC抽象层初始化
    g_adc1_handler=stm32_adc::getADC1();
    g_adc3_handler=stm32_adc::getADC3();

    //继电器抽象层初始化
    g_relay_handler=stm32_relay::getRelay1();

    //算法抽象层初始化
    g_adc_wrapper.init(&g_adc1_handler);
    g_adc_wrapper.create_mapping(STM32_ADC_WRAPPER_CHANNEL_ID1, STM32_ADC_WRAPPER_VIN);
    g_adc_wrapper.create_mapping(STM32_ADC_WRAPPER_CHANNEL_ID2, STM32_ADC_WRAPPER_VOUT);
    g_adc_wrapper.create_mapping(STM32_ADC_WRAPPER_CHANNEL_ID3, STM32_ADC_WRAPPER_CURRENT);
    g_dc_controler_handler=stm32_dc_dc::getDCBuckBoostADC(&g_hrtimer_pwm_handler,&g_adc_wrapper);
    g_voltage_pid.begin(0.295160, 20.9333, 0);
    g_current_pid.begin(0.0456, 41.4562, 0);
    g_dc_controler_handler.setCV_PID(&g_voltage_pid);
    g_dc_controler_handler.setCC_PID(&g_current_pid);

    g_message_handler.attachEvent(vofaReceiveCallback,PINGPONG_BUFFER);
    g_adc1_handler.startSample();
    g_adc3_handler.startSample();
  }

  void loop()
  {
    hardware_init();
    float battery_data[2]={0};//0:battery_voltage 1:battery_current
    float battery_voltage=0;
    while (1)
      {
	if(g_bool_isOutput == OUTPUT_START)
	  {
	    g_dc_controler_handler.enable();
	  }
	else
	  {
	    g_dc_controler_handler.disable();
	  }

	switch(g_power_control_mode)
	{
	  case CURRENT_SINGLE_CLOSE_LOOP:
	    if (g_bool_isResetPID == RESET_PID)
	      {
		g_dc_controler_handler.cv_pid_->kp=g_voltage_pid_vofa_set.kp;
		g_dc_controler_handler.cv_pid_->ki=g_voltage_pid_vofa_set.ki;
		g_dc_controler_handler.cv_pid_->kd=g_voltage_pid_vofa_set.kd;
		g_dc_controler_handler.cv_pid_->integral_limit=g_voltage_pid_vofa_set.integral_limit;
	      }

	   g_adc3_handler.read2Channel(battery_data,2);
	   battery_voltage=battery_data[0];
	    if ( battery_voltage > 24)
	      {
		//电池过压保护
		g_relay_handler.off();
		g_dc_controler_handler.disable();
	      }
	    else
	      {
		g_dc_controler_handler.setCurrent(g_target_vofa_set.target_current);
		g_dc_controler_handler.closedBuckCurrentLoopControl ();
	      }
	    break;
	  case VOLTAGE_DOUBLE_LOOP:
	    if (g_bool_isResetPID == RESET_PID)
	      {
		g_dc_controler_handler.cc_pid_->kp=g_current_pid_vofa_set.kp;
		g_dc_controler_handler.cc_pid_->ki=g_current_pid_vofa_set.ki;
		g_dc_controler_handler.cc_pid_->kd=g_current_pid_vofa_set.kd;
		g_dc_controler_handler.cc_pid_->integral_limit=g_current_pid_vofa_set.integral_limit;
	      }
	    //若过压保护后切换模式，需要重新使能继电器，并重新使能控制器
	    g_relay_handler.on();
	    g_dc_controler_handler.closedBoostVoltageLoopControl ();
	    break;
	  default:
	    break;
	}
      }
  }
}


#endif /* NUEDC_2015_H_ */
