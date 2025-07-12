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
  // SOGI �˲��� - �ӳٳ�ʼ��
  Flt_Sogi sogi_filter;  // Ĭ�Ϲ��죬����ͨ�� begin ��ʼ��

  // PID ������ - �ӳٳ�ʼ��
  Algorithim_PID *pid_controller;

  // ��ǰ��λ��
  float theta;

  // ��ǰƵ��ƫ��
  float frequence_adjust;

  // ��ʼ��λ
  float initial_phase;

  //pid ǰ��
  float pid_feedforward = 0; // PID ǰ��ֵ

  // ��������
  float Ts;

  bool is_initialized = false;  // ��ʼ����־

public:
  // Ĭ�Ϲ��캯��
  Algorithm_PLL() = default;

  /**
   * �ӳٳ�ʼ������
   * @param f0 Ŀ��Ƶ�� (Hz)
   * @param fs ����Ƶ�� (Hz)
   * @param kp ����ϵ��
   * @param ki ����ϵ��
   * @param kd ΢��ϵ��
   * @param integral_limit �����޷�
   * @param output_limit ����޷�
   * @param delta_output_limit ����仯��������
   * @param initial_phase ��ʼ��λ
   */
  void begin(float f0, float fs,float initial_phase_,Algorithim_PID *pid_controller,float sogi_k=1.0f,float pid_feedforward=50.0f)
  {
    // ��ֹ�ظ���ʼ��
    if (is_initialized || fs <= 0) return;
    is_initialized = true;

    // ���� SOGI �˲���
    sogi_filter = Flt_Sogi(f0, fs, sogi_k);

    // // ��ʼ�� PID ������
    // pid_controller.begin(kp, ki, kd, integral_limit, output_limit, delta_output_limit);
    this->pid_controller = pid_controller;

    // ���ó�ʼ��λ�Ͳ�������
    this->initial_phase = initial_phase_;
    this->theta = initial_phase_;
    this->Ts = 1.0f / fs;
    this->pid_feedforward = pid_feedforward;
  }



  /**
   * �������໷״̬
   * @param Ugrid �����ѹ�ź�
   * @param ref �ο�ֵ
   * @return ��ǰ��λ�� theta
   */
  float update(float Ugrid, float ref)
  {
    if (!is_initialized) {
	// ���δ��ʼ�������ص�ǰ��λ���׳�����
	return theta;
    }

    // 1. SOGI �˲������������ź�
    float outputs[2];
    sogi_filter.filter(Ugrid, outputs);
    float U_alpha = outputs[0];
    float U_beta = outputs[1];

    // 2. ���˱任
    float Id, Iq;
    float sin_theta;
    float cos_theta;

#ifdef USE_ARM_MATH
    arm_sin_cos_f32(theta, &sin_theta, &cos_theta);
    arm_park_f32(U_alpha, U_beta, &Id, &Iq, sin_theta, cos_theta);
#else
    // ���û��ʹ�� ARM ��ѧ�⣬�ֶ��������Һ�����
    sin_theta = std::sin(theta);
    cos_theta = std::cos(theta);
    Id = U_alpha * cos_theta + U_beta * sin_theta;
    Iq = -U_alpha * sin_theta + U_beta * cos_theta;
#endif

    // 3. PID ����������Ƶ��ƫ��
    frequence_adjust = pid_feedforward + pid_controller->cal_increase(0, Iq);

    // 4. ������λ��
    theta += frequence_adjust * Ts;//����Ҫ��Ҫ��Tsȥ��
#ifdef USE_ARM_MATH
    theta = fmod(theta + PI, 2.0f * PI) - PI; // [-pi, pi]
#else
    theta = fmod(theta + M_PI, 2.0f * M_PI) - M_PI; // [-pi, pi]
#endif

    return theta;
  }

  /**
   * �������໷״̬
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
