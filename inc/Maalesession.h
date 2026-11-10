#pragma once
#include <string>
#include <chrono>

class MaaleSession {
public:
    MaaleSession(int userId); //constructor svarer til initsession fra UC1 klassediagram.

    void startSession(); // start tidtagning
    void endSession(); // stop tidtagning

    int getUserId()const;
    int getShowerLength()const;
    double getTotalWaterUsed() const;
    double getAverageTemp() const;
    double getEnergyUsed() const;
    double getWaterPrice() const;
    double getEnergyPrice() const;
    double getTotalEnergyCost() const;
    double getTotalWaterCost() const;

    void setTotalWaterUsed(double volume);
    void setAverageTemp(double temp);
    void setEnergyUsed(double energy);
    void setTotalEnergyCost(double TotalEnergyCost);
    void setTotalWaterCost(double TotalWaterCost);
    
    
    
private:
    int Date_;
    int UserId_;
    double TotalWaterUsed_;//
    double AverageTemp_;//
    int ShowerLength_;//
    double EnergyUsed_;// omskrevet til double, håber det okay -JJ
    double WaterPrice_;//
    double EnergyPrice_;//
    double TotalEnergyCost_;//
    double TotalWaterCost_;//

    std::chrono::time_point<std::chrono::steady_clock> StartTime_;
    std::chrono::time_point<std::chrono::steady_clock> EndTime_;

};