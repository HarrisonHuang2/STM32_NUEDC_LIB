/*
 * stm32_dc_ac.h
 *
 *  Created on: Jun 8, 2025
 *      Author: GAOTIANHAO
 */

#ifndef INC_STM32_DC_AC_H_
#define INC_STM32_DC_AC_H_


#include "stm32_hrtim_pwm.h"
#include "hw_port_adc_wrapper.h"
#include "hw_port_mk1031_wrapper.h"
#include "alg_dc_ac.h"

namespace stm32_dc_ac
{
  Algorithim_DC_AC<Hardware_STM32_HRTIM_PWM, Hardware_MK1031_Wrapper>
  getDCACMK1031 (Hardware_STM32_HRTIM_PWM *pwm,
		 Hardware_MK1031_Wrapper *sensor_wrapper = nullptr)
  {
    Algorithim_DC_AC<Hardware_STM32_HRTIM_PWM, Hardware_MK1031_Wrapper> dc_ac;
   dc_ac.begin(pwm, 25000 , 50);//载波（PWM）频率为25khz，输出信号(正弦)频率为50Hz，调制比默认为0.9
    return dc_ac;
  }
}


#endif /* INC_STM32_DC_AC_H_ */
