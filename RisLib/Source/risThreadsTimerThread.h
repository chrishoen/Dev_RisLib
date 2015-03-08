#ifndef _RISTHREADSTIMERTHREAD_H_
#define _RISTHREADSTIMERTHREAD_H_

/*==============================================================================

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risThreads.h"

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is an extension of the virtual base class for general purpose threads.
// It provides a basis for a periodic thread that uses a polling termination
// policy.

class  BaseTimerThread : public BaseThread
{
public:

   typedef Ris::Threads::BaseThread BaseClass;

   BaseTimerThread(); 
  ~BaseTimerThread() {}; 

   //Base class overloads.
   //Thread function run.
   //This is an infinite loop that sleeps for the timer period
   //and periodically calls the inheritor executeOnTimer.

   void threadRunFunction  ();
   void shutdownThread();
   

   //An overload of this is supplied by the inheritor.
   //It is called periodically by the threadRunFunction

   virtual void executeOnTimer(int aTimerCount)=0;

   // Members
   Ris::Threads::BinarySemaphore mSemaphore;
   int   mTimerPeriod;
   int   mTimerCount;
   bool  mTerminateFlag;
   
};


//******************************************************************************


}//namespace
}//namespace

#endif

