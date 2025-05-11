/*
 * hw_port_mk1031_wrapper.h
 *
 *  Created on: May 1, 2025
 *      Author: GAOTIANHAO
 */

#ifndef PORT_INC_HW_PORT_MK1031_WRAPPER_H_
#define PORT_INC_HW_PORT_MK1031_WRAPPER_H_

#include "hw_hal_data_wrapper.h"
#include "hw_port_mk1031.h"


class Hardware_MK1031_Wrapper
{
public:
  Hardware_MK1031_Wrapper(){};
  void init(Hardware_MK1031 *out,Hardware_MK1031 *in=nullptr)
  {
    mk1031_in=in;
    mk1031_out=out;
  }
  float readVin()
  {
    if(!mk1031_in){return 0;}
    return mk1031_in->m_sensorData.voltage;
  }
  float readVout()
  {
    if(!mk1031_out){return 0;}
    return mk1031_out->m_sensorData.voltage;
  }
  float readCurrent()
  {
    if(!mk1031_out){return 0;}
    return mk1031_out->m_sensorData.current;
  }

  float readCurrent_in()
  {
    if(!mk1031_in){return 0;}
    return mk1031_in->m_sensorData.current;
  }
private:
  Hardware_MK1031 *mk1031_in;
  Hardware_MK1031 *mk1031_out;
};


#endif /* PORT_INC_HW_PORT_MK1031_WRAPPER_H_ */
