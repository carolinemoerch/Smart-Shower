#include "Calculator.h"

Calculator::Calculator(double EnergyPrice, double WaterPrice) :
    EnergyPrice_(EnergyPrice), WaterPrice_(WaterPrice) {}

double Calculator::calculateAverageTemp(double tempSum, int readingCount) {
    if (readingCount == 0) {
        return 0.0;
    }     
    return tempSum / readingCount;
}

double Calculator::calculateEnergyUsed(double TotalVolume) {
    int SpecifikVarmeKapacitet = 4186;
    return TotalVolume * SpecifikVarmeKapacitet * calculateAverageTemp(getTempSum(), getReadingCount());
}

double Calculator::calculateTotalWaterCost(double TotalVolume) {
    return TotalVolume * WaterPrice_;
}

double Calculator::calculateTotalEnergyCost(double TotalVolume) {
    return calculateEnergyUsed(TotalVolume) * EnergyPrice_;
}