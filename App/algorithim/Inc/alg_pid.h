/*
 * alg_pid.h
 *
 *  Created on: Mar 18, 2025
 *      Author: GAOTIANHAO
 */

#ifndef ALGORITHIM_INC_ALG_PID_H_
#define ALGORITHIM_INC_ALG_PID_H_

#include "portmacro.h"
class Algorithim_PID {
public:
    // PID����
    float kp = 0; // ����ϵ��
    float ki = 0; // ����ϵ��
    float kd = 0; // ΢��ϵ��

    // ״̬����
    float lastError = 0; // ��һ�ε����
    float last_lastError = 0; // ���ϴ�����������ʽPID��
    float integral = 0;  // ������
    float lastOutput = 0; // ��һ�ε��������������ʽPID��
    float integral_limit = 0;  // �����޷�
    float output_limit = 0;    // ����޷�
    float delta_output_limit = 0; // ����仯��������


    // ��ʼ������
    void begin(float kp, float ki, float kd, float integral_limit = -1, float output_limit = -1, float delta_output_limit = -1) {
        this->kp = kp;
        this->ki = ki;
        this->kd = kd;
        this->lastError = 0;
        this->integral = 0;
        this->last_lastError = 0;
        this->lastOutput = 0;
        this->integral_limit = integral_limit;
        this->output_limit = output_limit;
        this->delta_output_limit = delta_output_limit;
    }

    // ����ʽPID�������
    float cal_absolute(float target_value, float measured_value) {
        float error = target_value - measured_value; // �������
        float proportional = kp * error;
        integral += error;
        if (integral_limit > 0) {
            integral = LIMIT(integral, -integral_limit, integral_limit);
        }
        float integralTerm = ki * integral;
        float derivative = kd * (error - lastError);
        float output = proportional + integralTerm + derivative;

        if (output_limit > 0) {
            output = LIMIT(output, -output_limit, output_limit);
        }

        if (delta_output_limit > 0) {//��Ϊ-1���Խ���
            // ��PID�ı仯���ʽ�������
            float output_rate = output - lastOutput;
            if (output_rate > delta_output_limit)
                output = lastOutput + delta_output_limit;
            else if (output_rate < -delta_output_limit)
                output = lastOutput - delta_output_limit;
        }

        // ����״̬����
        lastError = error;
        lastOutput = output;

        return output;
    }

    // ����ʽPID�������
    float cal_increase(float target_value, float measured_value) {
        float error = target_value - measured_value; // �������
        float proportional = kp * (error - lastError);
        float integralTerm = ki * error;
        // �޷�
        if (integral_limit > 0) {
            integralTerm = LIMIT(integralTerm, -integral_limit, integral_limit);
        }


        float derivative = kd * (error - 2 * lastError + last_lastError);
        float output = lastOutput + proportional + integralTerm + derivative;

        if (output_limit > 0) {
            output = LIMIT(output, -output_limit, output_limit);
        }

        if (delta_output_limit > 0) {//��Ϊ-1���Խ���
            // ��PID�ı仯���ʽ�������
            float output_rate = output - lastOutput;
            if (output_rate > delta_output_limit)
                output = lastOutput + delta_output_limit;
            else if (output_rate < -delta_output_limit)
                output = lastOutput - delta_output_limit;
        }

        // ����״̬����
        last_lastError = lastError;
        lastError = error;
        lastOutput = output;

        return output;
    }

    void reset()
    {
        lastError = 0;
        last_lastError = 0;
        integral = 0;
        lastOutput = 0;
    }
};



#endif /* ALGORITHIM_INC_ALG_PID_H_ */
