/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "procoUdpSettings.h"

#define  _PROCOPROCTHREAD_CPP_
#include "procoProcThread.h"

namespace ProtoComm
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.
ProcThread::ProcThread()
{
   // Set base class variables.
   BaseClass::setThreadName("Udp");
   BaseClass::setThreadPriorityHigh();
   BaseClass::mTimerPeriod = 1000;

   // Initialize qcalls.
   mRxDataQCall.bind   (this,&ProcThread::executeRxData);
   mMaxDataSize = 1000;

   // Initialize member variables.
   mUdpDataThread = 0;
   mConnectionFlag = false;
   mTxCount = 0;
   mRxCount = 0;
   mTPFlag = false;
}

ProcThread::~ProcThread()
{
   if (mUdpDataThread) delete mUdpDataThread;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Place holder.

void ProcThread::show()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immedidately 
// after the thread starts running. It creates and launches the 
// child UdpDataThread.

void ProcThread::threadInitFunction()
{
   Trc::start(11);
   Trc::write(11, 0, "ProcThread::threadInitFunction");

   // Instance of network socket settings.
   Ris::Net::Settings tSettings;

   tSettings.setLocalAddress(gUdpSettings.mMyUdpAddress, gUdpSettings.mMyUdpPort);
   tSettings.setRemoteAddress(gUdpSettings.mOtherUdpAddress, gUdpSettings.mOtherUdpPort);
   tSettings.setUdpWrapFlag(gUdpSettings.mUdpWrapFlag);
   tSettings.setUdpBroadcast(gUdpSettings.mUdpBroadcast);
   tSettings.mRxDataQCall = mRxDataQCall;
   tSettings.mMaxDataSize = mMaxDataSize;
   tSettings.mTraceIndex = 11;

   // Create the child thread with the settings.
   mUdpDataThread = new Ris::Net::UdpDataThread(tSettings);

   // Launch the child thread.
   mUdpDataThread->launchThread();

   Trc::write(11, 0, "ProcThread::threadInitFunction done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immedidately
// before the thread is terminated. It shuts down the child UdpDataThread.

void ProcThread::threadExitFunction()
{
   Prn::print(0, "ProcThread::threadExitFunction BEGIN");

   // Shutdown the child thread.
   mUdpDataThread->shutdownThread();

   Prn::print(0, "ProcThread::threadExitFunction END");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread shutdown function. This calls the base class shutdownThread
// function to terminate the thread. This executes in the context of
// the calling thread.

void ProcThread::shutdownThread()
{
   Prn::print(0, "ProcThread::shutdownThread BEGIN");
   BaseClass::shutdownThread();
   Prn::print(0, "ProcThread::shutdownThread END");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute periodically. This is called by the base class timer.

void ProcThread::executeOnTimer(int aTimerCount)
{
   if (!mTPFlag) return;

   char tData[100];
   memset(tData, 2, 100);
   tData[0] = 1;
   tData[99] = 3;

   // Send data.
   sendData(tData, 100);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// qcall registered to the mUdpDataThread child thread. It is invoked by
// the child thread when a data is received.
// Based on the receive data type, call one of the specific receive
// data handlers. This is bound to the qcall.

void ProcThread::executeRxData(char* aData, int aSize)
{
   Prn::print(Prn::Show1, "RxData %d %d", mRxCount++, aSize);
   delete aData;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a data via mUdpDataThread:

void ProcThread::sendData(const char* aData, int aSize)
{
   mUdpDataThread->sendData(aData, aSize);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace