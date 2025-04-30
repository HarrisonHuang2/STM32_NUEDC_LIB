/*
 * modbus.h
 *
 *  Created on: Apr 30, 2025
 *      Author: GAOTIANHAO
 */

#ifndef PROTOCOL_PTL_MODBUS_MASTER_H_
#define PROTOCOL_PTL_MODBUS_MASTER_H_

#include "main.h"
#include "portmacro.h"
#include "hw_hal_message.h"
#include <cstdint>
#include <cstring>
#include <functional>

#ifdef __cpp_concepts
template < MessageInterface UART >
#else
template < MessageInterface UART>
#endif
class Protocol_Modbus_Master {
public:

    Protocol_Modbus_Master();

    // 初始化通信接口回调
    void init(uint8_t slaveAddress,UART *uart) {
      m_slaveAddr =  slaveAddress;
      uart_=uart;
    }

    // 读取保持寄存器（功能码03H）
    void readRegisters(uint16_t startAddr, uint16_t regCount) {
        uint8_t frame[8];
        frame[0] = m_slaveAddr;
        frame[1] = 0x03;
        frame[2] = startAddr >> 8;
        frame[3] = startAddr & 0xFF;
        frame[4] = regCount >> 8;
        frame[5] = regCount & 0xFF;
        addCRC16(frame, 6);
        uart_->send(frame, 8);
    }

    // 写多个寄存器（功能码10H）
    void writeRegisters(uint16_t startAddr, const uint16_t* values, uint16_t regCount) {
        uint8_t frame[256];
        const uint8_t byteCount = regCount * 2;

        frame[0] = m_slaveAddr;
        frame[1] = 0x10;
        frame[2] = startAddr >> 8;
        frame[3] = startAddr & 0xFF;
        frame[4] = regCount >> 8;
        frame[5] = regCount & 0xFF;
        frame[6] = byteCount;

        for(int i = 0; i < regCount; ++i) {
            frame[7 + i*2] = values[i] >> 8;
            frame[8 + i*2] = values[i] & 0xFF;
        }

        addCRC16(frame, 7 + byteCount);
        uart_->send(frame, 9 + byteCount);
    }

    void responseHandler(const uint8_t* data, size_t length) {
        if(length < 4 || !checkCRC(data, length)) return;

        const uint8_t functionCode = data[1];
        const uint8_t slaveAddr = data[0];

        if(slaveAddr != m_slaveAddr) return;

        switch(functionCode) {
            case 0x03:  // 处理读取响应
                processReadResponse(data, length);
                break;
            case 0x10:  // 处理写入响应
                // 不需要特殊处理
                break;
        }
    }

    virtual void updateSensorData(uint16_t regAddr, uint16_t rawValue)=0; // 纯虚函数，子类实现具体数据解析

    void processReadResponse(const uint8_t* data, size_t length) {
        const uint8_t byteCount = data[2];
        const uint16_t regCount = byteCount / 2;
        const uint16_t baseAddr = (data[0] << 8) | data[1];  // 实际应用中需要跟踪请求

        for(uint16_t i = 0; i < regCount; ++i) {
            const uint16_t regAddr = baseAddr + i;
            const uint16_t value = (data[3 + i*2] << 8) | data[4 + i*2];
            updateSensorData(regAddr, value);
        }
    }


    // CRC16校验算法（MODBUS）
    static uint16_t crc16(const uint8_t* data, size_t length) {
        uint16_t crc = 0xFFFF;
        while(length--) {
            crc ^= *data++;
            for(int i = 0; i < 8; ++i) {
                crc = (crc & 0x0001) ? (crc >> 1) ^ 0xA001 : (crc >> 1);
            }
        }
        return crc;
    }

    void addCRC16(uint8_t* data, size_t length) {
        uint16_t crc = crc16(data, length);
        data[length] = crc & 0xFF;
        data[length+1] = crc >> 8;
    }

    bool checkCRC(const uint8_t* data, size_t length) {
        if(length < 2) return false;
        uint16_t receivedCRC = (data[length-1] << 8) | data[length-2];
        return crc16(data, length-2) == receivedCRC;
    }

    uint8_t m_slaveAddr;
    UART *uart_;
};



#endif /* PROTOCOL_PTL_MODBUS_MASTER_H_ */
