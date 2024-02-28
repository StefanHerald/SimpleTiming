/*
Timing is as simple as waiting for a time to elapse and then doing the function.
In order for outputs to the main loop (or even other classes) to be possible, we will need to pass pointers, using *p = &a. 

Seems like to me, a timer is nothing more than a value of the delay and a function to be called when elapsedTime >= delay.
However, we may not want the delay to be exactly in millis, and instead use seconds or even minutes. For this, a converter needs to be given too.

We can store all the timers in an array, then iterate over it. luisllamasbinaburo/Arduino-List seems to be promising
We can also try to use the timer interrupts. Problem is, we have 3 of those and 1 of them is the clock for millis.

Probably also want a function to be run when the timer is removed: for example, when a LED timer is removed, we might also want to add a func that turns it off as well.
*/
#include <Timing.h>

Timing::Timing()
{
  List<Timer> timers;
 _timers = timers;
}

byte Timing::addTimer(unsigned int delay, byte repeat, void (*f)())
{
  byte id = _id;
  unsigned long current = millis();
  struct Timer newTimer = {delay, current, f, repeat, id};
  _timers.Add(newTimer);
  _id++;
  return id;
}

void Timing::removeTimer(byte ID)
{
  for(byte i = 0; i < _timers.Count(); i++)
  {
    if(_timers[i].ID == ID) 
    {  
      _timers.Remove(i);
      _timers.Trim(2); //might be better, but still need to test
      return;
    }
  }
}

void Timing::removeAll()
{
  _timers.Clear();
  _timers.Trim(4); //maybe just a new list?
  _id = 0;
}

void Timing::_handle(byte index, unsigned long current)
{
  void (*func)() = _timers[index].f;
  (*func)();
  _timers[index].initTime = current;
  if(_timers[index].repeat != 255 || _timers[index].repeat != 0)
  {
    --_timers[index].repeat;
  }
}

void Timing::_checkAll(unsigned long current)
{
  byte itemCount = _timers.Count();

  for(byte i = 0; i < itemCount; i++)
  {
    if(current - _timers[i].initTime >= _timers[i].delay) {_handle(i, current); itemCount = _timers.Count();}
  }

  for(byte i = 0; i < itemCount; i++)
  {
    if(_timers[i].repeat == 0)
    {
       _timers.Remove(i);
       --i;
       --itemCount;
    }
  }
}

void Timing::tick()
{
  unsigned long current = millis();
  _checkAll(current);
}

void Timing::tick(unsigned int time)
{
  unsigned long current = millis();
  _checkAll(current + (unsigned long)time);
}

