/*
 * hw_port_dac.h
 *
 *  Created on: Apr 20, 2025
 *      Author: GAOTIANHAO
 */

#ifndef PORT_INC_HW_PORT_DAC_H_
#define PORT_INC_HW_PORT_DAC_H_

#include <cstdint>
#include <array>
#include "main.h"

// 用户可定义此宏来使用ARM数学库
// #define USE_ARM_MATH

#ifdef USE_ARM_MATH
#include "arm_math.h"
#else
#include <math.h>
#endif

enum hw_dac_channel_t
{
  STM32_DAC_CHANNEL1 = DAC_CHANNEL_1,
  STM32_DAC_CHANNEL2 = DAC_CHANNEL_2
};

enum hw_dac_mode_t
{
  STM32_DAC_DDS_MODE,
  STM32_DAC_VAR_MODE
};

template <size_t MaxPoints = 256>
class Hardware_STM32_DAC {
public:
  /**
   * @brief 初始化正弦波生成器
   * @param hdac DAC句柄
   * @param htim 定时器句柄
   * @param channel DAC通道
   * @param frequency 正弦波频率(Hz)
   * @param points 波形点数
   * @param phase 初始相位(弧度)，默认为0
   */
  float m_frequency = 1000.0f;
  uint16_t m_points = 64;
  bool m_enabled = false;
  float m_phase = 0.0f;  // 初始相位(弧度)
  hw_dac_channel_t channel_;
  uint32_t sysclk_frequency_;
  int16_t m_update_index=0;
  TIM_HandleTypeDef *htim_=nullptr;
  DAC_HandleTypeDef *hdac_=nullptr;
  hw_dac_mode_t mode_;

  Hardware_STM32_DAC() = default;

  // 移动构造函数
  Hardware_STM32_DAC(Hardware_STM32_DAC&& other)noexcept = default;

  Hardware_STM32_DAC& operator=(Hardware_STM32_DAC&& other) noexcept
  {
    // 自我赋值检查
    if (this == &other) {
	return *this;
    }

    // 先停止当前DAC输出（如果正在运行）
    if (m_enabled) {
	this->disable();
    }

    // 交换所有资源
    std::swap(hdac_, other.hdac_);
    std::swap(htim_, other.htim_);
    std::swap(channel_, other.channel_);
    std::swap(sysclk_frequency_, other.sysclk_frequency_);
    std::swap(m_waveTable, other.m_waveTable);

    // 交换状态变量
    std::swap(m_frequency, other.m_frequency);
    std::swap(m_points, other.m_points);
    std::swap(m_enabled, other.m_enabled);
    std::swap(m_phase, other.m_phase);
    std::swap(mode_, other.mode_);

    // 如果原对象有启用DAC，则在新对象中恢复
    if (other.m_enabled) {
	this->enable();
    }

    return *this;
  }

  /**
   * @brief 初始化DAC正弦波生成器
   * @param hdac DAC句柄
   * @param htim 定时器句柄
   * @param channel DAC通道
   * @param frequency 正弦波频率(Hz)
   * @param points 波形点数
   * @param phase 初始相位(弧度)，默认为0
   */
  void begin(DAC_HandleTypeDef *hdac, TIM_HandleTypeDef *htim, hw_dac_channel_t channel,
	     float frequency, uint16_t points, hw_dac_mode_t mode ,float phase = 0.0f)
  {
    if(mode != STM32_DAC_DDS_MODE){return ;}
    hdac_ = hdac;
    htim_ = htim;
    channel_ = channel;
    sysclk_frequency_ = HAL_RCC_GetSysClockFreq();
    m_phase = phase;  // 设置初始相位
    mode_=mode;
    update_sin(frequency, points,phase);
  }

