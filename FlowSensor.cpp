#include "FlowSensor.h"
#include <avr/io.h>
#include <avr/interrupt.h>

// Global variabel til ISR
volatile uint16_t flowPulseCount = 0;

// Her tælles pulser fra flowsensor
ISR(INT4_vect) {
    flowPulseCount++;
}

void FlowSensor::init_flow_sensor() {
    // pin 2 som input
    DDRE &= ~(1 << DDE4);
    
    // intern pull-up modstand på Pin 2
    PORTE |= (1 << PORTE4);
    
    // Konfigurer External Interrupt 4 (Pin 2) til at trigge på "rising edge" (når signalet går fra 0V til 5V)
    EICRB |= (1 << ISC41) | (1 << ISC40);
    
    // Aktiver External Interrupt 4, så Arduinoen lytter på Pin 2
    EIMSK |= (1 << INT4);
    
    // Nulstil tælleren for at være sikker på en ren start
    flowPulseCount = 0;
}


double FlowSensor::readFlow() {
    cli();
    uint16_t pulses = flowPulseCount;
    flowPulseCount = 0;
    sei();

    // Beregner flow i liter per minut
    double currentFlowRate = pulses / 7.5;

    double deltaVolume = currentFlowRate / 60.0;
  
    return deltaVolume;
}

void FlowSensor::volumeUpdated(double deltaVolume) {
    // Skal der sendes noget direkte til AfspilLyd?
}