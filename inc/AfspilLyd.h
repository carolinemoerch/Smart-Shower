#pragma once 

#include "Sound.h" //AfspilLyd bruger Sound-klassen til at afspille lyden

//afspilLyd klassen styrer logikken for, HVORNÅR lyden skal afspilles
//den holder øjemed vandfobrug og afspiler lyd vedfx 5, 10, 15 liter
class AfspilLyd {
public:
    
    AfspilLyd(Sound& sound, double waterLimitLiters = 5.0);         //Constructor: kobler AfspilLyd sammen med et Sound-objekt og en vandgrænse. Kaldes når objektet oprettes, fx: AfspilLyd afspilLyd(sound, 5.0);

    void begin();                                                   //starter lydsystemet og nulstiller næste grænse. kaldes ved systemstart eller sessionstart
    void reset();                                                   //nulstiler næste lydgrønse, burde kaldess ved start af ny badesesion
    void setWaterLimit(double waterLimitLiters);                    //Ændrer vandgrænsen, fx fra 5 liter til 10 litre
    void setEnabled(bool enabled);                                  //Slår lydfeedvack til eller fra

    bool checkAndPlay(double totalVolumeLiters, bool waterFlowActive);      //tjekker vandforbrug og afspiller lyd, hvis grænsen er nået
    double getNextThreshold() const;                                        //returnerer næste liter grænse, hvor der skal afspilles lyd

private: 
    Sound& sound_;                  //reference til Sound-objektet, så AfspilLyd kan kalde sound_.playSound()
    double waterLimitLiters_;       //vor mange liter der skal gå mellem hver lyd, fx 5.0
    double nextThresholdLiters_;    //Dden næste litergrænse, fx 5.0, 10.0, 15.0 osv
    bool enabled_;                  //vestemmer om lydfeedback er slået til
};
