#ifndef _SOMETIMERTHREAD2_H_
#define _SOMETIMERTHREAD2_H_

/*==============================================================================
File: someThread2.h
Description:
QCall test thread class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risThreadsTimerThread.h"

namespace Some
{
//******************************************************************************
//******************************************************************************
//******************************************************************************

class TimerThread2 : public Ris::Threads::BaseTimerThread
{
public:
   typedef Ris::Threads::BaseTimerThread BaseClass;

   // Constructor
   TimerThread2();

   // Base class overloads
   void executeOnTimer(int aTimeCount);

   void showThreadInfo(int aTimeCount);

   // Members
   int mTimerFrequency;

};

//******************************************************************************
// Global instance

#ifdef _SOMETIMERTHREAD2_CPP_
          TimerThread2* gTimerThread2;
#else
   extern TimerThread2* gTimerThread2;
#endif

//******************************************************************************
}//namespace


#endif

