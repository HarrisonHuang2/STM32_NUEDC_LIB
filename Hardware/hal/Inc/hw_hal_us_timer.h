/*
 * hw_hal_us_timer.h
 *
 *  Created on: Apr 21, 2025
 *      Author: GAOTIANHAO
 */

#ifndef HAL_INC_HW_HAL_US_TIMER_H_
#define HAL_INC_HW_HAL_US_TIMER_H_

#include "main.h"
#include "portmacro.h"

#ifdef __cpp_concepts
template <typename T>
concept UsTimerInterfaceConcept = requires(T t , uint32_t time) {
  { t.on() } -> std::same_as<void>;
  { t.off() } -> std::same_as<void>;
  { t.getState() } -> std::convertible_to<bool>;
  { t.getUs() } -> std::convertible_to<uint32_t>;
  { t.dalay_us(time) } -> std::convertible_to<void>;
};
#endif




#endif /* HAL_INC_HW_HAL_US_TIMER_H_ */
