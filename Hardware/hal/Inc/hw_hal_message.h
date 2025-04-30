/*
 * hw_hal_message.h
 *
 *  Created on: Apr 30, 2025
 *      Author: GAOTIANHAO
 */

#ifndef HAL_INC_HW_HAL_MESSAGE_H_
#define HAL_INC_HW_HAL_MESSAGE_H_


#include "main.h"
#include "portmacro.h"


#ifdef __cpp_concepts
template <typename T>

//接收是异步接收，这个concept只处理发送任务
concept MessageInterface = requires(T t, uint8_t *data, size_t len) {
  { t.send(data, len)}-> std::same_as<void>;
};
#endif


#endif /* HAL_INC_HW_HAL_MESSAGE_H_ */
