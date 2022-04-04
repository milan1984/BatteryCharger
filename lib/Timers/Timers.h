#ifndef TIMERS_H
#define TIMERS_H

//==============================================  .H FILE =============================================================
class Timers
{
private:
    unsigned long pastTimeOn_Off;
    unsigned long pastTimeOn_Off_by_Timers;
    unsigned long pastTimeOn;
    unsigned long pastTimeOff;
    unsigned long pastTimeImpuls;

    bool outputOn_Off;
    bool outputOn_Off_by_Timers;
    bool outputOn;
    bool outputOff;
    bool outputImpuls;

public:
    Timers();
    bool on_off_Timer(bool, unsigned long);                    // On /OFF tajmer
    bool on_off_by_Timers(bool, unsigned long, unsigned long); // On /OFF tajmer po razlicitim vremenima trajanja ukljucenja - iskljucenja
    bool delayOn(bool, unsigned long);                         //Zadrska ukljucenja
    bool delayOff(bool, unsigned long);                        //Zadrska iskljucenja
    bool impulsTimer(bool, unsigned long);                     //Tajmer impulsa
};
//=====================================================================================================================
#endif
