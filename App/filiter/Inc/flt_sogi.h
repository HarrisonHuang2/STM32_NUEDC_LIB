#ifndef FILITER_INC_FLT_SOGI_H_
#define FILITER_INC_FLT_SOGI_H_

#include <cmath>
#include "portmacro.h"
#include "main.h"

#ifdef USE_ARM_MATH
#include "arm_math.h"
#endif

class Flt_Sogi {
private:
    // v' 通道系数
    float b0_v, b1_v, b2_v;
    float a1_v, a2_v;

    // qv' 通道系数
    float b0_qv, b1_qv, b2_qv;
    float a1_qv, a2_qv;  // 通常与 v' 通道相同

    // 公共输入状态
    float x_prev1 = 0.0f;
    float x_prev2 = 0.0f;

    // v' 输出状态
    float y_prev1_v = 0.0f;
    float y_prev2_v = 0.0f;

    // qv' 输出状态
    float y_prev1_qv = 0.0f;
    float y_prev2_qv = 0.0f;

public:
    /**
     * 构造函数：自动根据目标频率、采样周期、增益系数计算系数
     * @param f0 目标频率 (Hz)
     * @param fs 采样频率 (Hz)
     * @param k 增益系数
     */
    Flt_Fir_Sogi(float f0, float fs, float k) {
        float T = 1.0f / fs;
        float omega_d = 2.0f * PI * f0;
        float omega_c = (2.0f / T) * tanf(omega_d * T / 2.0f);

        // === v' 通道系数计算 ===
        {
            float num[3], den[3];

            // 分子系数
            num[0] = 2.0f * k * omega_c * T;
            num[1] = 0.0f;
            num[2] = -2.0f * k * omega_c * T;

            // 分母系数
            den[0] = omega_c * omega_c * T * T + 2.0f * k * omega_c * T + 4.0f;
            den[1] = 2.0f * omega_c * omega_c * T * T - 8.0f;
            den[2] = omega_c * omega_c * T * T - 2.0f * k * omega_c * T + 4.0f;

            // 归一化
            float a0 = den[0];
            b0_v = num[0] / a0;
            b1_v = num[1] / a0;
            b2_v = num[2] / a0;
            a1_v = den[1] / a0;
            a2_v = den[2] / a0;
        }

        // === qv' 通道系数计算 ===
        {
            float num[3], den[3];

            // 分子系数
            num[0] = k * omega_c * omega_c * T * T;
            num[1] = 2.0f * k * omega_c * omega_c * T * T;
            num[2] = k * omega_c * omega_c * T * T;

            // 分母系数（与 v' 通道相同）
            den[0] = omega_c * omega_c * T * T + 2.0f * k * omega_c * T + 4.0f;
            den[1] = 2.0f * omega_c * omega_c * T * T - 8.0f;
            den[2] = omega_c * omega_c * T * T - 2.0f * k * omega_c * T + 4.0f;

            // 归一化
            float a0 = den[0];
            b0_qv = num[0] / a0;
            b1_qv = num[1] / a0;
            b2_qv = num[2] / a0;
            a1_qv = den[1] / a0;
            a2_qv = den[2] / a0;
        }
    }

    /**
     * 处理一个输入样本，输出两个正交分量
     * @param input 输入信号
     * @param outputs 输出数组（outputs[0] = v', outputs[1] = qv'）
     */
    void filter(float input, float* outputs) {
        // 计算 v' 通道
        float output_v = b0_v * input + b1_v * x_prev1 + b2_v * x_prev2
                       - a1_v * y_prev1_v - a2_v * y_prev2_v;

        // 计算 qv' 通道
        float output_qv = b0_qv * input + b1_qv * x_prev1 + b2_qv * x_prev2
                        - a1_qv * y_prev1_qv - a2_qv * y_prev2_qv;

        // 更新公共输入状态
        x_prev2 = x_prev1;
        x_prev1 = input;

        // 更新 v' 状态
        y_prev2_v = y_prev1_v;
        y_prev1_v = output_v;

        // 更新 qv' 状态
        y_prev2_qv = y_prev1_qv;
        y_prev1_qv = output_qv;

        // 赋值输出
        outputs[0] = output_v;
        outputs[1] = output_qv;
    }

    // 重置所有状态
    void reset() {
        x_prev1 = x_prev2 = 0.0f;
        y_prev1_v = y_prev2_v = 0.0f;
        y_prev1_qv = y_prev2_qv = 0.0f;
    }
};

#endif /* FILITER_INC_FLT_SOGI_H_ */
