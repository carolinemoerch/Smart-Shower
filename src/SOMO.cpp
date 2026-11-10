//klassen styrer SOMO-II lydmodulet via UART1 på en AVR-microcontroller,

#ifndef F_CPU
#define F_CPU 16000000UL //CPU-frekvensen sættes til 16 MHz, da det arduino der bruger dette
#endif

#include "SOMO.h"    

#if defined(__AVR__)
#include <avr/io.h>      //AVR-registere som UBRR1H, UBRR1L, UCSR1A, UCSR1B, UCSR1C og UDR1.
#include <util/delay.h> 
#endif



Somo::Somo(unsigned long baudrate) : baudrate_(baudrate)
{
    //constructoren gemmer kun baudraten i objektets private variabel baudrate_
    //UART startes først i begin()
}




void Somo::uartInit()
{
#if defined(__AVR__)
    //UBRR er baudrate-værdien, som AVR UART-hardwaren bruger
    //formle nkommer fra AVR UART: UBRR = F_CPU / (16 * baudrate) -1
    unsigned int ubrr = (F_CPU / (16UL * baudrate_)) - 1;       

    UBRR1H = static_cast<uint8_t>(ubrr >> 8);   //gemmer de øverste 8 bits af UBRR-værdien i high registeret
    UBRR1L = static_cast<uint8_t>(ubrr);        //gemmer de nederste 8 bits af UBRR-værdien i lowregistret

    UCSR1B = (1 << TXEN1);                      //TXEN1 aktiverer UART1 transmitter, så microcontrolleren kan sende data til SOMO-II

    UCSR1C = (1 << UCSZ11) | (1 << UCSZ10);     //UCSZ11 og UCSZ10 giver 8 databits, ingen paritet og 1 stopbit som standard.
#endif
}




void Somo::uartSend(uint8_t byte)
{
#if defined(__AVR__)
    //UDRE1 betyder UART Data Registe empty

    while (!(UCSR1A & (1 << UDRE1)))
    {
                        //tom løkke med vilje. Programmet svente her ellers kan den næste kode overskrive den forrig
    }

    UDR1 = byte;        //når UDR1 får en byte, sender UART-hardwaren automatisk byten ud på TX1-pinnen
#endif
}




void Somo::sendCommand(uint8_t cmd, uint8_t param1, uint8_t param2)
{
    //SOMO kommandoformatet er 8 byte: START, CMD, FEEDBACK, PARAM1, PARAM2, CHECKSUM_HIGH, CHECKSUM_LOW, END

    uint16_t sum = static_cast<uint16_t>(cmd) + FEEDBACK_OFF + param1 + param2;
    //sum er summen af de fire bytes, som indgår i checksum beregningne


    uint16_t checksum = 0xFFFF - sum + 1;   //ligger kommando dele sammen checksum formeln følger SOMO datablade er : checksum = 0xFFFF - (CMD + Feedback + Param1 + Param2) + 1.

    uartSend(START_BYTE);                             //byte 1: start på kommandoen, altid 0x7E
    uartSend(cmd);                                    //byte 2: selve kommandoen, fx 0x0F for mappe+ track
    uartSend(FEEDBACK_OFF);                           //byte 3: feedback slået fra
    uartSend(param1);                                 //byte 4: parameter 1, fx mappe nummer
    uartSend(param2);                                 //byte 5: parameter 2, fx track nummer
    uartSend(static_cast<uint8_t>(checksum >> 8));     //byte 6: checksum high byte
    uartSend(static_cast<uint8_t>(checksum & 0xFF));   //byte 7: checksum low byte
    uartSend(END_BYTE);                               //byte 8: slut på kommandoe n altid 0xEF

#if defined(__AVR__)
    _delay_ms(20);          
#endif
}



void Somo::begin()
{
    uartInit();                     //innitialiseereer UART1 med objektet baudrate 9600

#if defined(__AVR__)
    _delay_ms(500);                 // SOMO får tid starte op, før der  kommandoer
#endif

    setVolume(VOLUME_DEFAULT);      //sætte lydstyrken til 25 som defaultv ed opstart

#if defined(__AVR__)
    _delay_ms(100);                 
#endif

}


void Somo::playFolderTrack(uint8_t folderNumber, uint8_t trackNumber)
{
    //dene funktion bruge kommando 0x0F som er "specify folder &t rack"
    //eksempel: folderNumber = 0x01 og trackNumber = 0x02 spiller fil 002xxx.mp3 i mappen 01
    sendCommand(CMD_PLAY_FOLDER_TRACK, folderNumber, trackNumber);
}




void Somo::setVolume(uint8_t volume)
{
    if (volume > VOLUME_MAX)
    {
        volume = VOLUME_MAX;                    //ebskytter mod ugyldige værdier over 30
    }

    if (volume < VOLUME_MIN)
    {
        volume = VOLUME_MIN;                    //beskytter mod ugyldige værdier under 0
    }

    sendCommand(CMD_SET_VOLUME, 0x00, volume);  //sender volume-kommandoen til SOMO
}



void Somo::stop()
{
    sendCommand(CMD_STOP, 0x00, 0x00);      //stop kræver ingen parametre, derfor 0x00 og 0x00
}



