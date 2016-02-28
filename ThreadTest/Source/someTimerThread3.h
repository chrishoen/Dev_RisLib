#ifndef _SOMETIMERTHREAD3_H_
#define _SOMETIMERTHREAD3_H_

/*==============================================================================

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risThreadsThreads.h"
#include "risThreadsTimerEx.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

class TimerThread3 : public Ris::Threads::BaseThread
{
public:

   typedef Ris::Threads::BaseThread BaseClass;

   TimerThread3(); 

   //Base class overloads.
   void threadInitFunction();
   void threadRunFunction();
   void threadExitFunction();
   void shutdownThread();

   // Thread Members
   Ris::Threads::BinarySemaphore mSemaphore;
   bool  mTerminateFlag;

   // Timer Members
   Ris::Threads::ThreadTimerEx  mThreadTimer;
   Ris::Threads::TimerCall      mThreadTimerCall;
   int   mTimerPeriod;
   int   mTimerCount;

   //An overload of this is supplied by the inheritor.
   //It is called periodically by the threadRunFunction

   void executeOnTimer(int aTimerCount);

   
};

//******************************************************************************
// Global instance

#ifdef _SOMETIMERTHREAD3_CPP_
          TimerThread3* gTimerThread3;
#else
   extern TimerThread3* gTimerThread3;
#endif

//******************************************************************************
}//namespace

#endif

