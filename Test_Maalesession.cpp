#include <iostream>
#include <chrono>
#include <thread>
#include "Maalesession.h"



int main(){

     // test MaaleSession setters/getters
    MaaleSession session(1);
    session.setTotalWaterUsed(50.0);
    std::cout << "TotalWaterUsed (forventer 50): " << session.getTotalWaterUsed() << "\n";

    session.setAverageTemp(40.0);
    std::cout << "AverageTemp (forventer 40): " << session.getAverageTemp() << "\n";

    // test tidtagning
    session.startSession();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    session.endSession();
    std::cout << "ShowerLength (forventer ~2): " << session.getShowerLength() << " sekunder\n";

    return 0;
}
