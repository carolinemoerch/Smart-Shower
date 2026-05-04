#pragma once

class TemperatureSensor {
public:
    double readTemp();
	
private:
	uint8_t reset();
	void writeBit(uint8_t bit);
	uint8_t readBit();
	void writeByte(uint8_t byte);
	uint8_t readByte();
};