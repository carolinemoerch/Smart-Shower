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
    return (TotalVolume * SpecifikVarmeKapacitet * AverageTemp) / 3600000.0; //divideret med 36000... for at få kWh 
}

double Calculator::calculateTotalWaterCost(double TotalVolume, double WaterPrice) {
    return TotalVolume * WaterPrice;
}

double Calculator::calculateTotalEnergyCost(double EnergyUsed, double EnergyPrice) {
    return EnergyUsed * EnergyPrice;
}