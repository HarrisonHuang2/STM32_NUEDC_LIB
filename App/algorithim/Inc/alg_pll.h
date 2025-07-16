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
    Flt_Sogi sogi_filter;
    Algorithim_PID *pid_controller;

    float theta;                  // 当前相位
    float frequence_adjust;       // 频率偏移
    float initial_phase;          // 初始相位
    float pid_feedforward = 0;    // PID 前馈值
    float Ts;                     // 采样周期
    bool is_initialized = false;  // 初始化标志

public:
    Algorithm_PLL() = default;

    /**
     * 延迟初始化函数
     * @param f0 目标频率 (Hz)
     * @param fs 采样频率 (Hz)
     * @param initial_phase_ 初始相位
     * @param pid_controller PID 控制器
     * @param sogi_k SOGI 滤波系数
     * @param pid_feedforward PID 前馈值
     */
    void begin(float f0, float fs, float initial_phase_, Algorithim_PID *pid_controller,
               float sogi_k = 1.0f, float pid_feedforward = 50.0f)
    {
        if (is_initialized || fs <= 0) return;
        is_initialized = true;

        sogi_filter.begin(f0, fs, sogi_k);
        this->pid_controller = pid_controller;

        this->initial_phase = initial_phase_;
        this->theta = initial_phase_;
        this->Ts = 1.0f / fs;
        this->pid_feedforward = pid_feedforward;
    }

    /**
     * 更新锁相环状态
     * @param Ugrid 电网电压信号
     * @param ref 参考值
     * @return 当前相位 theta
     */
    float update(float Ugrid, float ref)
    {
        if (!is_initialized) return theta;

        float outputs[2];
        sogi_filter.filter(Ugrid, outputs);
        float U_alpha = outputs[0];
        float U_beta = outputs[1];

        float Id, Iq;
        float sin_theta, cos_theta;

#ifdef USE_ARM_MATH
        arm_sin_cos_f32(theta, &sin_theta, &cos_theta);
        arm_park_f32(U_alpha, U_beta, &Id, &Iq, sin_theta, cos_theta);
#else
        sin_theta = std::sin(theta);
        cos_theta = std::cos(theta);
        Id = U_alpha * cos_theta + U_beta * sin_theta;
        Iq = -U_alpha * sin_theta + U_beta * cos_theta;
#endif

        frequence_adjust = pid_feedforward + pid_controller->cal_increase(0, Iq);

        theta += frequence_adjust * Ts;

#ifdef USE_ARM_MATH
        theta = fmod(theta + PI, 2.0f * PI) - PI; // 限制在 [-pi, pi]
#else
        theta = fmod(theta + M_PI, 2.0f * M_PI) - M_PI; // 限制在 [-pi, pi]
#endif

        return theta;
    }

    /**
     * 复位锁相环状态
     */
    void reset()
    {
        if (!is_initialized) return;

        sogi_filter.reset();
        pid_controller->reset();
        theta = initial_phase;
        frequence_adjust = 0.0f;
    }
};

#endif /* ALGORITHIM_INC_ALG_PLL_H_ */
