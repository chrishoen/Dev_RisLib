/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "procoMsgHelper.h"
#include "procoSppParms.h"

#define  _PROCOCLIENTTHREAD_CPP_
#include "procoClientThread.h"

namespace ProtoComm
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

ClientThread::ClientThread()
{
   // Set base class variables.
   BaseClass::setThreadName("Client");
   BaseClass::setThreadPriority(Ris::Threads::gPriorities.mProc);
   BaseClass::mTimerPeriod = 100;

   // Initialize qcalls.
   mSessionQCall.bind(this, &ClientThread::executeSession);
   mRxMsgQCall.bind   (this,&ClientThread::executeRxMsg);

   // Initialize member variables.
   mMsgThread = 0;
   mMsgMonkey = new MsgMonkey;
   mConnectionFlag = false;
   mTPCode = 0;
   mRxCount = 0;
   mTxCount = 0;
   mShowCode = 0;
}

ClientThread::~ClientThread()
{
   if (mMsgThread) delete mMsgThread;
   delete mMsgMonkey;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show thread info for this thread and for child threads.

void ClientThread::showThreadInfo()
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
// child TcpMsgClientThread.

void ClientThread::threadInitFunction()
{
   Trc::write(11, 0, "ClientThread::threadInitFunction");

   // Instance of network socket settings.
   Ris::Bt::Settings tSettings;
   tSettings.setRemoteName(gSppParms.mRemoteName);
   tSettings.mMsgMonkey = mMsgMonkey;
   tSettings.mClientSessionQCall = mSessionQCall;
   tSettings.mRxMsgQCall = mRxMsgQCall;
   tSettings.mTraceIndex = 11;
   Trc::start(1);
   Trc::start(11);

   // Create the child thread with the settings.
   mMsgThread = new Ris::Bt::SppMsgClientThread(tSettings);

   // Launch the child thread.
   mMsgThread->launchThread();

   Trc::write(11, 0, "ClientThread::threadInitFunction done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immedidately
// before the thread is terminated. It shuts down the child TcpMsgClientThread.

void ClientThread::threadExitFunction()
{
   Trc::write(11, 0, "ClientThread::threadExitFunction");
   Prn::print(0, "ClientThread::threadExitFunction BEGIN");

   // Shutdown the child thread.
   mMsgThread->shutdownThread();

   Prn::print(0, "ClientThread::threadExitFunction END");
   Trc::write(11, 0, "ClientThread::threadExitFunction done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread shutdown function. This calls the base class shutdownThread
// function to terminate the thread. This executes in the context of
// the calling thread.

void ClientThread::shutdownThread()
{
   Trc::write(11, 0, "ClientThread::shutdownThread");
   Prn::print(0, "ClientThread::shutdownThread BEGIN");
   BaseClass::shutdownThread();
   Prn::print(0, "ClientThread::shutdownThread END");
   Trc::write(11, 0, "ClientThread::shutdownThread done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// QCall registered to the mTcpMsgThread child thread. It is invoked when
// a session is established or disestablished (when this client connects or
// disconnects to the server). It maintains session state variables. When
// a connection is established it sends a FirstMessage to the server to
// inform it of it's identity.

void ClientThread::executeSession(bool aConnected)
{
   if (aConnected)
   {
      Prn::print(Prn::Show1, "ClientThread CONNECTED");

      // Transmit a FirstMessage to the server to inform it of who this 
      // client is.
      FirstMessageMsg* tTxMsg = new FirstMessageMsg;
      tTxMsg->mCode1 = 101;
      sendMsg(tTxMsg);
   }
   else
   {
      Prn::print(Prn::Show1, "ClientThread DISCONNECTED");
   }

   mConnectionFlag = aConnected;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute periodically. This is called by the base class timer.

void ClientThread::executeOnTimer(int aTimerCount)
{
   if (mTPCode == 1)
   {
      EchoRequestMsg* tTxMsg = new EchoRequestMsg;
      tTxMsg->initialize(1000);
      sendMsg(tTxMsg);
   }
   else if (mTPCode == 2)
   {
      ByteBlobMsg* tTxMsg = new ByteBlobMsg;
      MsgHelper::initialize2(tTxMsg);
      sendMsg(tTxMsg);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// qcall registered to the mMsgThread child thread. It is invoked by
// the child thread when a message is received.
// Based on the receive message type, call one of the specific receive
// message handlers. This is bound to the qcall.

void ClientThread::executeRxMsg(Ris::ByteContent* aMsg)
{
   ProtoComm::BaseMsg* tMsg = (ProtoComm::BaseMsg*)aMsg;

   // Message jump table based on message type.
   // Call corresponding specfic message handler method.
   switch (tMsg->mMessageType)
   {
   case ProtoComm::MsgIdT::cTestMsg:
      processRxMsg((ProtoComm::TestMsg*)tMsg);
      break;
   case ProtoComm::MsgIdT::cEchoRequestMsg:
      processRxMsg((ProtoComm::EchoRequestMsg*)tMsg);
      break;
   case ProtoComm::MsgIdT::cEchoResponseMsg:
      processRxMsg((ProtoComm::EchoResponseMsg*)tMsg);
      break;
   case ProtoComm::MsgIdT::cDataMsg:
      processRxMsg((ProtoComm::DataMsg*)tMsg);
      break;
   case ProtoComm::MsgIdT::cByteBlobMsg:
      processRxMsg((ProtoComm::ByteBlobMsg*)tMsg);
      break;
   default:
      Prn::print(Prn::Show1, "ClientThread::executeServerRxMsg ??? %d", tMsg->mMessageType);
      delete tMsg;
      break;
   }
   mRxCount++;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Message handler - TestMsg.

void ClientThread::processRxMsg(ProtoComm::TestMsg*  aRxMsg)
{
   MsgHelper::show(Prn::Show1, aRxMsg);
   delete aRxMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - EchoRequestMsg.

void ClientThread::processRxMsg(ProtoComm::EchoRequestMsg* aRxMsg)
{
   if (true)
   {
      ProtoComm::EchoResponseMsg* tTxMsg = new ProtoComm::EchoResponseMsg;
      tTxMsg->initialize(1000);
      tTxMsg->mCode1 = aRxMsg->mCode1;
      mMsgThread->sendMsg(tTxMsg);
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

void ClientThread::processRxMsg(ProtoComm::EchoResponseMsg* aRxMsg)
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

void ClientThread::processRxMsg(ProtoComm::DataMsg* aRxMsg)
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

void ClientThread::processRxMsg(ProtoComm::ByteBlobMsg* aRxMsg)
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

void ClientThread::sendMsg(BaseMsg* aTxMsg)
{
   mMsgThread->sendMsg(aTxMsg);
   mTxCount++;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a message via mMsgThread:

void ClientThread::sendTestMsg()
{
   TestMsg* tMsg = new TestMsg;
   tMsg->mCode1 = 201;

   mMsgThread->sendMsg(tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace