#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "TemperatureSensor.h"

// Hardware defines
#define DS_PORT PORTA
#define DS_DDR  DDRA
#define DS_PIN  PINA
#define DS_BIT  PA0

uint8_t TemperatureSensor::reset() {
	uint8_t response = 0;

	DS_DDR |= (1 << DS_BIT);
	DS_PORT &= ~(1 << DS_BIT);
	_delay_us(480);

	DS_DDR &= ~(1 << DS_BIT);
	DS_PORT |= (1 << DS_BIT);
	_delay_us(70);

	if (!(DS_PIN & (1 << DS_BIT))) {
		response = 1;
	}

	_delay_us(410);
	return response;
}

void TemperatureSensor::writeBit(uint8_t bit) {
	DS_DDR |= (1 << DS_BIT);
	DS_PORT &= ~(1 << DS_BIT);
	_delay_us(2);

	if (bit) {
		DS_DDR &= ~(1 << DS_BIT);
	}

	_delay_us(60);

	DS_DDR &= ~(1 << DS_BIT);
	DS_PORT |= (1 << DS_BIT);
	_delay_us(2);
}

uint8_t TemperatureSensor::readBit() {
	uint8_t bit = 0;

	DS_DDR |= (1 << DS_BIT);
	DS_PORT &= ~(1 << DS_BIT);
	_delay_us(2);

	DS_DDR &= ~(1 << DS_BIT);
	DS_PORT |= (1 << DS_BIT);
	_delay_us(10);

	if (DS_PIN & (1 << DS_BIT)) {
		bit = 1;
	}

	_delay_us(50);
	return bit;
}

void TemperatureSensor::writeByte(uint8_t byte) {
	for (uint8_t i = 0; i < 8; i++) {
		writeBit(byte & 0x01);
		byte >>= 1;
	}
}

uint8_t TemperatureSensor::readByte() {
	uint8_t byte = 0;

	for (uint8_t i = 0; i < 8; i++) {
		if (readBit()) {
			byte |= (1 << i);
		}
	}

	return byte;
}

double TemperatureSensor::readTemp() {

	if (!reset()) {
		return -1000;
	}

	writeByte(0xCC); // Skip ROM
	writeByte(0x44); // Convert T

	_delay_ms(750);

	reset();
	writeByte(0xCC);
	writeByte(0xBE); // Read

	uint8_t lsb = readByte();
	uint8_t msb = readByte();

	int16_t raw = (msb << 8) | lsb;
	
	double temp = raw / 16.0;

	return temp;
}