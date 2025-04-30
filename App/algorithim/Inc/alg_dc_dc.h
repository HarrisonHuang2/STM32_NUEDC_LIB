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
#include "hw_hal_adc.h"
#include "hw_hal_relay.h"
#include "alg_pid.h"

//Note
/*输入电压可用户设置，也可以通过ADC读取，输入电压主要用于过压保护和切换
 * Algorithim_DC_DC类中的vout和current是设定的目标值，类中不存有adc读到的实际值
 * adc需要提供接口给du_buck类读取实际值
 */

// BuckConverter 类声明
#ifdef __cpp_concepts
template < PWMInterfaceConcept PWM, ADCInterfaceConcept ADC >
#else
template <class PWM, class ADC>
#endif
class Algorithim_DC_DC {
public:
  PWM *pwm_;
  ADC *adc_;
  Algorithim_PID *cc_pid_;
  Algorithim_PID *cv_pid_;
  bool isEnable_=false;
  float vin_;
  float vout_;
  float current_;
  Algorithim_DC_DC(){}
  void begin(PWM *pwm,ADC *adc = nullptr)
  {
    pwm_ = pwm;
    adc_ = adc;
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
