/*
 * hw_port_adc.h
 *
 *  Created on: Mar 18, 2025
 *      Author: GAOTIANHAO
 */

#ifndef PORT_INC_HW_PORT_ADC_H_
#define PORT_INC_HW_PORT_ADC_H_

#include "hw_hal_adc.h"
#include "adc.h"
#include <utility>
#include <vector>

enum hw_stm32_adc_dma_ready_t{
  STM32_ADC_DMA_NOT_READY=0,
  STM32_ADC_DMA_READY=1,
};

enum hw_stm32_adc_it_ready_t{
  STM32_ADC_IT_NOT_READY=0,
  STM32_ADC_IT_READY=1,
};

enum hw_stm32_adc_mode_t{
  STM32_ADC_SINGLE_ENDED=ADC_SINGLE_ENDED,
  STM32_ADC_DIFFERENTIAL=ADC_DIFFERENTIAL_ENDED
};


class Hardware_STM32_ADC
{
public:
  Hardware_STM32_ADC(){}

  // 移动构造函数
  Hardware_STM32_ADC(Hardware_STM32_ADC&& other)noexcept = default;

  Hardware_STM32_ADC& operator=(Hardware_STM32_ADC&& other) noexcept
  {
    // 自我赋值检查
    if (this == &other)
      {
	return *this;
      }

    // 使用 std::swap 交换资源
    std::swap(hadc_, other.hadc_);
    std::swap(isDataReady_, other.isDataReady_);
    std::swap(isDataReady_IT_, other.isDataReady_IT_);
    std::swap(readData_, other.readData_);

    return *this;
  }
  void begin(ADC_HandleTypeDef *hadc,hw_stm32_adc_mode_t mode,size_t length=3) {
    if (hadc == nullptr) {
	return;
    }
    hadc_ = hadc;
    readData_=std::vector<uint32_t>(length,0);
    HAL_ADCEx_Calibration_Start(hadc_,mode);
  }

  void startSample()
  {
    HAL_ADC_Start_DMA(hadc_, readData_.data(), readData_.size());
  }

  void stopSample() {
    HAL_ADC_Stop_DMA(hadc_);
  }

  void begin_IT(ADC_HandleTypeDef *hadc,hw_stm32_adc_mode_t mode) {
    if (hadc == nullptr) {
	return;
    }
    hadc_ = hadc;
    HAL_ADCEx_Calibration_Start(hadc_,mode);
  }

  void startSample_IT()
  {
    //需要先开启对应的定时器才会触发中断
    HAL_ADCEx_InjectedStart_IT(hadc_);
  }

  void stopSample_IT()
  {
    //需要先开启对应的定时器才会触发中断
    HAL_ADCEx_InjectedStop_IT(hadc_);
  }

  int8_t read2Channel (float *data,size_t length)
  {
    if (length != readData_.size()){return -1;}
    data[0] = readData_[0] / 4096.0f * 3.3f;
    data[1] = readData_[1] / 4096.0f * 3.3f;
    return 0;
  }


  int8_t read3Channel (float *data,size_t length)
  {
    if (length != readData_.size()){return -1;}
    data[0] = readData_[0] / 4096.0f * 3.3f;
    data[1] = readData_[1] / 4096.0f * 3.3f;
    data[2] = readData_[2] / 4096.0f * 3.3f;
    return 0;
  }

  int8_t read3Channel_IT (float *data,size_t length)
  {
    if (length != readData_.size()){return -1;}
    while (isDataReady_IT_ != STM32_ADC_IT_READY){}
    isDataReady_IT_ = STM32_ADC_IT_NOT_READY;
    data[0] = hadc_->Instance->JDR1 / 4096.0f * 3.3f;
    data[1] = hadc_->Instance->JDR2 / 4096.0f * 3.3f;
    data[2] = hadc_->Instance->JDR3 / 4096.0f * 3.3f;
    return 0;
  }

  void dmaCallbackHandler(ADC_HandleTypeDef *hadc) {
    if (hadc == hadc_)
      {
	//			if (hadc_->DMA_Handle->Instance->CNDTR == 0) {
	isDataReady_ = STM32_ADC_DMA_READY;
	//			}
      }
  }
  void iTCallbackHandler (ADC_HandleTypeDef *hadc)
  {
    if (hadc == hadc_)
      {
	isDataReady_IT_ = STM32_ADC_IT_READY;
      }
  }
private:
  std::vector<uint32_t>readData_;//普通模式下每个通道采集1次
  ADC_HandleTypeDef *hadc_;
  hw_stm32_adc_dma_ready_t isDataReady_=STM32_ADC_DMA_NOT_READY;
  hw_stm32_adc_it_ready_t isDataReady_IT_=STM32_ADC_IT_NOT_READY;
};



#endif /* PORT_INC_HW_PORT_ADC_H_ */
