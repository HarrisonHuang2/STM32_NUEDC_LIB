/*
 * hw_hal_adc.h
 *
 *  Created on: Mar 17, 2025
 *      Author: GAOTIANHAO
 */

#ifndef HAL_INC_HW_HAL_ADC_H_
#define HAL_INC_HW_HAL_ADC_H_

#include "main.h"
#include "portmacro.h"


#ifdef __cpp_concepts
template <typename T>
concept ADCInterfaceConcept = requires(T t, float* data, size_t length) {
  { t.read3Channel(data, length) } -> std::convertible_to<int8_t>;
  { t.startSample() } -> std::convertible_to<void>;
};
#endif


#endif /* HAL_INC_HW_HAL_ADC_H_ */
