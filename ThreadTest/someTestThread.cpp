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
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

TestParentThread::TestParentThread()
{
   mTimerPeriod = 0;
   mThreadInitCallPointer = 0;
   mThreadExitCallPointer = 0;
   mThreadExecuteOnTimerCallPointer = 0;
}

TestParentThread::~TestParentThread()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// BaseClass overload.

void TestParentThread::threadInitFunction()
{
   // Call the call pointer.
   if (mThreadInitCallPointer) mThreadInitCallPointer();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// BaseClass overload.

void TestParentThread::threadExitFunction()
{
   // Call the call pointer.
   if (mThreadExitCallPointer) mThreadExitCallPointer();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// BaseClass overload.

void TestParentThread::executeOnTimer(int aTimerCount)
{
   // Call the call pointer.
   if (mThreadExecuteOnTimerCallPointer) mThreadExecuteOnTimerCallPointer(aTimerCount);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

TestChildThread::TestChildThread()
{
   using namespace std::placeholders;
#if 0
   // Set base class thread variables.
   BaseClass::mLongThread->setThreadName("TestLong");
   BaseClass::mLongThread->setThreadPriority(Cmn::gPriorities.mMasterLong);
   BaseClass::mShortThread->setThreadName("TestShort");
   BaseClass::mShortThread->setThreadPriority(Cmn::gPriorities.mMasterShort);

   // Set base class call pointers.
   BaseClass::mShortThread->mThreadInitCallPointer           = std::bind(&TestChildThread::threadInitFunction, this);
   BaseClass::mShortThread->mThreadExitCallPointer           = std::bind(&TestChildThread::threadExitFunction, this);
   BaseClass::mShortThread->mThreadExecuteOnTimerCallPointer = std::bind(&TestChildThread::executeOnTimer, this, _1);

   // Set qcalls.
   mTest0QCall.bind(this->mLongThread, this, &TestChildThread::executeTest0);
   BaseClass::setThreadName("TestShort");
   BaseClass::setThreadPriority(Cmn::gPriorities.mMasterShort);

   // Set base class call pointers.
   BaseClass::mThreadInitCallPointer = std::bind(&TestChildThread::threadInitFunction, this);
   BaseClass::mThreadExitCallPointer = std::bind(&TestChildThread::threadExitFunction, this);
   BaseClass::mThreadExecuteOnTimerCallPointer = std::bind(&TestChildThread::executeOnTimer, this, _1);
#endif
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immediately 
// before the thread starts running.

#if 0
void TestChildThread::threadInitFunction()
{
}
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immediately 
// after  the thread starts running.

#if 0
void TestChildThread::threadExitFunction()
{
}
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute periodically. This is called by the base class timer.

#if 0
void TestChildThread::executeOnTimer(int aTimerCount)
{
   Prn::print(Prn::View11, "StatusCount %10d %10d", aTimerCount);
}
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TestChildThread::executeTest0()
{
   Prn::print(Prn::View21, "TestChildThread::executeTest0");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace