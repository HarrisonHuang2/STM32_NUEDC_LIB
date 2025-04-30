/*
 * stm32_dc_buck.h
 *
 *  Created on: Mar 23, 2025
 *      Author: GAOTIANHAO
 */

#ifndef INC_STM32_DC_DC_H_
#define INC_STM32_DC_DC_H_

#include "alg_dc_buck.h"
#include "alg_dc_boost.h"
#include "alg_dc_buck_boost.h"
#include "stm32_hrtim_pwm.h"
#include "alg_dc_dc.h"
#include "stm32_adc.h"
#include "stm32_relay.h"


namespace stm32_dc_dc
{
  Algorithim_DC_Buck<Hardware_STM32_HRTIM_PWM, Hardware_STM32_ADC>
  getDCBuck1(Hardware_STM32_HRTIM_PWM *pwm, Hardware_STM32_ADC *adc)
  {
    Algorithim_DC_Buck<Hardware_STM32_HRTIM_PWM, Hardware_STM32_ADC>dc_buck;
    dc_buck.begin(pwm, adc);
    return dc_buck;
  }

  Algorithim_DC_Boost<Hardware_STM32_HRTIM_PWM, Hardware_STM32_ADC>
  getDCBoost1(Hardware_STM32_HRTIM_PWM *pwm, Hardware_STM32_ADC *adc)
  {
    Algorithim_DC_Boost<Hardware_STM32_HRTIM_PWM, Hardware_STM32_ADC>dc_boost;
    dc_boost.begin(pwm, adc);
    return dc_boost;
  }

  Algorithim_DC_Buck_Boost<Hardware_STM32_HRTIM_PWM, Hardware_STM32_ADC>
  getDCBuckBoost1(Hardware_STM32_HRTIM_PWM *pwm, Hardware_STM32_ADC *adc)
  {
   Algorithim_DC_Buck_Boost<Hardware_STM32_HRTIM_PWM, Hardware_STM32_ADC>dc_buck_boost;
   dc_buck_boost.begin(pwm, adc);
   return dc_buck_boost;
  }

}
#endif /* INC_STM32_DC_DC_H_ */
