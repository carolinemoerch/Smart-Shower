#include "MeasurementControl.h"
#include "Maalesession.h"
#include <fstream>

const std::string LIVE_FILE_PATH    = "live_data.txt";
const std::string SESSION_FILE_PATH = "session_data.txt";

MeasurementControl::MeasurementControl()
    : CurrentTemp_(0.0),
      CurrentFlow_(0.0),
      TotalVolume_(0.0),
      TempSum_(0.0),
      ReadingCount_(0),
      CurrentUserId_(0),
      WaterLimit_(10.0),
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
    ReadingCount_++; // målinger optælles pr ny måling

    // Beregn løbende pris så Flask kan vise den på live-siden
    double avgTemp    = calculator_.calculateAverageTemp(TempSum_, ReadingCount_);
    double energyUsed = calculator_.calculateEnergyUsed(TotalVolume_, avgTemp);
    double totalCost  = calculator_.calculateTotalWaterCost(TotalVolume_, activeSession_->getWaterPrice())
                      + calculator_.calculateTotalEnergyCost(energyUsed, activeSession_->getEnergyPrice());

    // Skriv live data til fil så Flask kan læse det
    std::ofstream liveFile(LIVE_FILE_PATH);
    liveFile << "water=" << TotalVolume_ << "\n";
    liveFile << "temp="  << CurrentTemp_ << "\n";
    liveFile << "cost="  << totalCost    << "\n";
}

void MeasurementControl::selectedUser(int userId) {
    for(const auto& user : Users_){
        if (user.getId() == userId){
            CurrentUserId_ = userId;
            return;
        }
    }
}

void MeasurementControl::waterLimitSet(double limit) {
    if(limit < 0){ // sikrer vi ikke kommer under 0 L waterlimit
        WaterLimit_ = 0;
    }
    else if(limit > 100){ // sikrer vi ikke kommer over 100L waterlimit
        WaterLimit_ = 100;
    }
    else
    WaterLimit_ = limit;
}

void MeasurementControl::incLimit(){
    waterLimitSet(WaterLimit_+1);
}

void MeasurementControl::decLimit(){
    waterLimitSet(WaterLimit_-1);
}

void MeasurementControl::stopSession(){
    if (activeSession_ == nullptr) // hvis der ikke er noget activesession igang.
    return;

    (*activeSession_).endSession(); // stopper tidstagning for session
    int ShowerLength = (*activeSession_).getShowerLength(); // bruger getter til at tilgå showerlength fra activesession.

    // calculator beregninger.
    double avgTemp         = calculator_.calculateAverageTemp(TempSum_, ReadingCount_); // AverageTemp bruger summen af temp målinger og antal readings der er foretaget.
    double energyUsed      = calculator_.calculateEnergyUsed(TotalVolume_, avgTemp); // EnergyUsed bruger den totale vandvolume og gennemsnits temperatur til beregn.
    double totalWaterCost  = calculator_.calculateTotalWaterCost(TotalVolume_, (*activeSession_).getWaterPrice()); // TotalWaterCost bruger totale vandvolume og vandprisen.
    double totalEnergyCost = calculator_.calculateTotalEnergyCost(energyUsed, (*activeSession_).getEnergyPrice()); // TotalEnergyCost bruger energi brugt og energi prisen.

    // set attributer i active session.
    (*activeSession_).setTotalWaterUsed(TotalVolume_); // total water used
    (*activeSession_).setAverageTemp(avgTemp); // average temp
    (*activeSession_).setEnergyUsed(energyUsed); // energy used
    (*activeSession_).setTotalWaterCost(totalWaterCost); // totale water pris
    (*activeSession_).setTotalEnergyCost(totalEnergyCost); // totale energy pris

    storeSession(); // kalder metoden der gemmer activesession i databasen.

    delete activeSession_; // sletter målesession objektet og peger nu på garbage.
    activeSession_ = nullptr; // nulstiller pointer til næste session.
    TotalVolume_  = 0.0; // nulstiller attributer, så measurementcontrol er klar til ny session.
    TempSum_      = 0.0;
    ReadingCount_ = 0;
}

void MeasurementControl::storeSession() {
    if (activeSession_ == nullptr) {
        return; // der er ingen aktiv session at gemme
    }

    Database_.saveSession(*activeSession_);

    // Skriv til session_data.txt så Flask kan læse det til statistik-siden
    std::ofstream file(SESSION_FILE_PATH, std::ios::app);
    if (!file.is_open()) {
        return; // filen kunne ikke åbnes
    }

    file << "----- Session -----\n";
    file << "UserId: "          << CurrentUserId_                        << '\n';
    file << "ShowerLength: "    << activeSession_->getShowerLength()     << " sekunder\n";
    file << "TotalWaterUsed: "  << activeSession_->getTotalWaterUsed()   << " liter\n";
    file << "AverageTemp: "     << activeSession_->getAverageTemp()      << " C\n";
    file << "EnergyUsed: "      << activeSession_->getEnergyUsed()       << '\n';
    file << "WaterPrice: "      << activeSession_->getWaterPrice()       << " kr/L\n";
    file << "EnergyPrice: "     << activeSession_->getEnergyPrice()      << " kr\n";
    file << "TotalWaterCost: "  << activeSession_->getTotalWaterCost()   << " kr\n";
    file << "TotalEnergyCost: " << activeSession_->getTotalEnergyCost()  << " kr\n";
    file << "TotalCost: "       << activeSession_->getTotalWaterCost() + activeSession_->getTotalEnergyCost() << " kr\n";
    file << "WaterLimit: "      << WaterLimit_                           << " liter\n";
    file << "-------------------\n";
}

double MeasurementControl::getWaterLimit()    { return WaterLimit_; }
int    MeasurementControl::getCurrentUserId() { return CurrentUserId_; }
int    MeasurementControl::getReadingCount()  { return ReadingCount_; }
double MeasurementControl::getTotalVolume()   { return TotalVolume_; }
double MeasurementControl::getTempSum()       { return TempSum_; }
