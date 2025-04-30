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
template < PWMInterfaceConcept PWM, ADCInterfaceConcept ADC >
#else
template <class PWM, class ADC>
#endif
class Algorithim_DC_Buck : public Algorithim_DC_DC<PWM, ADC>
{
public:

  void openVoltageLoopControl() {
    if(this->vin_ == 0 || this->vout_ >= this->vin_ || !this->isEnable_) {return ;}
    this->pwm_->setDutyCycle(this->vout_/this->vin_);
  }

  void closedVoltageLoopControl() {
    float output;
    if(!this->adc_ || !this->isEnable_){return ;}
    float data[3];//0:vin 1:vout 2:current
    this->adc_->read3Channel(data,3);
    output = LIMIT(this->cv_pid_->cal_increase(this->vout_,data[1]),0,1);
    this->pwm_->setDutyCycle(output);
  }

  void closedCurrentLoopControl() {
    float output;
    if(!this->adc_ || !this->isEnable_){return ;}
    float data[3];//0:vin 1:vout 2:current
    this->adc_->read3Channel(data,3);
    output = LIMIT(this->cc_pid_->cal_increase(this->current_, data[2]),0,1);
    this->pwm_->setDutyCycle(output);
  }
};


#endif /* ALGORITHIM_INC_ALG_DC_BUCK_H_ */
