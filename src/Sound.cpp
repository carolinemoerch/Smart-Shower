#include "Sound.h"
//onstructoren kører automatisk, når et Sound-objekt oprettes.
//Den skal normalt ikke kaldes direkte som funktion. Man bruger fx: Sound sound;
Sound::Sound(uint8_t trackNumber, uint8_t volume)
    : somo_(),                         //Opretter Somo-objektet, som styrer SOMO-II.
      trackNumber_(trackNumber),       // track der blev valgt ved oprettelsen.
      volume_(volume),                 //lydstyrken der blev valgt ved oprettelsen.
      initialized_(false)              //Starter som false, fordi SOMO-II endnu ikke er initialiseret.
{}

//begin() starter kommunikationen med SOMO-II og sætter lydstyrken.
//Kan kaldes manuelt i setup/startSession med: sound.begin();
//Hvis man ikke kalder den selv, kalder playSound() den automatisk første gang.
void Sound::begin() {
    somo_.begin();              //Starter Somo lklassen,Serial/UART-forbindelsen til SOMO-II.
    somo_.setVolume(volume_);   //Sender kommando til SOMO-II om at sætte lydstyrken.
    initialized_ = true;        //Markerer, at Sound nu er klar til at afspille lyd.
}

//playSound() afspiller den valgte mp3øfil.
//bruges af AfspilLyd::checkAndPlay(),når vandgrænsen nås. kano gså kaldes direkte med: sound.playSound();
bool Sound::playSound() {
    if (!initialized_) {            //vis begin() ikke er kaldt endnu...
        begin();                    //...så initialiseres SOMO-II automatisk.
    }

    //Afspiller fx mappe 01 / fil 002_xxx.mp3 på micro-SD-kortet.
    somo_.playFolderTrack(folderNumber_, trackNumber_);         //sender UART-kommando: afspil valgt folder og track.
    return true;                                                //returnere true for  fortælle, at afspilningskommandoen sendt.
}

//stopSound() stopper den lyd, der i gang.
//kan kalde ved stop af badsession  med: sound.stopSound();
void Sound::stopSound() {
    if (!initialized_) {            //Hvis SOMO-II ikke er startet endnu...
        begin();                    //...startes det, så stop-kommandoen kan sendes korrekt.
    }

    somo_.stop();                   //Sender stop-kommando til SOMO-II.
}

//setVolume() øndrer lydstyrk.
//kan kaldes direkte med fx: sound.setVolume(25);
void Sound::setVolume(uint8_t volume) {
    if (volume > 30) {              //SOMO-II bruger en volume-skala op til 30., høj værdi, begrænses den til 30.
        volume = 30;                
    }

    volume_ = volume;               //gemmer den nye lydstyrke i objektet

    if (initialized_) {                 //Hvis SOMO-II allerede er startet...
        somo_.setVolume(volume_);       //...sendes den nye lydstyrke med det samme til modulet.
    }
}

//setSoundFile() vælger hvilken mp2-fil der skal afspilles næste gang.
// kaldes med fx: sound.setSoundFile(1, 2); for folder 01, track 002.
void Sound::setSoundFile(uint8_t trackNumber) 
{
    trackNumber_ = trackNumber;         //Gemmer nyt track.
}
