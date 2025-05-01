/*
 * hw_port_adc_wrapper.h
 *
 *  Created on: May 1, 2025
 *      Author: GAOTIANHAO
 */

#ifndef PORT_INC_HW_PORT_ADC_WRAPPER_H_
#define PORT_INC_HW_PORT_ADC_WRAPPER_H_

#include "hw_port_adc.h"
#include "hw_hal_data_wrapper.h"

enum hw_stm32_adc_wrapper_channel_id_t
{
  STM32_ADC_WRAPPER_CHANNEL_ID1,
  STM32_ADC_WRAPPER_CHANNEL_ID2,
  STM32_ADC_WRAPPER_CHANNEL_ID3
};

enum hw_stm32_adc_wrapper_type_t
{
  STM32_ADC_WRAPPER_VIN,
  STM32_ADC_WRAPPER_VOUT,
  STM32_ADC_WRAPPER_CURRENT,
  STM32_ADC_WRAPPER_EMPTY
};


class Hardware_STM32_ADC_Wrapper
{
public:
  Hardware_STM32_ADC_Wrapper(){};
  Hardware_STM32_ADC *adc_;
  void init(Hardware_STM32_ADC *adc)
  {
    adc_=adc;
    for(int i=0;i<3;i++)
      {
	map[i]=STM32_ADC_WRAPPER_EMPTY;
      }
  }

  void create_mapping(hw_stm32_adc_wrapper_channel_id_t channel_id,hw_stm32_adc_wrapper_type_t wrapper_type)
  {
    if(!adc_){return ;}
    //将枚举量映射到ADC通道
    map[wrapper_type]=channel_id;
  }
  float readVin()
  {
    if(!adc_){return 0;}
    return (map[STM32_ADC_WRAPPER_VIN]==STM32_ADC_WRAPPER_EMPTY)?0:(adc_->readData_[map[STM32_ADC_WRAPPER_VIN]] / 4096.0f * 3.3f);
  }
  float readVout()
  {
    if(!adc_){return 0;}
    return (map[STM32_ADC_WRAPPER_VOUT]==STM32_ADC_WRAPPER_EMPTY)?0:(adc_->readData_[map[STM32_ADC_WRAPPER_VOUT]] / 4096.0f * 3.3f);
  }
  float readCurrent()
  {
    if(!adc_){return 0;}
    return (map[STM32_ADC_WRAPPER_CURRENT]==STM32_ADC_WRAPPER_EMPTY)?0:(adc_->readData_[map[STM32_ADC_WRAPPER_CURRENT]] / 4096.0f * 3.3f);
  }
private:
  uint32_t map[3];

};

#endif /* PORT_INC_HW_PORT_ADC_WRAPPER_H_ */
