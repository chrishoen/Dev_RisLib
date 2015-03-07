#ifndef _SOMETIMERTHREAD_H_
#define _SOMETIMERTHREAD_H_

/*==============================================================================
File: someThread1.h
Description:
QCall test thread class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risThreadsTimerThread.h"
#include "risTimerTester.h"

namespace Some
{
//******************************************************************************
//******************************************************************************
//******************************************************************************

class TimerThread : public Ris::Threads::BaseTimerThread
{
public:
   typedef Ris::Threads::BaseTimerThread BaseClass;

   // Constructor
   TimerThread();

   // Base class overloads
   void executeOnTimer(int aTimeCount);

   //--------------------------------------------------------------
   //--------------------------------------------------------------
   //--------------------------------------------------------------
   // Members:

   Ris::TimerTester mTimerTester;

   bool mTPFlag;
};

//******************************************************************************
// Global instance

#ifdef _SOMETIMERTHREAD_CPP_
          TimerThread* gTimerThread;
#else
   extern TimerThread* gTimerThread;
#endif

//******************************************************************************
}//namespace


#endif

