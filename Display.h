#pragma once
#include "MeasurementControl.h"
#include <string>

class Display{
public:
    Display(MeasurementControl& M);
    void onStartButtonClicked();
    void onStopButtonClicked();
    void onSelectedUserButtonClicked(int UserId);
    void showScreen(); // datatype?
    void showStatus();
    void showError(std::string errorMessage);
    void pressStopSession();
    void showSessionSummary(); // skal den tage imod parametre? sessionData?
    void sleepScreen(); // måske en could?
    void showMenu(); // 
    void showConsumption(std::string userID);
    void showLeaderboard(); 
    void showEstimatedPrice();
    void showFlowError();
    void showSoundError();

private:
    MeasurementControl& Mcontrol_;

};

