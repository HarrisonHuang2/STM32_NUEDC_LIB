/*
 * alg_dc_boost.h
 *
 *  Created on: Apr 16, 2025
 *      Author: GAOTIANHAO
 */

#ifndef ALGORITHIM_INC_ALG_DC_BOOST_H_
#define ALGORITHIM_INC_ALG_DC_BOOST_H_

#include "alg_dc_buck.h"

#ifdef __cpp_concepts
template < PWMInterfaceConcept PWM, DataWrapperInterfaceConcept DATA >
#else
template <class PWM, class DATA>
#endif
class Algorithim_DC_Boost : public Algorithim_DC_DC<PWM, DATA>
{
public:
  Algorithim_DC_Boost (){}

  void openVoltageLoopControl ()
  {
    if (this->vin_ == 0 || this->vout_ < this->vin_ || !this->isEnable_)
      {
        return;
      }
    this->pwm_->setDutyCycle (LIMIT(this->vin_ / this->vout_,STM32_MIN_DUTY,STM32_MAX_DUTY)); // 输入小于输出（Boost 模式）
  }

  void closedVoltageLoopControl() {
    float output;
    if(!this->dataWrapper_ || !this->isEnable_){return ;}
    output = LIMIT(this->cv_pid_->cal_increase(this->vin_,this->dataWrapper_->readVin()),STM32_MIN_DUTY,STM32_MAX_DUTY);
    output = LIMIT(1 - output,STM32_MIN_DUTY,STM32_MAX_DUTY);
    this->pwm_->setDutyCycle(output);//默认控制上管，极性与PID输出取反
  }

  void closedCurrentLoopControl() {
    float output;
    float senorcurrent;
    if(!this->dataWrapper_ || !this->isEnable_){return ;}
//    senorcurrent = this->dataWrapper_->readCurrent(); //debug
    output = LIMIT(this->cc_pid_->cal_increase(this->current_, this->dataWrapper_->readCurrent()),STM32_MIN_DUTY,STM32_MAX_DUTY);
    output = LIMIT(1 - output,STM32_MIN_DUTY,STM32_MAX_DUTY);
    this->pwm_->setDutyCycle(output);//默认控制上管，极性与PID输出取反
//    output = output;
  }

  void closedVoltageCurrentLoopControl() {
    if(!this->dataWrapper_ || !this->isEnable_){return ;}
    static float outer=0;
    static float inner=0;
    constexpr uint16_t frequency_div=4;
    static uint16_t count=0;
    if (count == 0)
      {
	outer = LIMIT(this->cv_pid_->cal_increase(this->vin_,this->dataWrapper_->readVin()),STM32_MIN_DUTY,STM32_MAX_DUTY);
      }
    inner = LIMIT(this->cc_pid_->cal_increase(outer, this->dataWrapper_->readCurrent()),STM32_MIN_DUTY,STM32_MAX_DUTY);
    count=(count+1)%frequency_div;
    inner = LIMIT(1 - inner,STM32_MIN_DUTY,STM32_MAX_DUTY);
    this->pwm_->setDutyCycle(inner);
  }
};


#endif /* ALGORITHIM_INC_ALG_DC_BOOST_H_ */
