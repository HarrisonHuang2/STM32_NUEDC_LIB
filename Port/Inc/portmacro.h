/*
 * portmacro.h
 *
 *  Created on: Mar 17, 2025
 *      Author: GAOTIANHAO
 */

#ifndef INC_PORTMACRO_H_
#define INC_PORTMACRO_H_


//这个文件夹用来放置一些常用的宏定义，以及外部库依赖
/*当前的外部库列表
 concepts
 coroutine
 */
#if __has_include(<version>)
#include <version>
#ifdef __cpp_lib_concepts
#include <concepts>
#endif
#ifdef __cpp_lib_coroutine
#include <coroutine>
#endif
#endif

#include <utility>

// 用户配置开关（取消注释使用ARM数学库）
#define USE_ARM_MATH
#define ARM_MATH_CM4

#define LIMIT(value, minVal, maxVal) (((value) < (minVal)) ? (minVal) : (((value) > (maxVal)) ? (maxVal) : (value)))

#define STM32_MIN_DUTY (0.001f)
#define STM32_MAX_DUTY (0.9999f)

#endif /* INC_PORTMACRO_H_ */
