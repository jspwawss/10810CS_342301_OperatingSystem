#include "timer.h"
#include "main.h"
#include "sysdep.h"

Timer::Timer(bool doRandom, CallBackObj *toCall)
{
    randomize = doRandom;
    callPeriodically = toCall;
    disable = FALSE;
    SetInterrupt();
}

//----------------------------------------------------------------------
// Timer::CallBack
//      Routine called when interrupt is generated by the hardware
//	timer device.  Schedule the next interrupt, and invoke the
//	interrupt handler.
//----------------------------------------------------------------------
void
Timer::CallBack()
{
    // invoke the Nachos interrupt handler for this device
    callPeriodically->CallBack();

    SetInterrupt();	// do last, to let software interrupt handler
    			// decide if it wants to disable future interrupts
}

//----------------------------------------------------------------------
// Timer::SetInterrupt
//      Cause a timer interrupt to occur in the future, unless
//	future interrupts have been disabled.  The delay is either
//	fixed or random.
//----------------------------------------------------------------------

void
Timer::SetInterrupt()
{
    if (!disable) {
       int delay = TimerTicks;

       if (randomize) {
	     delay = 1 + (RandomNumber() % (TimerTicks * 2));
        }
       // schedule the next timer device interrupt
       kernel->interrupt->Schedule(this, delay, TimerInt);
    }
}
