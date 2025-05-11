/*
 * hw_port_keyboard.h
 *
 *  Created on: May 10, 2025
 *      Author: GAOTIANHAO
 */

#ifndef PORT_INC_HW_PORT_KEYBOARD_H_
#define PORT_INC_HW_PORT_KEYBOARD_H_

#include"main.h"

typedef void(*hw_stm32_keyboard_cb_t)();

constexpr size_t hw_stm32_keyboard_num=20;

class Hardware_STM32_Keyboard
{
public:
  Hardware_STM32_Keyboard(){};
  void init(uint8_t active_level)
  {
    _active_level=active_level;
    _key_prev=0;
  }
  void attach(uint8_t id,hw_stm32_keyboard_cb_t cb)
  {
    _cb[id]=cb;
  }
  void processHandler()
  {
    uint8_t key = getKey();
    if (key != _key_prev)
      {
	    if(_cb[key])  _cb[key]();
      }
   _key_prev=key;
  }
private:
  uint8_t getKey()
  {
    uint8_t Key_Board=0;

    HAL_GPIO_WritePin(KEY_R1_GPIO_Port,KEY_R1_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(KEY_R2_GPIO_Port,KEY_R2_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(KEY_R3_GPIO_Port,KEY_R3_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(KEY_R4_GPIO_Port,KEY_R4_Pin,GPIO_PIN_RESET);

    if(HAL_GPIO_ReadPin(KEY_C1_GPIO_Port,KEY_C1_Pin)==_active_level)     //检测到按键被按下
      {
	Key_Board = 16;
      }
    else if(HAL_GPIO_ReadPin(KEY_C2_GPIO_Port,KEY_C2_Pin)==_active_level)     //检测到按键被按下
      {
	Key_Board =15;
      }
    else if (HAL_GPIO_ReadPin (KEY_C3_GPIO_Port, KEY_C3_Pin) == _active_level) //检测到按键被按下
      {
           Key_Board = 14;
      }
    else if (HAL_GPIO_ReadPin (KEY_C4_GPIO_Port, KEY_C4_Pin) == _active_level) //检测到按键被按下
      {
	Key_Board = 13;
      }
    HAL_GPIO_WritePin(KEY_R1_GPIO_Port,KEY_R1_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(KEY_R2_GPIO_Port,KEY_R2_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(KEY_R3_GPIO_Port,KEY_R3_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(KEY_R4_GPIO_Port,KEY_R4_Pin,GPIO_PIN_RESET);

    if (HAL_GPIO_ReadPin (KEY_C1_GPIO_Port, KEY_C1_Pin) == _active_level) //检测到按键被按下
      {
	Key_Board = 12;
      }
    else if (HAL_GPIO_ReadPin (KEY_C2_GPIO_Port, KEY_C2_Pin) == _active_level) //检测到按键被按下
      {
	Key_Board = 11;
      }
    else if (HAL_GPIO_ReadPin (KEY_C3_GPIO_Port, KEY_C3_Pin) == _active_level) //检测到按键被按下
      {
	Key_Board = 10;
      }
    else if (HAL_GPIO_ReadPin (KEY_C4_GPIO_Port, KEY_C4_Pin) == _active_level) //检测到按键被按下
      {
	Key_Board = 9;
      }
    HAL_GPIO_WritePin(KEY_R1_GPIO_Port,KEY_R1_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(KEY_R2_GPIO_Port,KEY_R2_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(KEY_R3_GPIO_Port,KEY_R3_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(KEY_R4_GPIO_Port,KEY_R4_Pin,GPIO_PIN_RESET);
    if (HAL_GPIO_ReadPin (KEY_C1_GPIO_Port, KEY_C1_Pin) == _active_level) //检测到按键被按下
      {
	Key_Board = 8;
      }
    else if (HAL_GPIO_ReadPin (KEY_C2_GPIO_Port, KEY_C2_Pin) == _active_level) //检测到按键被按下
      {
	Key_Board = 7;
      }
    else if (HAL_GPIO_ReadPin (KEY_C3_GPIO_Port, KEY_C3_Pin) == _active_level) //检测到按键被按下
      {
	Key_Board = 6;
      }
    else if (HAL_GPIO_ReadPin (KEY_C4_GPIO_Port, KEY_C4_Pin) == _active_level) //检测到按键被按下
      {
	Key_Board = 5;
      }
    HAL_GPIO_WritePin(KEY_R1_GPIO_Port,KEY_R1_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(KEY_R2_GPIO_Port,KEY_R2_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(KEY_R3_GPIO_Port,KEY_R3_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(KEY_R4_GPIO_Port,KEY_R4_Pin,GPIO_PIN_SET);
    if (HAL_GPIO_ReadPin (KEY_C1_GPIO_Port, KEY_C1_Pin) == _active_level) //检测到按键被按下
      {
	Key_Board = 4;
      }
    else if (HAL_GPIO_ReadPin (KEY_C2_GPIO_Port, KEY_C2_Pin) == _active_level) //检测到按键被按下
      {
	Key_Board = 3;
      }
    else if (HAL_GPIO_ReadPin (KEY_C3_GPIO_Port, KEY_C3_Pin) == _active_level) //检测到按键被按下
      {
	Key_Board = 2;
      }
    else if (HAL_GPIO_ReadPin (KEY_C4_GPIO_Port, KEY_C4_Pin) == _active_level) //检测到按键被按下
      {
	Key_Board = 1;
      }
    return Key_Board;
  }
  uint8_t _active_level=1;
  uint8_t _key_prev=0;
  hw_stm32_keyboard_cb_t _cb[hw_stm32_keyboard_num]={};
};



#endif /* PORT_INC_HW_PORT_KEYBOARD_H_ */
