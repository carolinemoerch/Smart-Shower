#include "MeasurementControl.h"
#include "AfspilLyd.h"
#include "Sound.h"
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <iostream>

const std::string CMD_FILE     = "command.txt";
const std::string LIVE_FILE    = "live_data.txt";
const std::string SESSION_FILE = "session_data.txt";

int main() {
    Sound sound;
    AfspilLyd afspilLyd(sound, 10.0);
    MeasurementControl measurementControl;
    bool sessionActive = false;

    afspilLyd.begin();

    while (true) {

        // Læs kommando fra Flask
        std::string command = "";
        std::ifstream cmdFile(CMD_FILE);
        if (cmdFile.is_open()) {
            std::getline(cmdFile, command);
            if (!command.empty()) {
                std::cout << "Kommando laest: " << command << std::endl;
            }
            cmdFile.close();
            std::ofstream clear(CMD_FILE); // slet indhold så kommandoen ikke læses igen
        }

        // START:1:15 → start session for bruger 1 med interval 15
        if (command.rfind("START:", 0) == 0) {
            int colon       = command.find(":", 6);
            int userId      = std::stoi(command.substr(6, colon - 6));
            double interval = std::stod(command.substr(colon + 1));
            measurementControl.selectedUser(userId);
            afspilLyd.setWaterLimit(interval);
            measurementControl.startSession();
            sessionActive = true;
        }

        // STOP → stop session
        if (command == "STOP") {
            measurementControl.stopSession();
            sessionActive = false;
        }

        // Mål løbende mens session kører
        if (sessionActive) {
            measurementControl.beginMeasurement();
            afspilLyd.checkAndPlay(measurementControl.getTotalVolume(), measurementControl.getTotalVolume() > 0);
        }

        std::this_thread::sleep_for(std::chrono::seconds(1)); // vent 1 sekund
    }

    return 0;
}