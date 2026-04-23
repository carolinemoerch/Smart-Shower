#pragma once
#include <string>

class Display{
public:
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

};

