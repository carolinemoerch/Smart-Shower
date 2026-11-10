#pragma once 
#include "SOMO.h"       
#include <stdint.h>     



//sound-klassen er et lag ovenpå SOMO-klassen. Den bruges til at vælge lydfil, sætte lydstyrke og starte/stoppe afspilning.
class Sound {
public:
    //Constructor: opretter et Sound-objekt med standard mappe, track og volume
    Sound(uint8_t trackNumber = TRACK_INTERVAL,      //standardlydfil/track, fx 002_Interval.mp3
          uint8_t volume = 25);                     //default lydstyrke. SOMO-II bruger 0-30 som lyd syrke interval, jeg har brugt 25 fordi jeg synes det et flot tal

    void begin();                       //initialisere SOMO, skal kaldes før brug, men jeg har lakvet playSound() kalder den selv, hvis man glemt det
    bool playSound();                   //afspiller den valgte lydfil. Kan kaldes direkte med: sound.playSound();
    void stopSound();                   //stopper den lyd, der afspilles, kan kaldes direkte med: sound.stopSound();
    void setVolume(uint8_t volume);     //øndrer lydstyrken. kan kaldes med fx: sound.setVolume(20);
    void setSoundFile(uint8_t trackNumber);   //vælger en anden mp3-fil. Fx: sound.setSoundFile(1, 3); , eller sound.setSoundFile(SOMO_FOLDER_01, TRACK_INTERVAL);

private:                    
    Somo somo_;                                         // Objektet der kommunikerer med SOMO-II via Serial/UART.
    uint8_t folderNumber_= SOMO_FOLDER_01;              //mappen på SD-kortet lyd-filerne ligger i
    uint8_t trackNumber_;                               //hvilke ttrack/mo3-fil der skal afpilles.
    uint8_t volume_;                                    //ønsket lydstyrke.
    bool initialized_;                                  // hsker om begin() allerede er kaldt, så den ikke initialiseres ude grund.
};
