//================================================================ .CPP FILE ===================================================
#include "Edge.h"
#include "Arduino.h"
//Konstruktor i inicijalizacija
Edge::Edge()
    : lastState_rTrig(false), lastState_fTrig(false), lastState_change(false), change1time(false), lastState_rTrigDebounce (false),lastTime_rTrigDebounce(false),lastState_fTrigDebounce (false),lastTime_fTrigDebounce(false)
{
}

//Detekcija pozitivne ivice "input-a"
bool Edge ::rTrig(bool input)
{
    bool output = false;

    if (input == true && lastState_rTrig == false)
    {
        output = true;
    }
    else
    {
        output = false;
    }
    lastState_rTrig = input;
    return output;
}

//Detekcija negativne ivice "input"-a
bool Edge ::fTrig(bool input)
{
    bool output = false;

    if (input == false && lastState_fTrig == true)
    {
        output = true;
    }
    else
    {
        output = false;
    }
    lastState_fTrig = input;
    return output;
}

//Detekcija bilo koje promene stanja "input"-a
bool Edge ::change(bool input)
{
    bool output = false;

    if (change1time)
    {
        if (input != lastState_change)

        {
            output = true;
        }
        else
        {
            output = false;
        }
    }
    else
    {
        change1time = true;
    }
    lastState_change = input;
    return output;
}
// Edge detection on RISING trig with debounce time
bool Edge :: rTrigDebounce (bool input, unsigned long debounce_time)
{
  bool timer_output = false;
  bool output = false;
  unsigned long timeNow = millis(); //Trenutno vreme
 
   if (input)
    {
        if (timeNow - lastTime_rTrigDebounce >= debounce_time)
        {
            timer_output = true;
        }
    }
    else
    {
        lastTime_rTrigDebounce = timeNow;
    }

    if (timer_output == true && lastState_rTrigDebounce == false)
    {
        output = true;
    }
    else
    {
        output = false;
    }
    lastState_rTrigDebounce = timer_output;
    return output;
}
// Edge detection on FALLING trig with debounce time
bool Edge :: fTrigDebounce (bool input, unsigned long debounce_time)
{
  bool timer_output = false;
  bool output = false;
  unsigned long timeNow = millis(); //Trenutno vreme
 
   if (input)
    {
        if (timeNow - lastTime_fTrigDebounce >= debounce_time)
        {
            timer_output = true;
        }
    }
    else
    {
        lastTime_fTrigDebounce = timeNow;
    }

    if (timer_output == false && lastState_fTrigDebounce == true)
    {
        output = true;
    }
    else
    {
        output = false;
    }
    lastState_fTrigDebounce = timer_output;
    return output;
}
//================================================================================================================================
