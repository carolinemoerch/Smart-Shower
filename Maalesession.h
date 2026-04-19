#pragma once
#include <string>
#include <chrono>

class MaaleSession {
public:
    MaaleSession(int userId); //constructor svarer til initsession fra UC1 klassediagram.

    void startSession(); // start tidtagning
    void endSession(); // stop tidtagning

    int getShowerLength()const;
    double getTotalWaterUsed() const;
    double getAverageFlowRate() const;
    double getAverageTemp() const;
    int getEnergyUsed() const;
    double getWaterPrice() const;
    double getEnergyPrice() const;

    void setTotalWaterUsed(double volume);
    void setAverageFlowRate(double flowRate);
    void setAverageTemp(double temp);
    void setEnergyUsed(int energy);
    void setAverageEnergyPrice(double energyPrice);
    
    
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

    std::chrono::time_point<std::chrono::steady_clock> StartTime_;
    std::chrono::time_point<std::chrono::steady_clock> EndTime_;

};