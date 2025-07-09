/*
 * alg_pll.h
 *
 *  Created on: Jul 10, 2025
 *      Author: GAOTIANHAO
 */

#ifndef ALGORITHIM_INC_ALG_PLL_H_
#define ALGORITHIM_INC_ALG_PLL_H_

#include "portmacro.h"
#include <cmath>
#ifdef USE_ARM_MATH
#include "arm_math.h"
#endif

#include "flt_sogi.h"
#include "alg_pid.h"

class Algorithm_PLL {
private:
    // SOGI 滤波器 - 延迟初始化
    Flt_Sogi sogi_filter;  // 默认构造，后续通过 begin 初始化

    // PID 控制器 - 延迟初始化
    Algorithm_PID pid_controller;

    // 当前相位角
    float theta;

    // 当前频率偏差
    float frequency_deviation;

    // 初始相位
    float initial_phase;

    // 采样周期
    float Ts;

    bool is_initialized = false;  // 初始化标志

public:
    // 默认构造函数
    Algorithm_PLL()
        : theta(0.0f),
          frequency_deviation(0.0f),
          initial_phase(0.0f),
          Ts(0.0f),
          is_initialized(false)
    {}

    /**
     * 延迟初始化方法
     * @param f0 目标频率 (Hz)
     * @param fs 采样频率 (Hz)
     * @param kp 比例系数
     * @param ki 积分系数
     * @param kd 微分系数
     * @param integral_limit 积分限幅
     * @param output_limit 输出限幅
     * @param delta_output_limit 输出变化速率限制
     * @param initial_phase 初始相位
     */
    void begin(float f0, float fs, float kp, float ki, float kd,
               float integral_limit = std::numeric_limits<float>::infinity(),
               float output_limit = std::numeric_limits<float>::infinity(),
               float delta_output_limit = std::numeric_limits<float>::infinity(),
               float initial_phase_ = 0.0f)
    {
        // 防止重复初始化
        if (is_initialized) return;
        is_initialized = true;

        // 构造 SOGI 滤波器
        sogi_filter = Flt_Fir_Sogi(f0, fs, 1.0f);

        // 初始化 PID 控制器
        pid_controller.begin(kp, ki, kd, integral_limit, output_limit, delta_output_limit);

        // 设置初始相位和采样周期
        this->initial_phase = initial_phase_;
        this->theta = initial_phase_;
        this->Ts = 1.0f / fs;
    }

    /**
     * 更新锁相环状态
     * @param Ugrid 输入电压信号
     * @param ref 参考值
     * @return 当前相位角 theta
     */
    float update(float Ugrid, float ref)
    {
        if (!is_initialized) {
            // 如果未初始化，返回当前相位或抛出错误
            return theta;
        }

        // 1. SOGI 滤波器处理输入信号
        float outputs[2];
        sogi_filter.filter(Ugrid, outputs);
        float U_alpha = outputs[0];
        float U_beta = outputs[1];

        // 2. 帕克变换
        float Id, Iq;
        float sin_theta = std::sin(theta);
        float cos_theta = std::cos(theta);
        arm_park_f32(U_alpha, U_beta, &Id, &Iq, sin_theta, cos_theta);

        // 3. PID 控制器计算频率偏差
        frequency_deviation = pid_controller.cal_increase(ref, Iq);

        // 4. 更新相位角
        theta += frequency_deviation * Ts;
        theta = fmod(theta + M_PI, 2.0f * M_PI) - M_PI; // [-pi, pi]

        return theta;
    }

    /**
     * 重置锁相环状态
     */
    void reset()
    {
        if (!is_initialized) return;

        sogi_filter.reset();
        pid_controller.reset();
        theta = initial_phase;
        frequency_deviation = 0.0f;
    }
};

#endif /* ALGORITHIM_INC_ALG_PLL_H_ */
