/*
 * stm32_callback.cpp
 *
 *  Created on: Mar 22, 2025
 *      Author: GAOTIANHAO
 */

#include"hw_port_us_timer.h"
#include"hw_port_hrtim_pwm.h"
#include "alg_dc_buck.h"
#include "alg_dc_boost.h"
#include "hw_port_mk1031_wrapper.h"
#include"hw_port_adc.h"
#include"hw_port_message.h"
#include"hw_port_mk1031.h"
#include "tim.h"
#include "SEGGER_SYSVIEW_Conf.h"
#include "SEGGER_SYSVIEW.h"
#include "stdio.h"

namespace stm32_test
{
  extern Hardware_STM32_Message g_message_handler;
  extern Hardware_STM32_ADC g_adc_handler;
  extern Hardware_STM32_Message g_message_handler;
  extern Hardware_STM32_US_Timer g_us_timer_handler;
  extern Hardware_MK1031 g_mk1031_sensor_handler;
  extern Hardware_STM32_Message g_modbus_message_handler;
  extern Algorithim_DC_Buck<Hardware_STM32_HRTIM_PWM, Hardware_MK1031_Wrapper>g_dc_buck_sensor_handler;
  extern Algorithim_DC_Boost<Hardware_STM32_HRTIM_PWM, Hardware_MK1031_Wrapper> g_dc_boost_sensor_handler;
  void pll_it_test();
  void filiter_hilbert_it_singlePoint_test();
  void filiter_hilbert_multyPoints_test();
}

namespace nuedc_2015
{
  extern Hardware_STM32_HRTIM_PWM g_hrtimer_pwm_handler;
  extern Hardware_STM32_ADC g_adc1_handler;
  extern Hardware_STM32_ADC g_adc3_handler;
  extern Hardware_STM32_Message g_message_handler;
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  stm32_test::g_adc_handler.dmaCallbackHandler(hadc);
  nuedc_2015::g_adc1_handler.dmaCallbackHandler(hadc);
  nuedc_2015::g_adc3_handler.dmaCallbackHandler(hadc);
}

void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  stm32_test::g_adc_handler.iTCallbackHandler(hadc);
  nuedc_2015::g_adc1_handler.iTCallbackHandler(hadc);
  nuedc_2015::g_adc3_handler.dmaCallbackHandler(hadc);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  stm32_test::g_message_handler.callbackHandler(huart, Size);
  stm32_test:: g_modbus_message_handler.callbackHandler(huart, Size);
  //  nuedc_2015::g_message_handler.callbackHandler(huart, Size);
}

void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *htim)
{
  if(htim == &htim1)
    {
      //定时器1的频率是2000hz，周期为0.5ms
      constexpr uint16_t clock_div=50;//mk1031的采样周期要大于这里clock_div对应的25
      static uint16_t count=0;
      if(count == 0)
	{
	  //发送modbus采样
	  stm32_test::g_mk1031_sensor_handler.readRegisters(MK1031_VOLTAGE,3);
	  //电流环测试
//	  stm32_test::g_dc_buck_sensor_handler.closedVoltageCurrentLoopControl();

	  //电流环测试
	  stm32_test::g_dc_boost_sensor_handler.closedCurrentLoopControl();

	}
      count=(count+1)%clock_div;
      stm32_test::g_message_handler.processHandler();//蓝牙调试
      stm32_test:: g_modbus_message_handler.processHandler();//处理modbus接收数据

      //      nuedc_2015::g_message_handler.processHandler();
    }
  else if(htim == &htim7)
    {
      stm32_test::pll_it_test();
      //      stm32_test::filiter_hilbert_it_singlePoint_test();
    }
  //  stm32_test::g_us_timer_handler.callbackHandler(htim);
}
