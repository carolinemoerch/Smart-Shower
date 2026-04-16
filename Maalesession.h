#pragma once
#include <string>

class MaaleSession {
public:
    MaaleSession(int userId); //constructor
    
    
private:
    int Date_;
    int UserId_;
    double TotalWaterUsed_;
    double AverageFlowRate_;
    double AverageTemp_;
    int ShowerLength_;
    int EnergyUsed_;
    double AverageEnergyPrice_;
    double WaterPrice_;
    double EnergyPrice_;

};