#pragma once

class MeasurementControl{
    public:
        void startSession();
        void selectedUser();
        void waterLimitSet(); //måske bare SetWaterLimit
        void stopSession();
        void storeSession();

    private:
        double CurrentTemp;
        double CurrentFlow;
        double totalVolume;
};