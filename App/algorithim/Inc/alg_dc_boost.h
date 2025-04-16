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
template < PWMInterfaceConcept PWM, ADCInterfaceConcept ADC >
#else
template <class PWM, class ADC>
#endif
class Algorithim_DC_Boost : public Algorithim_DC_Buck<PWM, ADC>
{
public:
  Algorithim_DC_Boost (){}

  void openVoltageLoopControl ()
  {
    if (this->vin_ == 0 || this->vout_ <= this->vin_ || !this->isEnable_)
      {
        return;
      }
    this->pwm_->setDutyCycle (this->vin_ / this->vout_); // 输入小于输出（Boost 模式）
  }
};


#endif /* ALGORITHIM_INC_ALG_DC_BOOST_H_ */
