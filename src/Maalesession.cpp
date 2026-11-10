#include "Maalesession.h"

MaaleSession::MaaleSession(int userId):UserId_(userId),
      Date_(0),
      TotalWaterUsed_(0.0),
      AverageTemp_(0.0),
      ShowerLength_(0),
      EnergyUsed_(0.0),
      WaterPrice_(0.065), // stat fra danva 65 kr / 1000 liter = 0.065 kr per liter
      EnergyPrice_(2.5), //kr pr kwh danmarks statistik 2025
      TotalEnergyCost_(0.0),
      TotalWaterCost_(0.0) 
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

int MaaleSession::getUserId()const{return UserId_;}
int MaaleSession::getShowerLength()const{return ShowerLength_; }
double MaaleSession::getTotalWaterUsed() const{return TotalWaterUsed_;}
double MaaleSession::getAverageTemp() const{return AverageTemp_;}
double MaaleSession::getEnergyUsed() const{return EnergyUsed_;}
double MaaleSession::getWaterPrice() const{return WaterPrice_;}
double MaaleSession::getEnergyPrice() const{return EnergyPrice_;}
double MaaleSession::getTotalEnergyCost() const {return TotalEnergyCost_;}
double MaaleSession::getTotalWaterCost() const{return TotalWaterCost_;}

void MaaleSession::setTotalWaterUsed(double volume){TotalWaterUsed_ = volume;}
void MaaleSession::setAverageTemp(double temp){AverageTemp_ = temp;}
void MaaleSession::setEnergyUsed(double energy){EnergyUsed_ = energy;}
void MaaleSession::setTotalEnergyCost(double TotalEnergyCost){TotalEnergyCost_ = TotalEnergyCost;}
void MaaleSession::setTotalWaterCost(double TotalWaterCost){TotalWaterCost_ = TotalWaterCost;}

