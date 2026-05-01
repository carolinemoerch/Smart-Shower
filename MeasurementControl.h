#pragma once
#include "Maalesession.h"
#include "Calculator.h" // calculator mangler at blive oprettet.
#include "TemperatureSensor.h"
#include "FlowSensor.h"
#include "User.h"
#include "Database.h"
#include <array>


class MeasurementControl{
    public:
        MeasurementControl();
        void startSession();
        void beginMeasurement();
        void selectedUser(int userId);
        void waterLimitSet(double limit); 
        void incLimit();
        void decLimit();
        void stopSession();
        void storeSession();
        double getWaterLimit();
        int getCurrentUserId();

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
        TemperatureSensor tempSensor_; 
        FlowSensor flowSensor_;
        User User_;
        Database Database_; 

        std::array<User, 4> Users_{ // et array af User
        User(1, "User 1"),
        User(2, "User 2"),
        User(3, "User 3"),
        User(4, "User 4")
}; 
};