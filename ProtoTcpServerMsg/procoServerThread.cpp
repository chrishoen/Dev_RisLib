/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "procoMsgHelper.h"
#include "procoTcpSettings.h"

#define  _PROCOSERVERTHREAD_CPP_
#include "procoServerThread.h"

namespace ProtoComm
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

ServerThread::ServerThread()
{
   // Set base class variables.
   BaseClass::setThreadName("Server");
   BaseClass::setThreadPriority(Ris::Threads::gPriorities.mProc);
   BaseClass::mTimerPeriod = 100;

   // Initialize qcalls.
   mSessionQCall.bind(this, &ServerThread::executeSession);
   mRxMsgQCall.bind   (this,&ServerThread::executeRxMsg);

   // Initialize member variables.
   mMsgThread = 0;
   mMsgMonkey = new MsgMonkey;
   mConnectionFlag = false;
   mTPCode = 0;
   mRxCount = 0;
   mTxCount = 0;
   mShowCode = 0;
}

ServerThread::~ServerThread()
{
   if (mMsgThread) delete mMsgThread;
   delete mMsgMonkey;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show thread info for this thread and for child threads.

void ServerThread::showThreadInfo()
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
// child TcpMsgServerThread.

void ServerThread::threadInitFunction()
{
   Trc::write(11, 0, "ServerThread::threadInitFunction");

   // Instance of network socket settings.
   Ris::Net::Settings tSettings;
   tSettings.setLocalPort(gTcpSettings.mTcpServerPort);
   tSettings.mMsgMonkey = mMsgMonkey;
   tSettings.mMaxSessions = gTcpSettings.mTcpMaxSessions;
   tSettings.mServerSessionQCall = mSessionQCall;
   tSettings.mServerRxMsgQCall = mRxMsgQCall;
   tSettings.mTraceIndex = 11;
   Trc::start(11);

   // Create the child thread with the settings.
   mMsgThread = new Ris::Net::TcpMsgServerThread(tSettings);

   // Launch the child thread.
   mMsgThread->launchThread();

   Trc::write(11, 0, "ServerThread::threadInitFunction done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immedidately
// before the thread is terminated. It shuts down the child TcpMsgServerThread.

void ServerThread::threadExitFunction()
{
   Trc::write(11, 0, "ServerThread::threadExitFunction");
   Prn::print(0, "ServerThread::threadExitFunction BEGIN");

   // Shutdown the child thread.
   mMsgThread->shutdownThread();

   Prn::print(0, "ServerThread::threadExitFunction END");
   Trc::write(11, 0, "ServerThread::threadExitFunction done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread shutdown function. This calls the base class shutdownThread
// function to terminate the thread. This executes in the context of
// the calling thread.

void ServerThread::shutdownThread()
{
   Trc::write(11, 0, "ServerThread::shutdownThread");
   Prn::print(0, "ServerThread::shutdownThread BEGIN");
   BaseClass::shutdownThread();
   Prn::print(0, "ServerThread::shutdownThread END");
   Trc::write(11, 0, "ServerThread::shutdownThread done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// QCall registered to the mTcpMsgThread child thread. It is invoked when
// a session is established or disestablished (when a client connects or
// disconnects). It maintains session state variables.

void ServerThread::executeSession(int aSessionIndex, bool aConnected)
{
   if (aConnected) Prn::print(Prn::Show1, "ServerThread CONNECTED     %d", aSessionIndex);
   else            Prn::print(Prn::Show1, "ServerThread DISCONNECTED  %d", aSessionIndex);

   if (!aConnected)
   {
      // The connection was lost, so remove the session from the state list
      mSessionStateList.remove(aSessionIndex);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute periodically. This is called by the base class timer.

void ServerThread::executeOnTimer(int aTimerCount)
{
   if (mTPCode == 1)
   {
      EchoRequestMsg* tTxMsg = new EchoRequestMsg;
      MsgHelper::initialize(tTxMsg);
      sendMsg(0, tTxMsg);
   }
   else if (mTPCode == 2)
   {
      ByteBlobMsg* tTxMsg = new ByteBlobMsg;
      MsgHelper::initialize2(tTxMsg);
      sendMsg(0, tTxMsg);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// qcall registered to the mMsgThread child thread. It is invoked by
// the child thread when a message is received.
// Based on the receive message type, call one of the specific receive
// message handlers. This is bound to the qcall.

void ServerThread::executeRxMsg(int aSessionIndex, Ris::ByteContent* aMsg)
{
   ProtoComm::BaseMsg* tMsg = (ProtoComm::BaseMsg*)aMsg;

   // Message jump table based on message type.
   // Call corresponding specfic message handler method.
   switch (tMsg->mMessageType)
   {
   case ProtoComm::MsgIdT::cTestMsg:
      processRxMsg(aSessionIndex, (ProtoComm::TestMsg*)tMsg);
      break;
   case ProtoComm::MsgIdT::cFirstMessageMsg:
      processRxMsg(aSessionIndex, (ProtoComm::FirstMessageMsg*)tMsg);
      break;
   case ProtoComm::MsgIdT::cEchoRequestMsg:
      processRxMsg(aSessionIndex, (ProtoComm::EchoRequestMsg*)tMsg);
      break;
   case ProtoComm::MsgIdT::cEchoResponseMsg:
      processRxMsg(aSessionIndex, (ProtoComm::EchoResponseMsg*)tMsg);
      break;
   case ProtoComm::MsgIdT::cDataMsg:
      processRxMsg(aSessionIndex, (ProtoComm::DataMsg*)tMsg);
      break;
   case ProtoComm::MsgIdT::cByteBlobMsg:
      processRxMsg(aSessionIndex, (ProtoComm::ByteBlobMsg*)tMsg);
      break;
   default:
      Prn::print(Prn::Show1, "ServerThread::executeServerRxMsg ??? %d", tMsg->mMessageType);
      delete tMsg;
      break;
   }
   mRxCount++;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Message handler - TestMsg.

void ServerThread::processRxMsg(int aSessionIndex, ProtoComm::TestMsg*  aRxMsg)
{
   MsgHelper::show(Prn::Show1, aRxMsg);
   delete aRxMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - FirstMessageMsg.
//
// Specfic message handler for a FirstMessage. It adds the session to the 
// session state list. This message is sent by the client when a connection
// is established.

void ServerThread::processRxMsg(int aSessionIndex, FirstMessageMsg* aRxMsg)
{
   Prn::print(Prn::Show1, "processRxMsg_FirstMessageMsg %d %d", aSessionIndex, aRxMsg->mCode1);

   // Add session to state list.
   mSessionStateList.add(aSessionIndex, aRxMsg->mCode1);

   // Delete the message
   delete aRxMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - EchoRequestMsg.

void ServerThread::processRxMsg(int aSessionIndex, ProtoComm::EchoRequestMsg* aRxMsg)
{
   if (true)
   {
      ProtoComm::EchoResponseMsg* tTxMsg = new ProtoComm::EchoResponseMsg;
      MsgHelper::initialize(tTxMsg, 1000);
      tTxMsg->mCode1 = aRxMsg->mCode1;
      mMsgThread->sendMsg(aSessionIndex, tTxMsg);
   }
   if (mShowCode == 3)
   {
      MsgHelper::show(Prn::Show1, aRxMsg);
   }
   delete aRxMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - EchoResponseMsg.

void ServerThread::processRxMsg(int aSessionIndex, ProtoComm::EchoResponseMsg* aRxMsg)
{
   if (mShowCode == 3)
   {
      MsgHelper::show(Prn::Show1, aRxMsg);
   }
   delete aRxMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - DataMsg.

void ServerThread::processRxMsg(int aSessionIndex, ProtoComm::DataMsg* aRxMsg)
{
   if (mShowCode == 3)
   {
      MsgHelper::show(Prn::Show1, aRxMsg);
   }
   delete aRxMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - ByteBlobMsg.

void ServerThread::processRxMsg(int aSessionIndex, ProtoComm::ByteBlobMsg* aRxMsg)
{
   if (mShowCode == 3)
   {
      MsgHelper::show(Prn::Show1, aRxMsg);
   }
   delete aRxMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a message via mMsgThread:

void ServerThread::sendMsg(int aSessionIndex, BaseMsg* aTxMsg)
{
   mMsgThread->sendMsg(aSessionIndex, aTxMsg);
   mTxCount++;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a message via mMsgThread:

void ServerThread::sendTestMsg(int aAppNumber)
{
   // Get the session index associated with the application number.
   int tSessionIndex = mSessionStateList.getIndex(aAppNumber);
   if (tSessionIndex == Ris::Net::SessionStateList::cInvalidValue) return;

   // Send a message on socket at the session index.
   TestMsg* tMsg = new TestMsg;
   tMsg->mCode1 = 201;
   mMsgThread->sendMsg(tSessionIndex, tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace