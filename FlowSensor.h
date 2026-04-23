#pragma once

class FlowSensor {
public:

    double readFlow();
    void volumeUpdated(double deltaVolume);

};