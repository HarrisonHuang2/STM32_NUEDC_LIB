/*
 * alg_dc_buck.h
 *
 *  Created on: Apr 16, 2025
 *      Author: GAOTIANHAO
 */

#ifndef ALGORITHIM_INC_ALG_DC_BUCK_H_
#define ALGORITHIM_INC_ALG_DC_BUCK_H_

#include "alg_dc_dc.h"

#ifdef __cpp_concepts
template < PWMInterfaceConcept PWM ,DataWrapperInterfaceConcept DATA>
#else
template <class PWM, class DATA>
#endif
class Algorithim_DC_Buck : public Algorithim_DC_DC<PWM, DATA>
{
public:

  void openVoltageLoopControl() {
    if(this->vin_ == 0 || this->vout_ > this->vin_ || !this->isEnable_) {return ;}
    this->pwm_->setDutyCycle(LIMIT(this->vout_ / this->vin_ ,STM32_MIN_DUTY,STM32_MAX_DUTY));

  }

  void closedVoltageLoopControl() {
    float output;
    if(!this->dataWrapper_ || !this->isEnable_){return ;}
    output = LIMIT(this->cv_pid_->cal_increase(this->vout_,this->dataWrapper_->readVout()),STM32_MIN_DUTY,STM32_MAX_DUTY);
    this->pwm_->setDutyCycle(output);
  }

  void closedCurrentLoopControl() {
    float output;
    if(!this->dataWrapper_ || !this->isEnable_){return ;}
    output = LIMIT(this->cc_pid_->cal_increase(this->current_, this->dataWrapper_->readCurrent()),STM32_MIN_DUTY,STM32_MAX_DUTY);
    this->pwm_->setDutyCycle(output);
  }

  void closedVoltageCurrentLoopControl() {
    if(!this->dataWrapper_ || !this->isEnable_){return ;}
    static float outer=0;
    static float inner=0;
    constexpr uint16_t frequency_div=4;
    static uint16_t count=0;
    if (count == 0)
      {
	outer = LIMIT(this->cv_pid_->cal_increase(this->vout_,this->dataWrapper_->readVout()),STM32_MIN_DUTY,STM32_MAX_DUTY);
      }
    inner = LIMIT(this->cc_pid_->cal_increase(outer, this->dataWrapper_->readCurrent()),STM32_MIN_DUTY,STM32_MAX_DUTY);
    count=(count+1)%frequency_div;
    this->pwm_->setDutyCycle(inner);
  }
};


#endif /* ALGORITHIM_INC_ALG_DC_BUCK_H_ */
