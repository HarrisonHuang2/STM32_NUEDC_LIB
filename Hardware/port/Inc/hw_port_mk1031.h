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
enum ModbusRegisters : uint16_t {
  VOLTAGE         = 0x0048,
      CURRENT         = 0x0049,
      ACTIVE_POWER    = 0x004A,
      TOTAL_ENERGY    = 0x004B,
      POWER_FACTOR    = 0x004C,
      FREQUENCY       = 0x004D,
      APPARENT_POWER  = 0x0054,
      REACTIVE_POWER  = 0x0055,
      PHASE_ANGLE     = 0x0056
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
  Hardware_MK1031 ();
  // 获取解析后的传感器数据
  hw_mk1031_sensor_data_t m_sensorData{};
  const hw_mk1031_sensor_data_t& getData() const { return m_sensorData ;}

  void updateSensorData(uint16_t regAddr, uint16_t rawValue) override{
    // 根据实际传感器规格添加转换逻辑
    switch(regAddr) {
      case VOLTAGE:
	m_sensorData.voltage = rawValue * 0.1f;  // 示例转换
	break;
      case CURRENT:
	m_sensorData.current = rawValue * 0.01f;
	break;
      case ACTIVE_POWER:
	m_sensorData.active_power = rawValue * 1.0f;
	break;
      case TOTAL_ENERGY:
	m_sensorData.total_energy = rawValue * 0.1f;
	break;
      case POWER_FACTOR:
	m_sensorData.power_factor = rawValue * 0.01f;
	break;
      case FREQUENCY:
	m_sensorData.frequency = rawValue * 0.1f;
	break;
      case APPARENT_POWER:
	m_sensorData.apparent_power = rawValue * 1.0f;
	break;
      case REACTIVE_POWER:
	m_sensorData.reactive_power = rawValue * 1.0f;
	break;
      case PHASE_ANGLE:
	m_sensorData.phase_angle = rawValue * 0.1f;
	break;
    }
  }
};



#endif /* PORT_INC_HW_PORT_MK1031_H_ */
