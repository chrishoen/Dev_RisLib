/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "someThreadParms.h"

#define  _TESTQCALLTHREAD_CPP_
#include "someTestQCallThread.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

TestQCallThread::TestQCallThread()
{
   // Set base class thread priority.
   BaseClass::setThreadPriorityHigh();

   // Set base class timer period.
   BaseClass::mTimerPeriod = 1000;

   // Initialize qcalls.
   mTest1QCall.bind(this, &TestQCallThread::executeTest1);

   // Initialize variables.
   mTPFlag = true;
   mStatusCount1=0;
   mStatusCount2=0;
}

TestQCallThread::~TestQCallThread()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immediately 
// after the thread starts running.

void TestQCallThread::threadInitFunction()
{
   Prn::print(Prn::ThreadInit1, "TestQCallThread::threadInitFunction");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function, base class overload.

void  TestQCallThread::threadExitFunction()
{
   Prn::print(Prn::ThreadInit1, "TestQCallThread::threadExitFunction");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute periodically. This is called by the base class timer.

void TestQCallThread::executeOnTimer(int aTimerCount)
{
   if (!mTPFlag) return;
   Prn::print(Prn::View11, "StatusCount %10d", mStatusCount1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// QCall registered to the mUdpMsgThread child thread. It is invoked when
// a message is received. It process the received messages.

void TestQCallThread::executeTest1(int aSource,int aCode1)
{
   if (gThreadParms.mShowCode == 1 || aSource == 7)
   {
      Prn::print(Prn::View21, "TestQCallThread::executeTest1 %d %d", aSource, aCode1);
   }
   mStatusCount1++;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace