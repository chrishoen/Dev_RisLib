/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "risSleep.h"
#include "procoMsgHelper.h"
#include "procoUdpSettings.h"

#define  _PROCOPEERTHREAD_CPP_
#include "procoPeerThread.h"

namespace ProtoComm
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

PeerThread::PeerThread()
{
   // Set base class variables.
   BaseClass::setThreadName("Peer");
   BaseClass::setThreadPriority(Ris::Threads::gPriorities.mProc);
   BaseClass::mTimerPeriod = 100;

   // Initialize qcalls.
   mRxMsgQCall.bind   (this,&PeerThread::executeRxMsg);

   // Initialize member variables.
   mMsgThread = 0;
   mMsgMonkey = new MsgMonkey;
   mConnectionFlag = false;
   mTPCode = 0;
   mRxCount = 0;
   mTxCount = 0;
   mShowCode = 0;
}

PeerThread::~PeerThread()
{
   if (mMsgThread)
   {
      delete mMsgThread;
      mMsgThread = 0;
   }
   delete mMsgMonkey;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show thread info for this thread and for child threads.

void PeerThread::showThreadInfo()
{
   BaseClass::showThreadInfo();
   if (mMsgThread)
   {
      mMsgThread->showThreadInfo();
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immedidately 
// after the thread starts running. It creates and launches the 
// child UdpMsgThread.

void PeerThread::threadInitFunction()
{
   Trc::write(11, 0, "PeerThread::threadInitFunction");

   // Instance of network socket settings.
   Ris::Net::Settings tSettings;

   tSettings.setLocalPort(gUdpSettings.mMyUdpPort);
   tSettings.setRemoteAddress(gUdpSettings.mOtherUdpIPAddress, gUdpSettings.mOtherUdpPort);
   tSettings.setUdpWrapFlag(gUdpSettings.mUdpWrapFlag);
   tSettings.mMsgMonkey = mMsgMonkey;
   tSettings.mRxMsgQCall = mRxMsgQCall;
   tSettings.mTraceIndex = 11;
   Trc::start(11);

   // Create the child thread with the settings.
   mMsgThread = new Ris::Net::UdpMsgThread(tSettings);

   // Launch the child thread.
   mMsgThread->launchThread();

   Trc::write(11, 0, "PeerThread::threadInitFunction done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immedidately
// before the thread is terminated. It shuts down the child UdpMsgThread.

void PeerThread::threadExitFunction()
{
   Trc::write(11, 0, "PeerThread::threadExitFunction");
   Prn::print(0, "PeerThread::threadExitFunction BEGIN");

   // Shutdown the child thread.
   mMsgThread->shutdownThread();

   Prn::print(0, "PeerThread::threadExitFunction END");
   Trc::write(11, 0, "PeerThread::threadExitFunction done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread shutdown function. This calls the base class shutdownThread
// function to terminate the thread. This executes in the context of
// the calling thread.

void PeerThread::shutdownThread()
{
   Trc::write(11, 0, "PeerThread::shutdownThread");
   Prn::print(0, "PeerThread::shutdownThread BEGIN");
   BaseClass::shutdownThread();
   Prn::print(0, "PeerThread::shutdownThread END");
   Trc::write(11, 0, "PeerThread::shutdownThread done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute periodically. This is called by the base class timer.

void PeerThread::executeOnTimer(int aTimerCount)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// qcall registered to the mMsgThread child thread. It is invoked by
// the child thread when a message is received.
// Based on the receive message type, call one of the specific receive
// message handlers. This is bound to the qcall.

void PeerThread::executeRxMsg(Ris::ByteContent* aMsg)
{
   delete aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace