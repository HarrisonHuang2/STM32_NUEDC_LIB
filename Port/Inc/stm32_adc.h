/*
 * stm32_adc.h
 *
 *  Created on: Mar 22, 2025
 *      Author: GAOTIANHAO
 */

#ifndef INC_STM32_ADC_H_
#define INC_STM32_ADC_H_

#include "hw_port_adc.h"
#include "adc.h"

namespace stm32_adc
{
  Hardware_STM32_ADC getADC1()
  {
    Hardware_STM32_ADC adc;
    adc.begin(&hadc1, STM32_ADC_SINGLE_ENDED);
    __HAL_DMA_DISABLE_IT(&hdma_adc1, DMA_IT_HT);
    return adc;
  }
  Hardware_STM32_ADC getADC2()
  {
    Hardware_STM32_ADC adc;
    adc.begin(&hadc2, STM32_ADC_SINGLE_ENDED);
    __HAL_DMA_DISABLE_IT(&hdma_adc2, DMA_IT_HT);
    return adc;
  }
  Hardware_STM32_ADC getADC3()
  {
    Hardware_STM32_ADC adc;
    adc.begin(&hadc3, STM32_ADC_SINGLE_ENDED,2);
    __HAL_DMA_DISABLE_IT(&hdma_adc3, DMA_IT_HT);
    return adc;
  }
}


#endif /* INC_STM32_ADC_H_ */
