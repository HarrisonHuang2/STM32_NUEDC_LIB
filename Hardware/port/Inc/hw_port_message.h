/*
 * hw_port_serial.h
 *
 *  Created on: Mar 23, 2025
 *      Author: GAOTIANHAO
 */

#ifndef PORT_INC_HW_PORT_MESSAGE_H_
#define PORT_INC_HW_PORT_MESSAGE_H_


#include "usart.h"

typedef enum {
  PINGPONG_BUFFER = 0,
  number_of_event,
} ProcessEvent;

typedef void (*MessageCallback)(uint8_t *data, uint16_t len);

class Hardware_STM32_Message
{
public:

  Hardware_STM32_Message(){};
  // 移动构造函数
  Hardware_STM32_Message(Hardware_STM32_Message&& other) noexcept
  {
    // 使用 std::exchange 交换资源
    _huart = std::exchange(other._huart, nullptr);
    for (int i = 0; i < _buffer_num; i++) {
	delete[] _buffer[i];
	_buffer[i] =std::exchange(other._buffer[i], nullptr);
    }
    _buffer = std::exchange(other._buffer, nullptr);
    _buffer_num = std::exchange(other._buffer_num, 0);
    _buffer_len = std::exchange(other._buffer_len, 0);
    _data_len = std::exchange(other._data_len, 0);
    _new_data_available = std::exchange(other._new_data_available, false);
    _receiveBufferIndex = std::exchange(other._receiveBufferIndex, 0);
    _processBufferIndex = std::exchange(other._processBufferIndex, -1);
    for(int i=0; i<number_of_event; i++)
      {
	_cb[i] = std::exchange(other._cb[i], nullptr);
      }
  }

  Hardware_STM32_Message& operator=(Hardware_STM32_Message&& other) noexcept
  {
    // 自我赋值检查
    if (this == &other)
      {
	return *this;
      }


    // 使用 std::swap 交换资源
    std::swap(_huart, other._huart);
    for (int i = 0; i < _buffer_num; i++) {
	delete[] _buffer[i];
	_buffer[i] = nullptr;
	std::swap(_buffer[i], other._buffer[i]);
    }
    std::swap(_buffer_num, other._buffer_num);
    std::swap(_buffer_len, other._buffer_len);
    std::swap(_data_len, other._data_len);
    std::swap(_new_data_available, other._new_data_available);
    std::swap(_receiveBufferIndex, other._receiveBufferIndex);
    std::swap(_processBufferIndex, other._processBufferIndex);
    std::swap(_cb, other._cb);

    return *this;
  }
  void begin(uint16_t buffer_num, size_t buffer_len, UART_HandleTypeDef *huart)
  {
    if (!huart) {
	return;
    }
    if (buffer_num == 0 || buffer_len == 0) {
	return;
    }
    _buffer_num = buffer_num;
    _buffer_len = buffer_len;
    _huart = huart;
    _buffer = new uint8_t*[buffer_num];
    for (int i = 0; i < buffer_num; i++) {
	_buffer[i] = new uint8_t[buffer_len];
    }
  }

  void send(uint8_t *data, uint16_t len)
  {
    HAL_UART_Transmit(_huart, data, len,10);
  }

  void startReceive()
  {
    _startDMA();
  }

  void callbackHandler(UART_HandleTypeDef *huart, uint16_t Size) {
    if (huart == _huart) {
	_data_len = Size;
	_new_data_available = true;
	_receiveBufferIndex = (_receiveBufferIndex + 1) % _buffer_num;
	_startDMA();
    }
  }

  void attachEvent(MessageCallback cb, ProcessEvent event) {
    _cb[event] = cb;
  }

  void stopEvent(MessageCallback cb, ProcessEvent event) {
    _cb[event] = NULL;
  }

  void processHandler() {
    if (_new_data_available) {
	_new_data_available = false;
	_processBufferIndex = (_processBufferIndex + 1) % _buffer_num;
	if (_cb[PINGPONG_BUFFER]) {
	    _cb[PINGPONG_BUFFER](_buffer[_processBufferIndex], _data_len);
	}
    }
  }

  UART_HandleTypeDef *_huart;
  uint8_t **_buffer;
  uint8_t _buffer_num;
  uint8_t _buffer_len;
  int _data_len;
  bool _new_data_available = 0;
  uint8_t _receiveBufferIndex = 0;
  int8_t _processBufferIndex = -1;
  MessageCallback _cb[number_of_event];

  void _startDMA()
  {
    HAL_UARTEx_ReceiveToIdle_DMA(_huart, _buffer[_receiveBufferIndex], 2 * _buffer_len);
  }
};



#endif /* PORT_INC_HW_PORT_MESSAGE_H_ */
