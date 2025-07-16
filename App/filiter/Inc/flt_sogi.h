#pragma once
#ifndef SOGI_PLL_H_
#define SOGI_PLL_H_

#include <cmath>
<<<<<<< HEAD
#include "portmacro.h"

#ifdef USE_ARM_MATH
#else
constexpr float M_PI = 3.14159265358979323846f;
#endif
=======
#include <cstdint>
>>>>>>> c27c8f12d786d76fcf15a4ce22fc54c16441188c

class Flt_Sogi {
public:
    // SOGI系数
    struct SogiCoeff {
        float osg_b0, osg_b2, osg_a1, osg_a2;
        float osg_qb0, osg_qb1, osg_qb2;
    };
    // 低通滤波器系数
    struct LpfCoeff {
        float b0, b1;
    };

private:
<<<<<<< HEAD
    // v' 通路系数
    float b0_v, b1_v, b2_v;
    float a1_v, a2_v;

    // qv' 通路系数
    float b0_qv, b1_qv, b2_qv;
    float a1_qv, a2_qv;  // 分母与 v' 相同

    // 输入历史
    float x_prev1 = 0.0f;
    float x_prev2 = 0.0f;

    // v' 输出历史
    float y_prev1_v = 0.0f;
    float y_prev2_v = 0.0f;

    // qv' 输出历史
    float y_prev1_qv = 0.0f;
    float y_prev2_qv = 0.0f;

public:
    Flt_Sogi() = default;

    /**
     * 初始化 SOGI 滤波器参数
     * @param f0 目标频率 (Hz)
     * @param fs 采样频率 (Hz)
     * @param k 增益系数
     */
    void begin(float f0, float fs, float k) {
        float T = 1.0f / fs;
        float omega_d;
#ifdef USE_ARM_MATH
        omega_d = 2.0f * PI * f0;
#else
        omega_d = 2.0f * M_PI * f0;
#endif
        float omega_c = (2.0f / T) * tanf(omega_d * T / 2.0f);

        // 计算 v' 通路系数
        {
            float num[3], den[3];

            num[0] = 2.0f * k * omega_c * T;
            num[1] = 0.0f;
            num[2] = -2.0f * k * omega_c * T;

            den[0] = omega_c * omega_c * T * T + 2.0f * k * omega_c * T + 4.0f;
            den[1] = 2.0f * omega_c * omega_c * T * T - 8.0f;
            den[2] = omega_c * omega_c * T * T - 2.0f * k * omega_c * T + 4.0f;

            float a0 = den[0];
            b0_v = num[0] / a0;
            b1_v = num[1] / a0;
            b2_v = num[2] / a0;
            a1_v = den[1] / a0;
            a2_v = den[2] / a0;
        }

        // 计算 qv' 通路系数
        {
            float num[3], den[3];

            num[0] = k * omega_c * omega_c * T * T;
            num[1] = 2.0f * k * omega_c * omega_c * T * T;
            num[2] = k * omega_c * omega_c * T * T;

            den[0] = omega_c * omega_c * T * T + 2.0f * k * omega_c * T + 4.0f;
            den[1] = 2.0f * omega_c * omega_c * T * T - 8.0f;
            den[2] = omega_c * omega_c * T * T - 2.0f * k * omega_c * T + 4.0f;

            float a0 = den[0];
            b0_qv = num[0] / a0;
            b1_qv = num[1] / a0;
            b2_qv = num[2] / a0;
            a1_qv = den[1] / a0;
            a2_qv = den[2] / a0;
        }
    }

    /**
     * 输入信号经过 SOGI 滤波
     * @param input 输入信号
     * @param outputs 输出信号 [v', qv']
     */
    void filter(float input, float* outputs) {
        float output_v = b0_v * input + b1_v * x_prev1 + b2_v * x_prev2
            - a1_v * y_prev1_v - a2_v * y_prev2_v;

        float output_qv = b0_qv * input + b1_qv * x_prev1 + b2_qv * x_prev2
            - a1_qv * y_prev1_qv - a2_qv * y_prev2_qv;

        x_prev2 = x_prev1;
        x_prev1 = input;

        y_prev2_v = y_prev1_v;
        y_prev1_v = output_v;

        y_prev2_qv = y_prev1_qv;
        y_prev1_qv = output_qv;

        outputs[0] = output_v;
        outputs[1] = output_qv;
    }

