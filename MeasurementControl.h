#pragma once
#include "Maalesession.h"
#include "Calculator.h" // calculator mangler at blive oprettet.

class MeasurementControl{
    public:
        MeasurementControl();
        void startSession();
        void selectedUser(int userId);
        void waterLimitSet(double limit); 
        void stopSession();
        void storeSession();

    private:
        double CurrentTemp_;// seneste måling fra tempsensor og arduino
        double CurrentFlow_;// seneste måling fra flowsensor og arduino
        double TotalVolume_;// akkumuleret vandvolume
        double TempSum_; // temp målinger lagt sammen under session
        int ReadingCount_; // antal målinger pr session
        int CurrentUserId_; // den valgte user
        double WaterLimit_; // det valgte threshold ift lyd trigger.

        MaaleSession* activeSession_; // pointer til aktuelle målesession
        Calculator calculator_; // calculator objekt, så vi kan tilgå calculator funktioner
};