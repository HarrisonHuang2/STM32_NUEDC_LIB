/*
 * alg_dc_buck_boost.h
 *
 *  Created on: Apr 16, 2025
 *      Author: GAOTIANHAO
 */

#ifndef ALGORITHIM_INC_ALG_DC_BUCK_BOOST_H_
#define ALGORITHIM_INC_ALG_DC_BUCK_BOOST_H_

#include "alg_dc_buck.h"

#ifdef __cpp_concepts
template < PWMInterfaceConcept PWM, ADCInterfaceConcept ADC >
#else
template <class PWM, class ADC>
#endif
class Algorithim_DC_Buck_Boost : public Algorithim_DC_Buck<PWM, ADC>
{
  //四种模式:正方向为buck、负方向为boost，电压源用双环，电流源用单环
public:
  void closedBuckVoltageLoopControl() {
    //双环-电压外环电流内环
    if(!this->adc_out_ || !this->isEnable_){return ;}
    float outer = this->cv_pid_->cal_absolute(this->vout_, this->adc_out_->readVoltage());
    float inner = this->cc_pid_->cal_absolute(outer, this->adc_out_->readCurrent());
    float output = LIMIT(inner, 0, 1);
    this->pwm_->setDutyCycle(output);
  }

  void closedBoostVoltageLoopControl() {
    //双环-电压外环电流内环
    if(!this->adc_in_ || !this->adc_out_ || !this->isEnable_){return ;}
    float outer = this->cv_pid_->cal_absolute(this->vout_, this->adc_in_->readVoltage());
    float inner = this->cc_pid_->cal_absolute(-outer, this->adc_out_->readCurrent());
    float output = LIMIT(inner, 0, 1);
    this->pwm_->setDutyCycle(output);
  }

  void closedBuckCurrentLoopControl() {
    //单电流环
    if(!this->adc_out_ || !this->isEnable_){return ;}
    float output = LIMIT(this->cc_pid_->cal_absolute(this->current_, this->adc_out_->readCurrent()), 0, 1);
    this->pwm_->setDutyCycle(output);
  }

  void closedBoostCurrentLoopControl() {
    //单电流环
    if(!this->adc_out_ || !this->isEnable_){return ;}
    float output = LIMIT(this->cc_pid_->cal_absolute(-this->current_, this->adc_out_->readCurrent()), 0, 1);
    this->pwm_->setDutyCycle(output);
  }
};



#endif /* ALGORITHIM_INC_ALG_DC_BUCK_BOOST_H_ */
