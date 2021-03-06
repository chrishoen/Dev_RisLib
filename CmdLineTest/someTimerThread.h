#ifndef _SOMETIMERTHREAD_H_
#define _SOMETIMERTHREAD_H_

/*==============================================================================
Timer thead example
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risThreadsTimerThread.h"
#include "risTimeMarker.h"

namespace Some
{
//******************************************************************************
//******************************************************************************
//******************************************************************************

class TimerThread : public Ris::Threads::BaseTimerThread
{
public:
   typedef Ris::Threads::BaseTimerThread BaseClass;

   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   // Members.

   Ris::PeriodicTimeMarker mTimeMarker;

   bool mTPFlag;

   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   // Methods.

   // Constructor.
   TimerThread();

   // Base class overloads.
   void threadInitFunction() override;
   void executeOnTimer(int aTimeCount) override;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global instance

#ifdef _SOMETIMERTHREAD_CPP_
          TimerThread* gTimerThread;
#else
   extern TimerThread* gTimerThread;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace


#endif

