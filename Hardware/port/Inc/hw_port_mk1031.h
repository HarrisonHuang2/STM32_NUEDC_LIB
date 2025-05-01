/*
 * hw_port_mk1031.h
 *
 *  Created on: Apr 30, 2025
 *      Author: GAOTIANHAO
 */

#ifndef PORT_INC_HW_PORT_MK1031_H_
#define PORT_INC_HW_PORT_MK1031_H_

//modbus电流电压传感器

#include "ptl_modbus_master.h"
#include "hw_port_message.h"


// MODBUS寄存器地址定义
enum  ModbusRegisters : uint16_t {
  MK1031_VOLTAGE         = 0x0048,
      MK1031_CURRENT_H         = 0x0049,
      MK1031_CURRENT_L         = 0x004A,
      MK1031_ACTIVE_POWER_H    = 0x004B,
      MK1031_ACTIVE_POWER_L    = 0x004C,
      MK1031_TOTAL_ENERGY_H    = 0x004D,
      MK1031_TOTAL_ENERGY_L    = 0x004E,
      MK1031_POWER_FACTOR    = 0x004F,
      MK1031_FREQUENCY       = 0x0050,
      MK1031_APPARENT_POWER_H  = 0x0057,
      MK1031_APPARENT_POWER_L  = 0x0058,
      MK1031_REACTIVE_POWER_H  = 0x0059,
      MK1031_REACTIVE_POWER_L  = 0x005A,
      MK1031_PHASE_ANGLE     = 0x005B
};


// 传感器数据结构
struct hw_mk1031_sensor_data_t {
  float voltage;
  float current;
  float active_power;
  float total_energy;
  float power_factor;
  float frequency;
  float apparent_power;
  float reactive_power;
  float phase_angle;
};

class Hardware_MK1031 : public Protocol_Modbus_Master<Hardware_STM32_Message>
{
public:
  Hardware_MK1031 (){};
  // 获取解析后的传感器数据
  hw_mk1031_sensor_data_t m_sensorData{};
  const hw_mk1031_sensor_data_t& getData() const { return m_sensorData ;}


  void updateSensorData(const uint8_t* data, size_t length)override{
    const uint8_t byteCount = data[2];
    const uint16_t regCount = byteCount / 2;

    for(uint16_t i = 0; i < regCount; ++i) {
	const uint16_t regAddr =  this->m_first_regAddr + i;
	const uint16_t rawValue = (data[3 + i*2] << 8) | data[4 + i*2];

	// 现在只解析电压和电流寄存器，电流最大量程为63356/10000=6.3356A
	switch(regAddr) {
	  case MK1031_VOLTAGE:
	    m_sensorData.voltage = rawValue * 0.01f;  // 示例转换
	    break;
	  case MK1031_CURRENT_L:
	    m_sensorData.current = rawValue * 0.0001f;
	    break;
    }
  }
  }
};



#endif /* PORT_INC_HW_PORT_MK1031_H_ */
