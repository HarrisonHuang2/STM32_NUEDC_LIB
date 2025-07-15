/*
 * stm32_dac.h
 *
 *  Created on: Apr 20, 2025
 *      Author: GAOTIANHAO
 */

#ifndef INC_STM32_DAC_H_
#define INC_STM32_DAC_H_

#include "hw_port_dac.h"
#include "dac.h"

namespace stm32_dac
{
  Hardware_STM32_DAC<200> getDAC1_CH1()
  {
    Hardware_STM32_DAC<200> dac;
    dac.begin(&hdac1,&htim6,STM32_DAC_CHANNEL1,50,200,STM32_DAC_DDS_MODE);
    return dac;
  }

  Hardware_STM32_DAC<400> getDAC1_CH1_20khz()
  {
    //20khz定时器频率，生成50hz正弦信号，有400个点
    Hardware_STM32_DAC<400> dac;
    dac.begin(&hdac1,&htim6,STM32_DAC_CHANNEL1,50,20,STM32_DAC_DDS_MODE);
    return dac;
  }

  Hardware_STM32_DAC<200> getDAC1_CH2()
  {
    Hardware_STM32_DAC<200> dac;
    dac.begin(&hdac1,&htim7,STM32_DAC_CHANNEL2,50,200,STM32_DAC_DDS_MODE,PI/2);
    return dac;
  }

  Hardware_STM32_DAC<400> getDAC1_CH2_20khz()
  {
    //20khz定时器频率，生成50hz正弦信号，有400个点
    Hardware_STM32_DAC<400> dac;
    dac.begin(&hdac1,&htim7,STM32_DAC_CHANNEL2,50,20,STM32_DAC_DDS_MODE);
    return dac;
  }
}



#endif /* INC_STM32_DAC_H_ */
