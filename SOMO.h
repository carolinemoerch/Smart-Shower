#pragma once



#include <stdint.h> 

class Somo
{
private:

    static const uint8_t START_BYTE = 0x7E;                     //start-byte for alle SOMO-II kommandoer i følge databladet.
    static const uint8_t END_BYTE   = 0xEF;                     //slut-byte for all SOMO-II kommandoer i følge bladet.

    static const uint8_t FEEDBACK_OFF = 0x00;                   //0 betyder, at SOMO-II ikk skal sende bekræftelse tilbagee

    static const uint8_t CMD_PLAY_TRACK_SECONDARY = 0x03;       //afspiller track efter kopieringsrækkefælge ikke filnavn
    static const uint8_t CMD_SET_VOLUME           = 0x06;       //sætter lydstyrken mellem 0 og 30
    static const uint8_t CMD_PLAY                 = 0x0D;       //starter eller fortsætter afspilning
    static const uint8_t CMD_PAUSE                = 0x0E;       //pauser afspilning
    static const uint8_t CMD_PLAY_FOLDER_TRACK    = 0x0F;       //afspiller mappe + track efter navngivning, fx mappe 01 og fil 001xxx.mp3
    static const uint8_t CMD_STOP                 = 0x16;       //stopper afspilning

    static const uint8_t VOLUME_MAX = 30;        //maksimal lydstyrke på SOMO-II
    static const uint8_t VOLUME_MIN = 0;
    static const uint8_t VOLUME_DEFAULT = 25;

    unsigned long baudrate_;                     //onjektets baudrate. SOMO-II bruger normalt 9600 baud

    void uartInit();                //initialiserer UART1, kalde saf begin().
    void uartSend(uint8_t byte);    //UART1 sender en byte til SOMO-II via UART 1,  kaldes af sendCommand()
    void sendCommand(uint8_t cmd, uint8_t param1, uint8_t param2);  //sender hel SOMO-II kommando.

public:
    Somo(unsigned long baudrate = 9600UL);         //constructorr, opretter objektet med 9600 baud som standard

    void begin();                                           //ksal kaldes en gang i starten af programmet for at initialisere UART og SOMO-II, Kaldes gennem Sound::begin().
    void playFolderTrack(uint8_t folderNumber, uint8_t trackNumber);    //spiller efter mappe-/filnavn, fx 01/001xxx.mp3
    void setVolume(uint8_t volume);                                     //sætter lydstyrke fra 0 til 30
    void stop();                                                        //stopper lyd
    void pause();                                                       //pauser lyd
    void play();                                                        //starter eller fortsætter afspilning
};

//track-konstanter.
//001_START_lyd.mp3
//002_INTERVAL_lyd.mp3
//003_SLUT_lyd.mp3
static const uint8_t SOMO_FOLDER_01 = 0x01;     //,appe 01 SOMO-II den bruger hex-værdi 0x01
static const uint8_t TRACK_START    = 0x01;     //track 001
static const uint8_t TRACK_INTERVAL  = 0x02;    //track 002
static const uint8_t TRACK_SLUT     = 0x03;     //track 003

