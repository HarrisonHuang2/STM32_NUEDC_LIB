/*
 * mppt.h
 *
 *  Created on: Mar 18, 2025
 *      Author: GAOTIANHAO
 */

#ifndef ALGORITHIM_INC_MPPT_H_
#define ALGORITHIM_INC_MPPT_H_

#include "portmacro.h"
#include <cmath>  // 添加数学库

class Algorithim_MPPT {
public:
    float lastVoltage;      // 上一次电压
    float lastCurrent;      // 上一次电流
    float lastPower;        // 上一次功率

    float voltage;          // 当前电压
    float current;          // 当前电流
    float power;            // 当前功率

    float duty;             // 当前占空比
    float lastDuty;         // 上一次占空比

    float stepSize;         // 扰动步长
    float maxDuty;          // 最大占空比
    float minDuty;          // 最小占空比

    // 构造函数初始化
    Algorithim_MPPT() {
        lastVoltage = 0;
        lastCurrent = 0;
        lastPower = 0;

        voltage = 0;
        current = 0;
        power = 0;

        duty = 0;
        lastDuty = 0;

        stepSize = 0.01f;
        maxDuty = 1.0f;
        minDuty = 0.0f;
    }

    // 初始化函数
    void begin(float initialDuty, float stepSize_val = 0.01f,
               float minDuty_val = 0.0f, float maxDuty_val = 1.0f) {
        this->duty = LIMIT(initialDuty, minDuty_val, maxDuty_val);
        this->stepSize = stepSize_val;
        this->minDuty = minDuty_val;
        this->maxDuty = maxDuty_val;

        reset();
    }

    // 更新当前电压、电流，自动计算功率
    void getMeasured(float voltage_val, float current_val) {
        this->voltage = voltage_val;
        this->current = current_val;
        this->power = voltage_val * current_val;  // 自动计算功率
    }

    // 扰动观察法（P&O）- 直接传入采样值
    float cal_PO(float voltage_val, float current_val) {
        // 更新当前测量值
        getMeasured(voltage_val, current_val);

        float newDuty = duty;

        // 计算功率和占空比的变化
        float deltaPower = power - lastPower;
        float deltaDuty = duty - lastDuty;

        if (deltaPower > 0) {
            // 功率增加，沿相同方向继续扰动
            if (deltaDuty > 0) {
                newDuty = duty + stepSize; // 之前增大占空比，继续增大
            } else {
                newDuty = duty - stepSize; // 之前减小占空比，继续减小
            }
        } else if (deltaPower < 0) {
            // 功率减少，反向扰动
            if (deltaDuty > 0) {
                newDuty = duty - stepSize; // 之前增大占空比，现在减小
            } else {
                newDuty = duty + stepSize; // 之前减小占空比，现在增大
            }
        } else {
            // 功率没有变化，保持当前占空比或做小幅扰动
            newDuty = duty + stepSize; // 做一个小扰动来检测方向
        }

        return applyLimitAndUpdate(newDuty);
    }

    // 电导增量法 - 直接传入采样值
    float cal_IC(float voltage_val, float current_val) {
        // 更新当前测量值
        getMeasured(voltage_val, current_val);

        float newDuty = duty;

        float deltaV = voltage - lastVoltage;
        float deltaI = current - lastCurrent;
        float deltaP = power - lastPower;

        if (abs(deltaV) < 1e-6f) {  // 电压变化很小
            if (deltaI > 0) {
                newDuty = duty + stepSize; // 电流增加，增大占空比
            } else if (deltaI < 0) {
                newDuty = duty - stepSize; // 电流减小，减小占空比
            } else {
                newDuty = duty; // 电流不变，保持占空比
            }
        } else {
            // 计算 dP/dV = d(VI)/dV = I + V*(dI/dV)
            float dP_dV = deltaP / deltaV;
            float dI_dV = deltaI / deltaV;
            float conductance = current / voltage;          // G = I/V
            float incrementalConductance = dI_dV;           // dI/dV

            // MPPT条件：dP/dV = 0，即 I + V*(dI/dV) = 0，所以 dI/dV = -I/V
            if (abs(incrementalConductance + conductance) < 1e-6f) {
                newDuty = duty; // 已达到最大功率点
            } else if (incrementalConductance > -conductance) {
                newDuty = duty + stepSize; // 还未到达最大功率点，增大占空比
            } else {
                newDuty = duty - stepSize; // 超过最大功率点，减小占空比
            }
        }

        return applyLimitAndUpdate(newDuty);
    }

    // 重置MPPT状态
    void reset() {
        lastVoltage = voltage;
        lastCurrent = current;
        lastPower = voltage * current;
        lastDuty = duty;
    }

private:
    // 内部辅助函数：应用限幅并更新状态
    float applyLimitAndUpdate(float newDuty) {
        // 限幅处理
        newDuty = LIMIT(newDuty, minDuty, maxDuty);

        // 更新历史值（在应用新占空比之前）
        lastVoltage = voltage;
        lastCurrent = current;
        lastPower = power;
        lastDuty = duty;

        // 应用新的占空比
        duty = newDuty;

        return duty;
    }

};

#endif /* ALGORITHIM_INC_MPPT_H_ */
