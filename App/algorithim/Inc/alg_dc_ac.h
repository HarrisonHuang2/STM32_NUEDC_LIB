/*
 * alg_dc_ac.h
 *
 *  Created on: Jun 7, 2025
 *      Author: GAOTIANHAO
 */

#ifndef ALGORITHIM_INC_ALG_DC_AC_H_
#define ALGORITHIM_INC_ALG_DC_AC_H_

#include "portmacro.h"
#include <cstdint>
#include<vector>

#include <cmath>
#ifdef USE_ARM_MATH
#include "arm_math.h"
#endif

#include "hw_hal_pwm.h"
#include "hw_hal_data_wrapper.h"

#ifdef __cpp_concepts
template <PWMInterfaceConcept PWM, DataWrapperInterfaceConcept DATA>
#else
template <class PWM, class DATA>
#endif
class Algorithim_DC_AC {
public:
  PWM *pwm_;
  DATA *dataWrapper_;
  bool isEnable_ = false;

  // SPWM相关参数
  float ratio_ = 0.9f;  // 调制比，默认0.9
  float carrierFreq_ = 0.0f;      // 载波频率(Hz)- PWM的频率
  float signalFreq_ = 0.0f;       // 调制信号频率(Hz) - 输出的正弦波频率
  std::vector<float>spwmTable_ ; //SPWM波表指针,存储的是占空比(0-1)
  uint32_t tableSize_ = 0;        // 波表大小
  size_t outputIndex_ = 0;// 用于输出索引

  Algorithim_DC_AC(){}

  Algorithim_DC_AC(Algorithim_DC_AC&& other)noexcept = default;

  Algorithim_DC_AC& operator= (Algorithim_DC_AC &&other) noexcept
    {
      if (this == &other)
	{
	  return *this;
	}
      // 交换资源
      std::swap (pwm_, other.pwm_);
      std::swap (dataWrapper_, other.dataWrapper_);
      std::swap (isEnable_, other.isEnable_);
      std::swap (ratio_, other.ratio_);
      std::swap (carrierFreq_, other.carrierFreq_);
      std::swap (signalFreq_, other.signalFreq_);
      std::swap (spwmTable_, other.spwmTable_);
      std::swap (tableSize_, other.tableSize_);
      std::swap (outputIndex_, other.outputIndex_);
      return *this;
    }

  // 初始化接口
  void begin(PWM *pwm, float carrierFreq, float signalFreq,
	     float ratio = 0.9f, DATA *dataWrapper = nullptr)
  {
    pwm_ = pwm;
    dataWrapper_ = dataWrapper;
    carrierFreq_ = carrierFreq;
    signalFreq_ = signalFreq;
    ratio_ = ratio;

    // 生成SPWM波表
    generateSPWMTable();
  }

  // 启用逆变器
  void enable() {
    isEnable_ = true;
    outputIndex_ = 0;
    // 这里可以添加PWM启动代码
  }

  // 禁用逆变器
  void disable() {
    isEnable_ = false;
    // 这里可以添加PWM停止代码
  }

  // 检查是否启用
  bool isEnable() const {
    return isEnable_;
  }

  // 设置调制比
  void setRatio(float mi) {
    ratio_ = mi;
    // 重新生成波表
    generateSPWMTable();
    outputIndex_ = 0;
  }

  void openLoopControl ()
  {
    if (!isEnable_ || spwmTable_.empty())
      {
	return;
      }
    pwm_->setDutyCycle (LIMIT(spwmTable_[outputIndex_],STM32_MIN_DUTY,STM32_MAX_DUTY));
    outputIndex_ = (outputIndex_ + 1) % tableSize_;
  }

private:
  // 生成SPWM波表
  void generateSPWMTable() {
    // 计算一个信号周期内的采样点数
    tableSize_ = static_cast<uint32_t>(carrierFreq_ / signalFreq_);

    // 创建新的波表
    spwmTable_.resize(tableSize_,0);

    // 生成SPWM波表数据
    const float twoPi = 2.0f * PI;
    const float phaseStep = twoPi / tableSize_;

    for (uint32_t i = 0; i < tableSize_; ++i) {
	// SPWM算法：0.5 * (1 + M * sin(θ))
	float angle = i * phaseStep;
	spwmTable_[i] = 0.5f * (1.0f + ratio_ * std::sin(angle));
    }
  }
};

#endif /* ALGORITHIM_INC_ALG_DC_AC_H_ */
