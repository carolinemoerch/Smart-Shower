#include "MeasurementControl.h" // viser rød indtil calculator er oprettet fordi measurementcontrol er afhængig.
#include "Maalesession.h"

MeasurementControl::MeasurementControl()
    : CurrentTemp_(0.0),
      CurrentFlow_(0.0),
      TotalVolume_(0.0),
      TempSum_(0.0),
      ReadingCount_(0),
      CurrentUserId_(0),
      WaterLimit_(0.0),
      activeSession_(nullptr)
{}

void MeasurementControl::startSession() {
    if (activeSession_ != nullptr) {
        return; // der kører allerede en session
    }

    if (CurrentUserId_ == 0) {
        return; // ingen bruger valgt endnu
    }

    CurrentTemp_ = 0.0;
    CurrentFlow_ = 0.0;
    TotalVolume_ = 0.0;
    TempSum_ = 0.0;
    ReadingCount_ = 0;

    activeSession_ = new MaaleSession(CurrentUserId_);
    activeSession_->startSession();
}

void MeasurementControl::selectedUser(int userId) {
    // 
}

void MeasurementControl::waterLimitSet(double limit) {
    // 
}

void MeasurementControl::stopSession(){
 
    if (activeSession_ == nullptr) // hvis der ikke er noget activesession igang.
    return;

    (*activeSession_).endSession(); // stopper tidstagning for session
    int ShowerLength = (*activeSession_).getShowerLength(); // bruger getter til at tilgå showerlength fra activesession.

    // calculator beregninger.
    // Averagegtemp
    //AverageFlowRate
    //EnergyUsed
    //TotalWaterPrice
    

    // set attributer i active session.
    //total water used
    // average flowrate
    //average temp
    // energy used
    // average energy price

    storeSession(); //kalder metoden der gemmer activesession i databasen.

    delete activeSession_; // sletter målesession objektet og peger nu på garbage.
    activeSession_ = nullptr; // nulstiller pointer til næste session.
    TotalVolume_ = 0.0; // nulstiller attributer, så measurementcontrol er klar til ny session.
    TempSum_ = 0.0;
    ReadingCount_ = 0;

}

void MeasurementControl::storeSession() {
    if (activeSession_ == nullptr) {
        return;
    }

    std::ofstream file("session_data.txt", std::ios::app);

    if (!file.is_open()) {
        return;
    }

    file << "----- Session -----\n";
    file << "UserId: " << CurrentUserId_ << '\n';
    file << "ShowerLength: " << activeSession_->getShowerLength() << " sekunder\n";
    file << "TotalWaterUsed: " << activeSession_->getTotalWaterUsed() << " liter\n";
    file << "AverageFlowRate: " << activeSession_->getAverageFlowRate() << " L/min\n";
    file << "AverageTemp: " << activeSession_->getAverageTemp() << " C\n";
    file << "EnergyUsed: " << activeSession_->getEnergyUsed() << '\n';
    file << "WaterPrice: " << activeSession_->getWaterPrice() << " kr/L\n";
    file << "EnergyPrice: " << activeSession_->getEnergyPrice() << " kr\n";
    file << "WaterLimit: " << WaterLimit_ << " liter\n";
    file << "-------------------\n";

    file.close();
}

}



