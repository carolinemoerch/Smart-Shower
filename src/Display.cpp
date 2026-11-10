#include "Display.h"

Display::Display(MeasurementControl& M):Mcontrol_(M){}

void onStartButtonClicked();{
    Mcontrol_.startSession();
}

void onStopButtonClicked();{
    Mcontrol_.stopSession();
}

void onSelectedUserButtonClicked(int UserId){
    Mcontrol_.selectedUser();
}

void Display::showScreen() {
    // Code to display the main screen
}
void Display::showStatus() {
    // Code to display the current status with volume and temperature
}
void Display::showError(std::string errorMessage) {
    // Code to display an error message
}
void Display::pressStopSession() {
    // Code to handle the stop session button press
}
void Display::showSessionSummary() {
    // Code to display the session summary
}
void Display::sleepScreen() {
    // Code to put the screen to sleep
}
void Display::showMenu() {
    // Code to display the menu
}
void Display::showConsumption(std::string userID) {
    // Code to display the consumption for a specific user
}
void Display::showLeaderboard() {
    // Code to display the leaderboard
}
void Display::showEstimatedPrice() {
    // Code to display the estimated price
}
void Display::showFlowError() {
    // Code to display a flow error message
}
void Display::showSoundError() {
    // Code to display a sound error message
}