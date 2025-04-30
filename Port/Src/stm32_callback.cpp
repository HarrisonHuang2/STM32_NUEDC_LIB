/*
 * stm32_callback.cpp
 *
 *  Created on: Mar 22, 2025
 *      Author: GAOTIANHAO
 */

#include"hw_port_us_timer.h"
#include"hw_port_hrtim_pwm.h"
#include"hw_port_adc.h"
#include"hw_port_message.h"
#include "tim.h"
#include "SEGGER_SYSVIEW_Conf.h"
#include "SEGGER_SYSVIEW.h"

namespace stm32_test
{
  extern Hardware_STM32_Message g_message_handler;
  extern Hardware_STM32_ADC g_adc_handler;
  extern Hardware_STM32_Message g_message_handler;
  extern Hardware_STM32_US_Timer g_us_timer_handler;
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
//  nuedc_2015::g_message_handler.callbackHandler(huart, Size);
}

void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *htim)
{
  if(htim == &htim1)
    {
      stm32_test::g_message_handler.processHandler();
//      nuedc_2015::g_message_handler.processHandler();
    }
  else if(htim == &htim7)
    {
      stm32_test::pll_it_test();
//      stm32_test::filiter_hilbert_it_singlePoint_test();
    }
//  stm32_test::g_us_timer_handler.callbackHandler(htim);
}
