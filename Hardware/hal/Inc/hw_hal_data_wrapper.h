/*
 * hw_hal_adc_wrapper.h
 *
 *  Created on: May 1, 2025
 *      Author: GAOTIANHAO
 */

#ifndef HAL_INC_HW_HAL_DATA_WRAPPER_H_
#define HAL_INC_HW_HAL_DATA_WRAPPER_H_

#include "main.h"
#include "portmacro.h"

#ifdef __cpp_concepts
template <typename T>
concept DataWrapperInterfaceConcept = requires(T t)
{
  {t.readVin()} -> std::convertible_to<float>;
  {t.readVout()} -> std::convertible_to<float>;
  {t.readCurrent()} -> std::convertible_to<float>;
};
#endif



#endif /* HAL_INC_HW_HAL_DATA_WRAPPER_H_ */