  void begin(DAC_HandleTypeDef *hdac, TIM_HandleTypeDef *htim, hw_dac_channel_t channel, uint16_t points , hw_dac_mode_t mode)
  {
    if(mode != STM32_DAC_VAR_MODE){return ;}
    hdac_ = hdac;
    htim_ = htim;
    channel_ = channel;
    mode_=mode;
  }

  void update_diy(uint32_t output) {
    if (mode_ != STM32_DAC_VAR_MODE || m_points == 0 || m_points > MaxPoints) return;
    m_waveTable[m_update_index]=output;
    m_update_index=(m_update_index+1)%m_points;
  }

  /**
   * @brief 更新波形参数
   * @param frequency 新频率(Hz)
   * @param points 新点数
   * @param phase 新相位(弧度)，可选参数，不传则保持当前相位
   */
  void update_sin(float frequency, uint16_t points, float phase = NAN) {
    if (mode_ != STM32_DAC_DDS_MODE || points == 0 || points > MaxPoints || frequency <= 0) return;

    m_frequency = frequency;
    m_points = points;

    // 如果传入了新相位，则更新
    if (!isnan(phase)) {
	m_phase = phase;
    }

    uint32_t reload = static_cast<uint32_t>(sysclk_frequency_ / (points * frequency));
    __HAL_TIM_SET_AUTORELOAD(htim_, reload);

    // 计算角度增量
    const float angleIncrement = 2.0f * _PI / static_cast<float>(points);

    // 生成正弦波表，考虑初始相位
    for (uint16_t i = 0; i < points; ++i) {
	float angle = angleIncrement * i + m_phase;  // 加上初始相位
	float sineValue = calculateSine(angle);

	// 转换为DAC值 (假设12位DAC)
	m_waveTable[i] = static_cast<uint16_t>((sineValue + 1.0f) * 2047.5f);
    }
  }

  /**
   * @brief 设置初始相位
   * @param phase 新相位(弧度)
   */
  void setPhase(float phase) {
    if(mode_ != STM32_DAC_DDS_MODE) {return;}
    m_phase = phase;
    // 重新生成波形表以应用新相位
    update_sin(m_frequency, m_points);
  }

  /**
   * @brief 获取当前相位
   * @return 当前相位(弧度)
   */
  float getPhase() const { return m_phase; }

  /**
   * @brief 启用正弦波输出
   */
  void enable() {
    if (m_enabled || m_points == 0 || (m_update_index==0 && mode_ == STM32_DAC_VAR_MODE)) return;
    //在输出可变的模式(STM32_DAC_VAR_MODE)下，如果还没传入数据就不能开启搬运
    m_enabled = true;
    HAL_TIM_Base_Start_IT(htim_);
    HAL_DAC_Start_DMA(hdac_, channel_, m_waveTable.data(), m_points, DAC_ALIGN_12B_R);
  }

  /**
   * @brief 禁用正弦波输出
   */
  void disable() {
    m_enabled = false;
    HAL_DAC_Stop_DMA(hdac_, channel_);
  }

  /**
   * @brief 获取当前频率
   * @return 当前频率(Hz)
   */
  float getFrequency() const { return m_frequency; }

  /**
   * @brief 获取当前点数
   * @return 当前点数
   */
  uint16_t getPoints() const { return m_points; }

  uint32_t getOutputValue() { return HAL_DAC_GetValue(hdac_, static_cast<uint32_t>(channel_)); }

  /**
   * @brief 检查是否启用
   * @return true如果启用
   */
  bool isEnabled() const { return m_enabled; }

private:
  static constexpr float _PI = 3.14159265358979323846f;
  std::array<uint32_t, MaxPoints> m_waveTable{};

  /**
   * @brief 计算正弦值
   * @param angle 角度(弧度)
   * @return 正弦值
   */
  float calculateSine(float angle) {
#ifdef USE_ARM_MATH
    float result=arm_sin_f32(angle);
    return result;
#else
    return sinf(angle);
#endif
  }
};

#endif /* PORT_INC_HW_PORT_DAC_H_ */
