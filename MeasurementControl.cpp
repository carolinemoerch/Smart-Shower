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
    // 
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
    double avgTemp = calculator_.calculateAverageTemp(TempSum_, ReadingCount_); // Averagegtemp beregnes via calculator
    double avgFlowRate = calculator_.calculateAverageFlowRate()// AverageFlowRate
    double energyUsed = calculator_.calculateEnergyUsed()// EnergyUsed
    double totalWaterCost = calculator_.calculateTotalWaterCost()// TotalEnergyPrice
    double totalEnergyCost = calculator_.calculateTotalEnergyCost()// TotalWaterPrice
    
    // set attributer i active session.
    (*activeSession_).setTotalWaterUsed(TotalVolume_)//total water used
    (*activeSession_).setAverageFlowRate(avgFlowRate)// average flowrate
    (*activeSession_).setAverageTemp(avgTemp)//average temp
    (*activeSession_).setEnergyUsed(energyUsed)// energy used
    (*activeSession_).setAverageEnergyPrice()// average energy price

    storeSession(); //kalder metoden der gemmer activesession i databasen.

    delete activeSession_; // sletter målesession objektet og peger nu på garbage.
    activeSession_ = nullptr; // nulstiller pointer til næste session.
    TotalVolume_ = 0.0; // nulstiller attributer, så measurementcontrol er klar til ny session.
    TempSum_ = 0.0;
    ReadingCount_ = 0;

}

void MeasurementControl::storeSession(){
    //
}



