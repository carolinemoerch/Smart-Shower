#pragma once
#include "MeasurementControl.h"
#include "Maalesession.h"

class Calculator {
    public:
        Calculator(double EnergyPrice, double WaterPrice);

        double calculateAverageTemp(double tempSum, int readingCount);
        double calculateEnergyUsed(double TotalVolume);
        double calculateTotalWaterCost(double TotalVolume);
        double calculateTotalEnergyCost(double TotalVolume);

    private:
        double EnergyPrice_;
        double WaterPrice_;
};