/*
 * stm32_mk1031.h
 *
 *  Created on: May 1, 2025
 *      Author: GAOTIANHAO
 */

#ifndef INC_STM32_MK1031_H_
#define INC_STM32_MK1031_H_

#include"hw_port_mk1031.h"

namespace stm32_mk1031
{
  Hardware_MK1031 getMK1031(Hardware_STM32_Message *uart,uint8_t slaveAddress=1)
  {
    Hardware_MK1031 mk1031;
    mk1031.init(slaveAddress,uart);
    return mk1031;
  }
}




#endif /* INC_STM32_MK1031_H_ */
