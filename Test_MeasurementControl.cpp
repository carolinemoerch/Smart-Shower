#include <iostream>
#include "MeasurementControl.h"


int main(){
        MeasurementControl mc;
    // test selectedUser
   mc.selectedUser(1);
    std::cout << "selectedUser(1) (forventer 1): " << mc.getCurrentUserId() << "\n";

    // test waterLimitSet normale værdier
    mc.waterLimitSet(50.0);
    std::cout << "waterLimitSet(50) (forventer 50): " << mc.getWaterLimit() << "\n";

    // test waterLimitSet over grænse
    mc.waterLimitSet(150.0);
    std::cout << "waterLimitSet(150) (forventer 100): " << mc.getWaterLimit() << "\n";

    // test waterLimitSet under grænse
    mc.waterLimitSet(-10.0);
    std::cout << "waterLimitSet(-10) (forventer 0): " << mc.getWaterLimit() << "\n";

    // test incLimit
    mc.waterLimitSet(50.0);
    mc.incLimit();
    std::cout << "incLimit (forventer 51): " << mc.getWaterLimit() << "\n";

    // test decLimit
    mc.decLimit();
    std::cout << "decLimit (forventer 50): " << mc.getWaterLimit() << "\n";

    // test decLimit ved grænse
    mc.waterLimitSet(0.0);
    mc.decLimit();
    std::cout << "decLimit ved 0 (forventer 0): " << mc.getWaterLimit() << "\n";

    // test incLimit ved grænse
    mc.waterLimitSet(100.0);
    mc.incLimit();
    std::cout << "incLimit ved 100 (forventer 100): " << mc.getWaterLimit() << "\n";

    // test beginMeasurement - frisk instans så testen er ren
    MeasurementControl mc2;
    mc2.selectedUser(2); // anden bruger så vi er sikre
    mc2.startSession();
    
    int i = 0;
    while (i < 5) {
        mc2.beginMeasurement();
        i++;
    }
    std::cout << "ReadingCount (forventer 5): " << mc2.getReadingCount() << "\n";
    std::cout << "TotalVolume (forventer 210 uden hardware, fordi retur value paa sensor sat til: 42): " << mc2.getTotalVolume() << "\n";
    std::cout << "TempSum (forventer 125 uden hardware, fordi retur value paa sensor sat til: 25 ): " << mc2.getTempSum() << "\n";

    return 0;
}