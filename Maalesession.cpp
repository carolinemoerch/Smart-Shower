#include "MaaleSession.h"

MaaleSession::MaaleSession(int userId):UserId_(userId),
      Date_(0),
      TotalWaterUsed_(0.0),
      AverageFlowRate_(0.0),
      AverageTemp_(0.0),
      ShowerLength_(0),
      EnergyUsed_(0),
      AverageEnergyPrice_(0.0),
      WaterPrice_(0.065), // stat fra danva 65 kr / 1000 liter = 0.065 kr per liter
      EnergyPrice_(2.5) //kr pr kwh danmarks statistik 2025
      {};

void MaaleSession::startSession(){
      StartTime_ = std::chrono::steady_clock::now(); // timer, tager tid i sekunder.
} 

void MaaleSession::endSession(){
    EndTime_ = std::chrono::steady_clock::now();
    
    ShowerLength_ = std::chrono::duration_cast<std::chrono::seconds>
                    (EndTime_ - StartTime_).count();// count standard metode i chrono
                    //beregnes i sekunder.
}

int MaaleSession::getShowerLength()const{}
double MaaleSession::getTotalWaterUsed() const{}
double MaaleSession::getAverageFlowRate() const{}
double MaaleSession::getAverageTemp() const{}
int MaaleSession::getEnergyUsed() const{}
double MaaleSession::getWaterPrice() const{}
double MaaleSession::getEnergyPrice() const{}

void MaaleSession::setTotalWaterUsed(double volume){}
void MaaleSession::setAverageFlowRate(double flowRate){}
void MaaleSession::setAverageTemp(double temp){}
void MaaleSession::setEnergyUsed(int energy){}
void MaaleSession::setAverageEnergyPrice(double energyPrice){}

