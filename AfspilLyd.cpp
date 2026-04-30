#include "AfspilLyd.h" 

//constructoren opretter AfspilLyd og forbinder den med Sound. Den kaldes automatisk, når man skriver fx: AfspilLyd afspilLyd(sound, 5.0);
AfspilLyd::AfspilLyd(Sound& sound, double waterLimitLiters)
    : sound_(sound),                            //gemmer en reference til Sound objektet
      waterLimitLiters_(waterLimitLiters),      //gemme hvor mange liter der skal være mellem hver lyd
      nextThresholdLiters_(waterLimitLiters),   //første lydgræns er den valgte vandgrænse, fx 5 liter
      enabled_(true)                            //lyd feeedback er slået til som standard
{} 




//begin() starte rlydsystemet og nulstille grænsen. kan kaldes ved opstart, eller når en ny session atarter: afspilLyd.begin();
void AfspilLyd::begin() {
    sound_.begin();                 //kalder Sound::begin(), som starter SOMO-II kommunikationen
    reset();                        //nulstiller næste lydgrænse tul waterLimitLiters_
}




//reset() gør klar til en ny badesession. Den bør kaldes, når sessionen starter, så lyden igen starter ved fx 5 liter
void AfspilLyd::reset() {
    nextThresholdLiters_ = waterLimitLiters_;       //sætter næste lydgrænse til fx 5 liter
}



//setWaterLimit() ændrer hvor ofte lyden skal komm, kan kaldes mede fx: afspilLyd.setWaterLimit(10.0);
void AfspilLyd::setWaterLimit(double waterLimitLiters) {
    if (waterLimitLiters <= 0.0) {                  //grænsen må ikke væree 0 eller negativ
        return;                                     //stopper hvis værdien er ugyldig
    }

    waterLimitLiters_ = waterLimitLiters;           //gemmer den nye vandgrænse
    reset();                                        //nulstiller næste lydgrænse, så den passer til den nye grænse
}





//setEnabled() slår lydfeedback til eller fra. Kan kaldes med: afspilLyd.setEnabled(false); for at slukke lyden
void AfspilLyd::setEnabled(bool enabled) {
    enabled_ = enabled; // gemmer om lydfeedback skal være aktiv
}




//den kaldes fra MeasurementControl::beginMeasurement() hver gsng ny flowdata er målt
//eksempel: afspilLyd.checkAndPlay(TotalVolume_, CurrentFlow_ > 0.0);
//returnrer true, hvis den afspillede en lyd. Returnerer false, hvis den ikk gjorde
bool AfspilLyd::checkAndPlay(double totalVolumeLiters, bool waterFlowActive) {
    if (!enabled_ || !waterFlowActive || waterLimitLiters_ <= 0.0) {                //stop hvis slyd er slukket, der ikk er flow, eller grønsen er ugyldig
        return false;           //hvisn ingen lyd blev afspillt
    }

    if (totalVolumeLiters >= nextThresholdLiters_) {            //hvis samlet vandforbrug har nået næste grænse...
        sound_.setSoundFile(TRACK_INTERVAL);
        sound_.playSound();                                     //...så afspilles den valgte lyd via Sound-klassen

        //sørge for, at næste lyd kommer ved næste grænse, fx 5, 10... liter
        while (totalVolumeLiters >= nextThresholdLiters_) { //hvis målingen sprang over flere grænse...
            nextThresholdLiters_ += waterLimitLiters_;      //...flyttes næste grænse frem, indtil den lægger over totalVolumeLiters
        }

        return true;                                        //fortælle rresten af programmet at en lyd blev afspillelt
    }

    return false;                                           //hvis grænsen ikk er nået endnu, afspilles ingen lyd
}



//getNextThreshold() returnerer næste grænse for lyd. Kan bruges til debugging eller visning: double next = afspilLyd.getNextThreshold();
double AfspilLyd::getNextThreshold() const {
    return nextThresholdLiters_; //Returnerer fx 5.0, 10.0 eller 15.0
}
