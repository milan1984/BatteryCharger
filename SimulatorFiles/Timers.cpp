#include "Timers.h"
#include "Arduino.h"
//================================================= .CPP FILE =========================================================

//Konstruktor
Timers::Timers()
    : pastTimeOn_Off(0), pastTimeOn_Off_by_Timers(0), pastTimeOn(0), pastTimeOff(0), pastTimeImpuls(0), outputOn_Off(false), outputOn_Off_by_Timers(false), outputOn(false), outputOff(false),
      outputImpuls(false)

{
}

//On - off tajmer.Posle svakog isteka "interval" vremena, vraca drugaciju vrednost bool-a
bool Timers ::on_off_Timer(bool input, unsigned long interval)
{
    unsigned long timeNow = millis(); //Trenutno vreme

    if (input)
    {
        if (timeNow - pastTimeOn_Off >= interval)
        {
            pastTimeOn_Off = timeNow;
            outputOn_Off = !outputOn_Off; //Posle isteka "interval" vremena, menjamo stanje bool-a koje vraca funkcija
        }
    }
    else
    {
        pastTimeOn_Off = timeNow;
        outputOn_Off = false;
    }
    return outputOn_Off;
}
//On - off tajmer sa biranjem vremena trajanja on, kao i vremena trajanja off
bool Timers::on_off_by_Timers(bool input, unsigned long interval_on, unsigned long interval_off)
{
    unsigned long timeNow = millis(); //Trenutno vreme

    unsigned long time_to_watch = interval_on;

    if (input)
    {
        if (outputOn_Off_by_Timers)
        {
            time_to_watch = interval_on;
        }
        else
        {
            time_to_watch = interval_off;
        }

        if (timeNow - pastTimeOn_Off_by_Timers >= time_to_watch)
        {
            pastTimeOn_Off_by_Timers = timeNow;
            outputOn_Off_by_Timers = !outputOn_Off_by_Timers; //Posle isteka "interval" vremena, menjamo stanje bool-a koje vraca funkcija
        }
    }
    else
    {
        pastTimeOn_Off_by_Timers = timeNow;
        outputOn_Off_by_Timers = false;
    }
    return outputOn_Off_by_Timers;
}

//Tajmer zadrske ukljucenja.
bool Timers ::delayOn(bool input, unsigned long interval)
{
    unsigned long timeNow = millis(); //Trenutno vreme

    if (input)
    {
        if (timeNow - pastTimeOn >= interval)
        {
            outputOn = true;
        }
    }
    else
    {
        pastTimeOn = timeNow;
        outputOn = false;
    }
    return outputOn;
}

//Tajmer zadrske iskljucenja
bool Timers ::delayOff(bool input, unsigned long interval)
{
    unsigned long timeNow = millis(); //Trenutno vreme

    if (!input)
    {
        if (timeNow - pastTimeOff >= interval) // Kada istekne vreme, bool sto funkcija vraca je false
        {
            outputOff = false;
        }
    }
    else
    {
        pastTimeOff = timeNow; //Reset vremena
        outputOff = true;      // Bool sto funkcija vraca je true
    }
    return outputOff; //Vracamo vrednost funkcije
}

//Tajmer koji daje jedan puls svakih interval vremena
bool Timers::impulsTimer(bool input, unsigned long interval)
{

    unsigned long timeNow = millis(); //Trenutno vreme

    if (input)
    {
        if (timeNow - pastTimeImpuls >= interval)
        {
            outputImpuls = true;
            pastTimeImpuls = timeNow;
        }
        else
        {
            outputImpuls = false;
        }
    }
    else
    {
        outputImpuls = false;
        pastTimeImpuls = timeNow;
    }

    return outputImpuls;
}
//====================================================================================================================
