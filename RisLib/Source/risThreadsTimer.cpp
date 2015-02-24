/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreads.h"
#include "prnPrint.h"

#include "risThreadsTimerThread.h"
#include "risThreadsTimer.h"

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// class definition for implementation specific timer thread

class TimerTimerThread : public Ris::Threads::BaseTimerThread
{
public:
   typedef Ris::Threads::BaseTimerThread BaseClass;

   TimerTimerThread(TimerCall aTimerCall,int aTimerPeriod,int aThreadPriority)
   {
      mFirstFlag=true;
      mTimerCall                 = aTimerCall;
      BaseClass::mTimerPeriod    = aTimerPeriod;
      BaseClass::mThreadPriority = aThreadPriority;
   }

   //--------------------------------------------------------------
   // Base class overloads:

   void executeOnTimer(int aTimerCount)
   {
      // Ignore first occurrence
      if (mFirstFlag)
      {
         mFirstFlag=false;
         return;
      }

      // Invoke owner timer call
      mTimerCall(aTimerCount);
   }

   //--------------------------------------------------------------
   // Members

   TimerCall  mTimerCall;
   bool       mFirstFlag;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Class definition for implementation specific

class ThreadTimer::Specific
{
public:
   Specific()
   {
      mTimerThread=0;
   }
   TimerTimerThread* mTimerThread;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

ThreadTimer::ThreadTimer()
{
   // Initialize members
   mTimerPeriod=1000;

   // Create new specific implementation
   mSpecific = new Specific;
}

ThreadTimer::~ThreadTimer()
{
   cancel();
   delete mSpecific;
}

//******************************************************************************

void ThreadTimer::startTimer(
   TimerCall aTimerCall,
   int       aTimerPeriod,
   int       aThreadPriority)
{
   mTimerCall      = aTimerCall;
   mTimerPeriod    = aTimerPeriod;
   mThreadPriority = aThreadPriority;

   // Create and launch timer thread  
   mSpecific->mTimerThread = new TimerTimerThread(mTimerCall,mTimerPeriod,mThreadPriority);
   mSpecific->mTimerThread->launchThread();
}

//******************************************************************************

void ThreadTimer::cancel()
{
   if (mSpecific->mTimerThread != 0)
   {
      mSpecific->mTimerThread->shutdownThread();
   }
}

}//namespace
}//namespace