    /**
     * 复位滤波器状态
     */
=======
    // 状态变量
    float u[3] = { 0.0f }, osg_u[3] = { 0.0f }, osg_qu[3] = { 0.0f };
    float u_Q[2] = { 0.0f }, u_D[2] = { 0.0f };
    float ylf[2] = { 0.0f };
    float fo = 0.0f, fn = 50.0f, theta = 0.0f;
    float cosine = 1.0f, sine = 0.0f;
    float delta_t = 1.0f / 20000.0f;
    SogiCoeff sogi_coeff;
    LpfCoeff lpf_coeff;

public:
    Flt_Sogi() = default;

    void config(float acFreq, float isrFrequency, float lpf_b0, float lpf_b1, float k = 0.5f) {
        fn = acFreq;
        delta_t = 1.0f / isrFrequency;
        // SOGI系数计算
        float wn = fn * 2.0f * 3.1415926f;
        float osgx = 2.0f * k * wn * delta_t;
        float osgy = wn * delta_t * wn * delta_t;
        float temp = 1.0f / (osgx + osgy + 4.0f);
        sogi_coeff.osg_b0 = osgx * temp;
        sogi_coeff.osg_b2 = -sogi_coeff.osg_b0;
        sogi_coeff.osg_a1 = 2.0f * (4.0f - osgy) * temp;
        sogi_coeff.osg_a2 = (osgx - osgy - 4.0f) * temp;
        sogi_coeff.osg_qb0 = 0.5f * osgy * temp;
        sogi_coeff.osg_qb1 = 2.0f * sogi_coeff.osg_qb0;
        sogi_coeff.osg_qb2 = sogi_coeff.osg_qb0;
        // LPF系数
        lpf_coeff.b0 = lpf_b0;
        lpf_coeff.b1 = lpf_b1;
    }

>>>>>>> c27c8f12d786d76fcf15a4ce22fc54c16441188c
    void reset() {
        for (int i = 0; i < 3; ++i) u[i] = osg_u[i] = osg_qu[i] = 0.0f;
        for (int i = 0; i < 2; ++i) u_Q[i] = u_D[i] = ylf[i] = 0.0f;
        fo = 0.0f; theta = 0.0f; sine = 0.0f; cosine = 1.0f;
    }

    // 主运行函数，输入采样值
    void filter(float acValue) {
        // 输入缓存
        u[0] = acValue;
        // SOGI正交信号发生器
        osg_u[0] = sogi_coeff.osg_b0 * (u[0] - u[2])
            + sogi_coeff.osg_a1 * osg_u[1]
            + sogi_coeff.osg_a2 * osg_u[2];
        osg_qu[0] = sogi_coeff.osg_qb0 * u[0]
            + sogi_coeff.osg_qb1 * u[1]
            + sogi_coeff.osg_qb2 * u[2]
            + sogi_coeff.osg_a1 * osg_qu[1]
            + sogi_coeff.osg_a2 * osg_qu[2];
        // Park变换
        u_Q[0] = cosine * osg_u[0] + sine * osg_qu[0];
        u_D[0] = cosine * osg_qu[0] - sine * osg_u[0];
        // 低通滤波器
        ylf[0] = ylf[1] + lpf_coeff.b0 * u_Q[0] + lpf_coeff.b1 * u_Q[1];
        ylf[1] = ylf[0];
        u_Q[1] = u_Q[0];
        // VCO
        fo = fn + ylf[0];
        theta += (fo * delta_t) * (2.0f * 3.1415926f);
        if (theta > 2.0f * 3.1415926f) {
            theta -= 2.0f * 3.1415926f;
            // 可选：GPIO翻转指示
            // HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_13);
        }
//        sine = std::sinf(theta);
//        cosine = std::cosf(theta);
        // 状态更新
        osg_u[2] = osg_u[1]; osg_u[1] = osg_u[0];
        osg_qu[2] = osg_qu[1]; osg_qu[1] = osg_qu[0];
        u[2] = u[1]; u[1] = u[0];
    }

    // 获取当前锁相角度、频率、正交分量等
    float getTheta() const { return theta; }
    float getFreq() const { return fo; }
    float getSine() const { return sine; }
    float getCosine() const { return cosine; }
    float getAlpha() const { return osg_u[0]; }
    float getBeta() const { return osg_qu[0]; }
    float getDQ() const { return u_D[0]; }
    float getQQ() const { return u_Q[0]; }
};

#endif // SOGI_PLL_H_
