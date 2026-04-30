#pragma once

class Calculator {
    public:
        Calculator();

        double calculateAverageTemp(double tempSum, int readingCount);
        double calculateEnergyUsed(double TotalVolume, double AverageTemp);
        double calculateTotalWaterCost(double TotalVolume, double WaterPrice);
        double calculateTotalEnergyCost(double EnergyUsed, double EnergyPrice);
};