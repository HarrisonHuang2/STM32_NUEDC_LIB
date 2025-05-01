/*
 * t.h
 *
 *  Created on: Mar 15, 2025
 *      Author: GAOTIANHAO
 */

#ifndef ALGORITHIM_INC_ALG_DC_DC_H_
#define ALGORITHIM_INC_ALG_DC_DC_H_

#include "main.h"
#include "portmacro.h"
#include <utility>
#include "hw_hal_pwm.h"
#include "hw_hal_relay.h"
#include "hw_hal_data_wrapper.h"
#include "alg_pid.h"

//Note
/*TO DO

 */

// BuckConverter 类声明
#ifdef __cpp_concepts
template < PWMInterfaceConcept PWM ,DataWrapperInterfaceConcept DATA>
#else
template <class PWM, class DATA>
#endif
class Algorithim_DC_DC {
public:
  PWM *pwm_;
  DATA *dataWrapper_;
  Algorithim_PID *cc_pid_;
  Algorithim_PID *cv_pid_;
  bool isEnable_=false;
  float vin_;
  float vout_;
  float current_;
  Algorithim_DC_DC(){}
  void begin(PWM *pwm,DATA *dataWrapper=nullptr)
  {
    pwm_ = pwm;
    dataWrapper_=dataWrapper;
  }

  void setCV_PID(Algorithim_PID *pid)
  {
    cv_pid_ = pid;
  }

  void setCC_PID(Algorithim_PID *pid)
  {
    cc_pid_ = pid;
  }

  void setVin(float vin)
  {
    vin_ = vin;
  }

  void setVout(float vout)
  {
    vout_ = vout;
  }

  void setCurrent(float current)
  {
    current_ = current;
  }

  void enable() {
    isEnable_=true;
  }

  void disable() {
    isEnable_=false;
  }

  bool isEnable()
  {
    return isEnable_;
  }
};


#endif /* ALGORITHIM_INC_ALG_DC_DC_H_ */
