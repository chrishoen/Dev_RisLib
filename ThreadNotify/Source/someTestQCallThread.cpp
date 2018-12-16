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
   // Set base class thread variables.
   BaseClass::setThreadName("TestQCall");
   BaseClass::setThreadPrintLevel(TS::PrintLevel(3, 3));
   BaseClass::setThreadPriorityHigh();
   BaseClass::mTimerPeriod = 1000;

   // Initialize qcalls.
   mTest1QCall.bind(this, &TestQCallThread::executeTest1);
   mTest2QCall.bind(this, &TestQCallThread::executeTest2);
   mTest3QCall.bind(this, &TestQCallThread::executeTest3);

   // Initialize variables.
   mTPFlag = false;
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
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function, base class overload.

void  TestQCallThread::threadExitFunction()
{
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
// Test qcall function. 

void TestQCallThread::executeTest1(int aCode1)
{
   Prn::print(Prn::View11, "Test1 BEGIN");

   mNotify.setMaskSingle(1);
   mNotify.wait(-1);

   Prn::print(Prn::View11, "Test1 END");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Test qcall function. 

void TestQCallThread::executeTest2(int aCode1)
{
   Prn::print(Prn::View11, "Test2 BEGIN");

   mNotify.setMaskAny(2, 1, 2);
   mNotify.wait(-1);

   Prn::print(Prn::View11, "Test2 END");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Test qcall function. 

void TestQCallThread::executeTest3(int aCode1)
{
   Prn::print(Prn::View11, "Test3 BEGIN");

   mNotify.setMaskAll(2, 1, 2);
   mNotify.wait(-1);

   Prn::print(Prn::View11, "Test3 END");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace