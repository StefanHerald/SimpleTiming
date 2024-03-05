/*
Very basic timing class using the List structure from Luis at luisllamasbinaburo/Arduino-List.
Created by Stefan Herald, sdn: 0960543, Feb 24, 2024.
Released into the public domain
*/
#ifndef _Timing_h
#define _Timing_h

#include <ListLib.h> //from luisllamasbinaburo/Arduino-List
#include <Arduino.h>

struct Timer{
  unsigned long initTime;
  unsigned long delay;
  void (*onDelay)();
  void (*onRemove)();
  byte repeat;
  byte ID;
}; 

class Timing
{
  public:
          Timing();
          byte addTimer(unsigned int delay, byte repeat, void (*onDelay)(), void (*onRemove)());
          void removeTimer(byte ID);
          void removeTimerWithoutFunc(byte ID);
          void removeAll();
          void removeAllWithoutFunc();
          void tick();
          void tick(unsigned int time);
  
  private:
          List<Timer> _timers;
          byte _id; 
          void _checkAll(unsigned long current);
          void _handle(byte index, unsigned long current);
};

#endif