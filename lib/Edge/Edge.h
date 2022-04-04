
#ifndef EDGE_H
#define EDGE_H

//=================================================================== .H FILE =================================================

//Klasa detekcije "ivice" ulaza
class Edge
{
  private:
    bool lastState_rTrig;    //Zapamceno stanje input-a rTriga-a
    bool lastState_fTrig;    //Zapamceno stanje input-a fTrig-a
    bool lastState_change;   //Zapamceno stanje input-a change-a
    bool change1time;         //Bool prvog ukljucenja (potreban za "change" edge)
    bool lastState_rTrigDebounce;  //Zapamceno stanje input-a debounce-a
    unsigned long lastTime_rTrigDebounce; //Memory of last debounce time
    bool lastState_fTrigDebounce;  //Zapamceno stanje input-a debounce-a
    unsigned long lastTime_fTrigDebounce; //Memory of last debounce time
  public:
    //Konstruktor
    Edge(void);
    //Na pozitivnu ivicu "input"-a vraca true
    bool rTrig(bool input);
    //Na negativnu ivicu "input"-a vraca true
    bool fTrig(bool input);
    //Na bilo koju promenu stanja "input"-a vraca true
    bool change(bool input);
    //Debounce on rising edge input
    bool rTrigDebounce(bool input, unsigned long debounce_time);
    //Debounce on falling edge input
    bool fTrigDebounce(bool input, unsigned long debounce_time);
};
//==============================================================================================================================
#endif
