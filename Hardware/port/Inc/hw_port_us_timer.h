/*
 * hw_port_us_timer.h
 *
 *  Created on: Apr 21, 2025
 *      Author: GAOTIANHAO
 */

#ifndef PORT_INC_HW_PORT_US_TIMER_H_
#define PORT_INC_HW_PORT_US_TIMER_H_

#include "hw_hal_us_timer.h"
#include "main.h"

class Hardware_STM32_US_Timer
{
public:
  TIM_HandleTypeDef *htim_=nullptr;
  bool isEnable_=false;
  uint64_t count_=0;

  Hardware_STM32_US_Timer()= default;
  void begin(TIM_HandleTypeDef *htim)
  {
    htim_=htim;
    __HAL_TIM_SET_AUTORELOAD(htim,HAL_RCC_GetSysClockFreq()/1000000);
  }
  void on()
  {
    if(!htim_)return;
    HAL_TIM_Base_Start_IT(htim_);
    isEnable_=true;
  }

  void off()
  {
    if(!htim_)return;
    HAL_TIM_Base_Stop_IT(htim_);
    isEnable_=false;
  }

  bool getState()
  {
    return isEnable_;
  }

  void callbackHandler(TIM_HandleTypeDef *htim)
  {
    if(htim == htim_)
      {
	count_++;
      }
  }

  uint32_t getUs()
  {
    return count_;
  }

  void dalay_us(uint32_t time)
  {
    //TO DO
  }

};



#endif /* PORT_INC_HW_PORT_US_TIMER_H_ */
