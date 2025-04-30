/*
 * stm32_us_timer.h
 *
 *  Created on: Apr 21, 2025
 *      Author: GAOTIANHAO
 */

#ifndef INC_STM32_US_TIMER_H_
#define INC_STM32_US_TIMER_H_

#include "hw_port_us_timer.h"
#include "tim.h"

namespace stm32_us_timer
{
  Hardware_STM32_US_Timer getUsTimer()
  {
    Hardware_STM32_US_Timer us_timer;
    us_timer.begin(&htim8);
    return us_timer;
  }
}


#endif /* INC_STM32_US_TIMER_H_ */
