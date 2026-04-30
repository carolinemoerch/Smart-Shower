#pragma once

class FlowSensor {
public:
    void init_flow_sensor();
    double readFlow();
    void volumeUpdated(double deltaVolume);

};