/*
 * stm32_keyboard.h
 *
 *  Created on: May 10, 2025
 *      Author: GAOTIANHAO
 */

#ifndef INC_STM32_KEYBOARD_H_
#define INC_STM32_KEYBOARD_H_

#include "hw_port_keyboard.h"

namespace stm32_keyboard
{
  Hardware_STM32_Keyboard getKeyboard(uint8_t active_level)
  {
    Hardware_STM32_Keyboard keyboard;
    keyboard.init (active_level);
    return keyboard;
  }
}



#endif /* INC_STM32_KEYBOARD_H_ */
