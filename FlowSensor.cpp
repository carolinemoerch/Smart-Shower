#include "FlowSensor.h"
#include <avr/io.h>
#include <avr/interrupt.h>

// Mangler init. af pins på arduino
void intit_flow_sensor() {
    // Pin 2 sættes som input
	DDRE &= ~(1 << DDE4);
	// 
	PORTE |= (1 << PORTE4);
	
	// Konfigurer External Interrupt 4 (Pin 2) til at trigge på "rising edge"
	EICRB |= (1 << ISC41) | (1 << ISC40);
	// Aktiver External Interrupt 4
	EIMSK |= (1 << INT4);
}

volatile uint16_t = flowPulseCount = 0;

// Her tælles pulser fra flowsensor
ISR(INT4_vect) {
    flowPulseCount++;
}

double FlowSensor::readFlow() {
    cli;
    uint16_t pulses = flowPulseCount;
    flowPulseCount = 0;
    sei();

    // Beregner flow i liter per minut
    double currentFlowRate = pulses / 7.5;

    double deltaVolume = currentFlowRate / 60.0;
  
    return deltaVolume;
}

void FlowSensor::volumeUpdated(double deltaVolume) {
    // Handle volume update logic
    // Skal der sendes data direkte til lyd?

}