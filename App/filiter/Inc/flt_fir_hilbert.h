/*
 * flt_fir_hilbert.h
 *
 *  Created on: Apr 19, 2025
 *      Author: GAOTIANHAO
 */

#ifndef FILITER_INC_FLT_FIR_HILBERT_H_
#define FILITER_INC_FLT_FIR_HILBERT_H_

#include <array>
#include <cmath>
#include "main.h"
#include "portmacro.h"



#ifdef USE_ARM_MATH
#include "arm_math.h"
#endif

//该希尔伯特滤波器的设计参数为
//阶数为120，采样频率为1khz
class Flt_Fir_Hilbert {
public:
  Flt_Fir_Hilbert() {
    reset();
  }

  // 统一处理接口
  float process(float input) {
#ifdef USE_ARM_MATH
    float output;
    arm_fir_f32(&arm_instance, &input, &output, 1);
    return output;
#else
    buffer[buffer_index] = input;
    float output = 0.0f;
    for (size_t i = 0; i <= order; ++i) {
	size_t idx = (buffer_index + order - i) % buffer_size;
	output += coefficients[i] * buffer[idx];
    }
    buffer_index = (buffer_index + 1) % buffer_size;
    return output;
#endif
  }

  // 批处理（可选扩展）
  void process_block(float* in, float* out, size_t size) {
      #ifdef USE_ARM_MATH
      arm_fir_f32(&arm_instance, in, out, size); // ARM优化批处理
      #else
      for (size_t i = 0; i < size; ++i) {
          out[i] = process(in[i]); // 复用单样本处理
      }
      #endif
  }

  // 重置滤波器状态
  void reset() {
    buffer.fill(0.0f);
    buffer_index = 0;
#ifdef USE_ARM_MATH
    arm_fir_init_f32(&arm_instance,
		     order + 1,
		     const_cast<float*>(coefficients.data()),
		     arm_state.data(),
		     1);
#endif
  }

  // 获取理论相位差
  float get_phase_shift() const { return 90.0f; }

private:
  // 滤波器参数（与MATLAB设计匹配）
  static constexpr uint32_t order = 60;      // 滤波器阶数
  static constexpr float fs = 10000.0f;        // 采样率
  static constexpr size_t buffer_size = order + 1;

  // MATLAB生成的滤波器系数（通过firpm设计）
  static constexpr std::array<float, order + 1> coefficients = {
      -0.000466f, -0.140387f, 0.000448f, -0.021502f, 0.000041f,
      -0.023434f, 0.000018f, -0.025730f, 0.000013f, -0.028485f,
      -0.000000f, -0.031819f, -0.000023f, -0.035907f, -0.000042f,
      -0.041047f, -0.000039f, -0.047733f, -0.000005f, -0.056816f,
      0.000043f, -0.069880f, 0.000072f, -0.090305f, 0.000046f,
      -0.126887f, -0.000037f, -0.211944f, -0.000035f, -0.636426f,
      0.000000f, 0.636426f, 0.000035f, 0.211944f, 0.000037f,
      0.126887f, -0.000046f, 0.090305f, -0.000072f, 0.069880f,
      -0.000043f, 0.056816f, 0.000005f, 0.047733f, 0.000039f,
      0.041047f, 0.000042f, 0.035907f, 0.000023f, 0.031819f,
      0.000000f, 0.028485f, -0.000013f, 0.025730f, -0.000018f,
      0.023434f, -0.000041f, 0.021502f, -0.000448f, 0.140387f,
      0.000466f
  };

  // 滤波器状态
  std::array<float, buffer_size> buffer;//这个buffer的大小其实多大都可以
  size_t buffer_index = 0;

#ifdef USE_ARM_MATH
  arm_fir_instance_f32 arm_instance;
  std::array<float, order + 1> arm_state;
#endif
};

#endif /* FILITER_INC_FLT_FIR_HILBERT_H_ */
