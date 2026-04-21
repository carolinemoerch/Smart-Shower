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

void MeasurementControl::beginMeasurement() { 
    if (activeSession_ == nullptr) return; // hvis session ikke er påbegyndt
    CurrentTemp_ = tempSensor_.readTemp(); // læsning fra sensor og gem i variablen CurrentTemp_
    CurrentFlow_ = flowSensor_.readFlow(); // læsning fra sensor og gem i variablen CurrentFlow_

    TempSum_ += CurrentTemp_; // TempSum opdateres for pr ny måling
    TotalVolume_ += CurrentFlow_; // TotalVolume opdateres pr ny måling

    ReadingCount_ ++; // målinger optælles pr ny måling
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
    double avgTemp = calculator_.calculateAverageTemp(TempSum_, ReadingCount_); // Averagegtemp bruger summen af temp målinger og antal readings der er foretaget.
    double energyUsed = calculator_.calculateEnergyUsed(TotalVolume_,avgTemp)// EnergyUsed bruger den totale vandvolume og gennemsnits temperatur til beregn.
    double totalWaterCost = calculator_.calculateTotalWaterCost(TotalVolume_, (*activeSession_).getWaterPrice() )// TotalEnergyPrice bruger totale vandvolume og vandprisen
    double totalEnergyCost = calculator_.calculateTotalEnergyCost(energyUsed,(*activeSession_).getEnergyPrice())// TotalWaterPrice bruger energi brugt og energi prisen.
    
    // set attributer i active session.
    (*activeSession_).setTotalWaterUsed(TotalVolume_);//total water used
    (*activeSession_).setAverageTemp(avgTemp);//average temp
    (*activeSession_).setEnergyUsed(energyUsed);// energy used
    (*activeSession_).setTotalWaterCost(totalWaterCost);// totale water pris
    (*activeSession_).setTotalEnergyCost(totalEnergyCost);// totale energy pris

    storeSession(); //kalder metoden der gemmer activesession i databasen.

    delete activeSession_; // sletter målesession objektet og peger nu på garbage.
    activeSession_ = nullptr; // nulstiller pointer til næste session.
    TotalVolume_ = 0.0; // nulstiller attributer, så measurementcontrol er klar til ny session.
    TempSum_ = 0.0;
    ReadingCount_ = 0;

}

#include <fstream>

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
    file << "AverageTemp: " << activeSession_->getAverageTemp() << " C\n";
    file << "EnergyUsed: " << activeSession_->getEnergyUsed() << '\n';
    file << "WaterPrice: " << activeSession_->getWaterPrice() << " kr/L\n";
    file << "EnergyPrice: " << activeSession_->getEnergyPrice() << " kr\n";
    file << "TotalWaterCost: " << activeSession_->getTotalWaterCost() << " kr\n";
    file << "TotalEnergyCost: " << activeSession_->getTotalEnergyCost() << " kr\n";
    file << "WaterLimit: " << WaterLimit_ << " liter\n";
    file << "-------------------\n";
}
}

}



