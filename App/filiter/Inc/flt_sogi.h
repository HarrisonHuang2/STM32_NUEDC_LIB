#ifndef FILITER_INC_FLT_SOGI_H_
#define FILITER_INC_FLT_SOGI_H_

#include <cmath>
#include "portmacro.h"

#ifdef USE_ARM_MATH
#else
constexpr float M_PI = 3.14159265358979323846f;
#endif

class Flt_Sogi {
private:
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
    void reset() {
        x_prev1 = x_prev2 = 0.0f;
        y_prev1_v = y_prev2_v = 0.0f;
        y_prev1_qv = y_prev2_qv = 0.0f;
    }
};

#endif /* FILITER_INC_FLT_SOGI_H_ */
