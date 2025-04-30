/*
 * hw_hal_dac.h
 *
 *  Created on: Apr 20, 2025
 *      Author: GAOTIANHAO
 */

#ifndef HAL_INC_HW_HAL_DAC_H_
#define HAL_INC_HW_HAL_DAC_H_

#include "main.h"
#include "portmacro.h"

#ifdef __cpp_concepts
template <typename T>
concept DACInterfaceConcept = requires(T t,float frequency, uint16_t points)
{
  {t.update(frequency,points)} -> std::convertible_to<void>;
  {t.enable()} -> std::convertible_to<void>;
  {t.disable()} -> std::convertible_to<void>;
  {t.getFrequency()} -> std::convertible_to<float>;
  {t.getFrequency()} -> std::convertible_to<uint16_t>;
  {t.isEnabled()} -> std::convertible_to<bool>;
};
#endif


#endif /* HAL_INC_HW_HAL_DAC_H_ */
