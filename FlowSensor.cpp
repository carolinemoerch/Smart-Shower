#include "FlowSensor.h"
#include <avr/io.h>
#include <avr/interrupt.h>

// 
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

}