#include "Calculator.h"

Calculator::Calculator() {}

double Calculator::calculateAverageTemp(double tempSum, int readingCount) {
    if (readingCount == 0) {
        return 0.0;
    }     
    return tempSum / readingCount;
}

double Calculator::calculateEnergyUsed(double TotalVolume, double AverageTemp) {
    int SpecifikVarmeKapacitet = 4186;
    int JouleToKwh = 3600000;
    int DeltaTemp = AverageTemp - 8;
    return (TotalVolume * SpecifikVarmeKapacitet * DeltaTemp) / JouleToKwh; 
}

double Calculator::calculateTotalWaterCost(double TotalVolume, double WaterPrice) {
    return TotalVolume * WaterPrice;
}

double Calculator::calculateTotalEnergyCost(double EnergyUsed, double EnergyPrice) {
    return EnergyUsed * EnergyPrice;
}