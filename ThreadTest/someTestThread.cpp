/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "cmnPriorities.h"
#include "someThreadParms.h"

#define  _SOMETESTTHREAD_CPP_
#include "someTestThread.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

TestThread::TestThread()
{
   using namespace std::placeholders;
#if 0
   // Set base class thread variables.
   BaseClass::mLongThread->setThreadName("TestLong");
   BaseClass::mLongThread->setThreadPriority(Cmn::gPriorities.mMasterLong);
   BaseClass::mShortThread->setThreadName("TestShort");
   BaseClass::mShortThread->setThreadPriority(Cmn::gPriorities.mMasterShort);

   // Set base class call pointers.
   BaseClass::mShortThread->mThreadInitCallPointer           = std::bind(&TestThread::threadInitFunction, this);
   BaseClass::mShortThread->mThreadExitCallPointer           = std::bind(&TestThread::threadExitFunction, this);
   BaseClass::mShortThread->mThreadExecuteOnTimerCallPointer = std::bind(&TestThread::executeOnTimer, this, _1);

   // Set qcalls.
   mTest0QCall.bind(this->mLongThread, this, &TestThread::executeTest0);
#endif
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immediately 
// before the thread starts running.

void TestThread::threadInitFunction()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immediately 
// after  the thread starts running.

void TestThread::threadExitFunction()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute periodically. This is called by the base class timer.

void TestThread::executeOnTimer(int aTimerCount)
{
   Prn::print(Prn::View11, "StatusCount %10d %10d", aTimerCount);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TestThread::executeTest0()
{
   Prn::print(Prn::View21, "TestThread::executeTest0");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace