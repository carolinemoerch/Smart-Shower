#include "TemperatureSensor.h"

double TemperatureSensor::readTemp() {

    

    return 25.0;
}



#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define DS_PORT PORTA
#define DS_DDR  DDRA
#define DS_PIN  PINA
#define DS_BIT  PA0


void UART_init(uint32_t baud) {
	uint16_t ubrr = F_CPU / 16 / baud - 1;
	UBRR0H = (uint8_t)(ubrr >> 8);
	UBRR0L = (uint8_t)ubrr;
	UCSR0B = (1 << TXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void UART_sendString(char* str) {
	while (*str) {
		while (!(UCSR0A & (1 << UDRE0)));
		UDR0 = *str++;
	}
}


uint8_t ds18b20_reset() {
	uint8_t response = 0;
	
	DS_DDR |= (1 << DS_BIT);    // Sæt pin som output
	DS_PORT &= ~(1 << DS_BIT);  // Træk pin LAV
	_delay_us(480);             // Vent i mindst 480us
	
	DS_DDR &= ~(1 << DS_BIT);   // Sæt pin som input (slip linjen)
	DS_PORT |= (1 << DS_BIT);   // Aktiver intern pull-up som hjælp
	_delay_us(70);              
	
	if (!(DS_PIN & (1 << DS_BIT))) {
		response = 1;           
	}
	
	_delay_us(410);             
	return response;
}

// Skriver en enkelt bit til sensoren
void ds18b20_write_bit(uint8_t bit) {
	DS_DDR |= (1 << DS_BIT);    // Output
	DS_PORT &= ~(1 << DS_BIT);  // Træk LAV
	_delay_us(2);               // Kort delay
	
	if (bit) {
		DS_DDR &= ~(1 << DS_BIT); // Hvis bit er 1, slip linjen tidligt
	}
	_delay_us(60);              // Hold i 60us
	
	DS_DDR &= ~(1 << DS_BIT);   // Slip linjen uanset hvad
	DS_PORT |= (1 << DS_BIT);
	_delay_us(2);               // Recovery tid
}

// Læser en enkelt bit fra sensoren
uint8_t ds18b20_read_bit() {
	uint8_t bit = 0;
	
	DS_DDR |= (1 << DS_BIT);    // Output
	DS_PORT &= ~(1 << DS_BIT);  // Træk LAV
	_delay_us(2);
	
	DS_DDR &= ~(1 << DS_BIT);   // Sæt til input for at læse
	DS_PORT |= (1 << DS_BIT);   // Intern pull-up
	_delay_us(10);              // Vent 10us før der læses
	
	if (DS_PIN & (1 << DS_BIT)) {
		bit = 1;
	}
	
	_delay_us(50);              // Vent resten af de 60us
	return bit;
}

// Skriver en hel byte (8 bits)
void ds18b20_write_byte(uint8_t byte) {
	for (uint8_t i = 0; i < 8; i++) {
		ds18b20_write_bit(byte & 0x01); // Send den mindst betydende bit først
		byte >>= 1;                     // Skift til højre
	}
}

// Læser en hel byte (8 bits)
uint8_t ds18b20_read_byte() {
	uint8_t byte = 0;
	for (uint8_t i = 0; i < 8; i++) {
		if (ds18b20_read_bit()) {
			byte |= (1 << i);           // Sæt den læste bit på den rigtige plads
		}
	}
	return byte;
}


int main(void) {
	UART_init(9600);
	char buffer[60];
	char tempStr[10];
	
	UART_sendString("DS18B20 Temp Sensor Test Startet...\r\n");
	UART_sendString("Sikre at Tera Term er sat til 9600 baud.\r\n");
	_delay_ms(1000);

	while (1) {
		// 1. Tjek om sensoren er forbundet
		if (!ds18b20_reset()) {
			UART_sendString("Fejl: Kunne ikke finde DS18B20 sensoren!\r\n");
			_delay_ms(2000);
			continue;
		}

		// 2. Bed sensoren om at måle temperaturen
		ds18b20_write_byte(0xCC); 
		ds18b20_write_byte(0x44); 

		// 3. Vent på at målingen bliver færdig (DS18B20 tager op til 750ms om dette)
		_delay_ms(750);

		// 4. Reset igen og bed om dataene
		ds18b20_reset();
		ds18b20_write_byte(0xCC); // "Skip ROM"
		ds18b20_write_byte(0xBE); // "Read Scratchpad" (Hent data)

		// 5. Læs de to første bytes fra sensoren (som indeholder temperaturen)
		uint8_t lsb = ds18b20_read_byte(); // Mindst betydende byte
		uint8_t msb = ds18b20_read_byte(); // Mest betydende byte

		// 6. Sæt de to bytes sammen til et 16-bit heltal
		int16_t raw_temp = (msb << 8) | lsb;

		// 7. DS18B20 gemmer temperaturen i 1/16 dele grader. Divider med 16.0 for at få Celsius.
		float temperature = raw_temp / 16.0;

		// 8. Formater og print til Tera Term
		dtostrf(temperature, 5, 2, tempStr);
		sprintf(buffer, "Temperatur: %s C\r\n", tempStr);
		UART_sendString(buffer);
		
		_delay_ms(250); // Lidt ekstra pause inden næste måling
	}
}