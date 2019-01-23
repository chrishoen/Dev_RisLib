/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "procoMsgHelper.h"
#include "procoTcpSettings.h"

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
   // Set base class thread priority.
   BaseClass::setThreadPriorityHigh();

   // Set base class timer period.
   BaseClass::mTimerPeriod = gTcpSettings.mThreadTimerPeriod;

   // Initialize qcalls.
   mSessionQCall.bind(this, &ClientThread::executeSession);
   mRxMsgQCall.bind(this, &ClientThread::executeRxMsg);

   // Initialize variables.
   mTcpMsgClientThread = 0;
   mMonkeyCreator.configure(gTcpSettings.mMyAppNumber);
   mTPFlag = false;
   mStatusCount1 = 0;
   mStatusCount2 = 0;
}

ClientThread::~ClientThread()
{
   delete mTcpMsgClientThread;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immediately 
// after the thread starts running. It starts the child thread.

void ClientThread::threadInitFunction()
{
   Prn::print(Prn::ThreadInit1, "ClientThread::threadInitFunction");

   // Instance of network socket settings.
   Ris::Net::Settings tSettings;

   tSettings.setRemoteIp(gTcpSettings.mTcpServerIPAddress, gTcpSettings.mTcpServerPort);
   tSettings.mMonkeyCreator = &mMonkeyCreator;
   tSettings.mClientSessionQCall = mSessionQCall;
   tSettings.mRxMsgQCall = mRxMsgQCall;

   // Create the child thread with the settings.
   mTcpMsgClientThread = new Ris::Net::TcpMsgClientThread(tSettings);

   // Launch the child thread.
   mTcpMsgClientThread->launchThread();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immediately
// before the thread is terminated. It shuts down the child thread.

void  ClientThread::threadExitFunction()
{
   Prn::print(Prn::ThreadInit1, "ClientThread::threadExitFunction");

   // Shutdown the child thread.
   mTcpMsgClientThread->shutdownThread();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// QCall registered to the mTcpMsgThread child thread. It is invoked when
// a session is established or disestablished (when this client connects or
// disconnects to the server). It maintains session state variables. When
// a connection is established it sends a FirstMessage to the server to
// inform it of it's identity.

void ClientThread::executeSession (bool aConnected)
{
   if (aConnected)
   {
      Prn::print(Prn::ThreadRun1, "ClientThread CONNECTED");

      // Transmit a FirstMessage to the server to inform it of who this 
      // client is.
      FirstMessageMsg* msg = new FirstMessageMsg;
      sendMsg(msg);
   }
   else
   {
      Prn::print(Prn::ThreadRun1, "ClientThread DISCONNECTED");
   }

   mConnectionFlag = aConnected;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// QCall registered to the mTcpMsgThread child thread. It is invoked when
// a message is received. It process the received messages. It calls one of
// the specific receive message handlers.

void ClientThread::executeRxMsg(Ris::ByteContent* aMsg)
{
   if(!aMsg) return;

   BaseMsg* tMsg = (BaseMsg*)aMsg;

   // Message jump table based on message type.
   // Calls corresponding specfic message handler method.
   switch (tMsg->mMessageType)
   {
      case MsgIdT::cTestMsg :
         processRxMsg((TestMsg*)tMsg);
         break;
      case MsgIdT::cEchoRequestMsg :
         processRxMsg((EchoRequestMsg*)tMsg);
         break;
      case MsgIdT::cEchoResponseMsg:
         processRxMsg((EchoResponseMsg*)tMsg);
         break;
      case MsgIdT::cDataMsg:
         processRxMsg((DataMsg*)tMsg);
         break;
      default :
         Prn::print(Prn::ThreadRun1, "processRxMsg %d",tMsg->mMessageType);
         delete aMsg;
         break;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - TestMsg.

void ClientThread::processRxMsg(TestMsg* aMsg)
{
   Prn::print(Prn::ThreadRun1, "processRxMsg_TestMsg");
   delete aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - EchoRequestMsg.

void ClientThread::processRxMsg(EchoRequestMsg* aMsg)
{
   Prn::print(Prn::ThreadRun1, "processRxMsg_EchoRequestMsg %d %d", aMsg->mCode1, aMsg->mNumWords);

   EchoResponseMsg* tTxMsg = new EchoResponseMsg;
   tTxMsg->mCode1 = aMsg->mCode1;
   tTxMsg->mNumWords = aMsg->mNumWords;
   sendMsg(tTxMsg);

   delete aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - EchoResponseMsg.

void ClientThread::processRxMsg(EchoResponseMsg* aMsg)
{
   Prn::print(Prn::ThreadRun1, "processRxMsg_EchoResponseMsg %d %d", aMsg->mCode1, aMsg->mNumWords);
   delete aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - DataMsg.

void ClientThread::processRxMsg(DataMsg* aMsg)
{
   Prn::print(Prn::ThreadRun1, "processRxMsg_DataMsg");
   MsgHelper::show(aMsg);
   delete aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a message via the child thread.

void ClientThread::sendMsg (BaseMsg* aTxMsg)
{
   mTcpMsgClientThread->sendMsg(aTxMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a message via the child thread.

void ClientThread::sendTestMsg()
{
   TestMsg* msg = new TestMsg;
   msg->mCode1=201;
 
   mTcpMsgClientThread->sendMsg(msg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute periodically. This is called by the base class timer. It
// sends an echo request message.

void ClientThread::executeOnTimer(int aTimerCount)
{
   if (!mTPFlag) return;
   Prn::print(Prn::ThreadRun3, "ClientThread::executeOnTimer %d", aTimerCount);

   // Send a status request message.
   EchoRequestMsg* tMsg = new EchoRequestMsg;
   tMsg->mCode1 = aTimerCount;
   tMsg->mNumWords = gTcpSettings.mNumWords;
   sendMsg(tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace