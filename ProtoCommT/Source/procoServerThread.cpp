/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "procoSettings.h"

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
   // Set base class thread priority.
   BaseClass::setThreadPriorityHigh();

   // Set base class timer period.
   BaseClass::mTimerPeriod = 1000;

   // Initialize qcalls.
   mSessionQCall.bind(this, &ServerThread::executeSession);
   mRxMsgQCall.bind(this,   &ServerThread::executeRxMsg);

   // Initialize variables.
   mTcpMsgServerThread = 0;
   mMonkeyCreator.configure(gSettings.mMyAppNumber);
   mTPFlag = false;
   mStatusCount1 = 0;
   mStatusCount2 = 0;
}

ServerThread::~ServerThread()
{
   delete mTcpMsgServerThread;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function, base class overload.

void ServerThread::threadInitFunction()
{
   Prn::print(Prn::ThreadInit1, "ServerThread::threadInitFunction");

   // Instance of network socket settings.
   Ris::Net::Settings tSettings;

   tSettings.setLocalIp(gSettings.mTcpServerIPAddress, gSettings.mTcpServerPort);
   tSettings.mMonkeyCreator = &mMonkeyCreator;
   tSettings.mMaxSessions = gSettings.mTcpMaxSessions;
   tSettings.mServerSessionQCall = mSessionQCall;
   tSettings.mServerRxMsgQCall = mRxMsgQCall;

   // Create the child thread with the settings.
   mTcpMsgServerThread = new Ris::Net::TcpMsgServerThread(tSettings);

   // Launch the child thread.
   mTcpMsgServerThread->launchThread();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function, base class overload.

void  ServerThread::threadExitFunction()
{
   Prn::print(Prn::ThreadInit1, "ServerThread::threadExitFunction");

   // Shutdown the child thread.
   mTcpMsgServerThread->shutdownThread(); 
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// QCall registered to the mTcpMsgThread child thread. It is invoked when
// a session is established or disestablished (when a client connects or
// disconnects). It maintains session state variables.

void ServerThread::executeSession (int aSessionIndex,bool aConnected)
{
   if (aConnected) Prn::print(Prn::ThreadRun1, "ServerThread CLIENT  CONNECTED     %d",aSessionIndex);
   else            Prn::print(Prn::ThreadRun1, "ServerThread CLIENT  DISCONNECTED  %d",aSessionIndex);

   if(!aConnected)
   {
      // The connection was lost, so remove the session from the state list
      mSessionStateList.remove(aSessionIndex);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// QCall registered to the mTcpMsgThread child thread. It is invoked when
// a message is received. It process the received messages. It calls one of
// the specific receive message handlers.

void ServerThread::executeRxMsg(int aSessionIndex,Ris::ByteContent* aMsg)
{
   if(!aMsg) return;

   BaseMsg* tMsg = (BaseMsg*)aMsg;

   // Message jump table based on message type.
   // Calls corresponding specfic message handler method.
   switch (tMsg->mMessageType)
   {
      case MsgIdT::cTestMsg :
         processRxMsg(aSessionIndex,(TestMsg*)tMsg);
         break;
      case MsgIdT::cFirstMessageMsg :
         processRxMsg(aSessionIndex,(FirstMessageMsg*)tMsg);
         break;
      case MsgIdT::cStatusRequestMsg :
         processRxMsg(aSessionIndex,(StatusRequestMsg*)tMsg);
         break;
      case MsgIdT::cStatusResponseMsg :
         processRxMsg(aSessionIndex,(StatusResponseMsg*)tMsg);
         break;
      default :
         Prn::print(Prn::ThreadRun1, "ServerThread::processRxMsg %d",tMsg->mMessageType);
         delete aMsg;
         break;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - TestMsg

void ServerThread::processRxMsg(int aSessionIndex,TestMsg* aMsg)
{
   Prn::print(Prn::ThreadRun1, "ServerThread::processRxMsg_TestMsg");
   delete aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - FirstMessageMsg
//
// Specfic message handler for a FirstMessage. It adds the session to the 
// session state list. This message is sent by the client when a connection
// is established.

void ServerThread::processRxMsg(int aSessionIndex,FirstMessageMsg* aMsg)
{
   Prn::print(Prn::ThreadRun1, "ServerThread::processRxMsg_FirstMessageMsg %d %d",aSessionIndex,aMsg->mHeader.mSourceId);

   // Add session to state list.
   mSessionStateList.add(aSessionIndex,aMsg->mHeader.mSourceId);

   // Delete the message
   delete aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - StatusRequestMsg

void ServerThread::processRxMsg(int aSessionIndex,StatusRequestMsg* aMsg)
{
   Prn::print(Prn::ThreadRun2, "ServerThread::processRxMsg_StatusRequestMsg %d",aMsg->mCode1);

   if (true)
   {
      StatusResponseMsg* tTxMsg = new StatusResponseMsg;
      tTxMsg->mCode1 = aMsg->mCode1;
      sendMsg(aSessionIndex,tTxMsg);
   }

   delete aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - StatusResponseMsg

void ServerThread::processRxMsg(int aSessionIndex,StatusResponseMsg* aMsg)
{
   Prn::print(Prn::ThreadRun2, "ServerThread::processRxMsg_StatusResponseMsg");
   delete aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a message via the child thread.

void ServerThread::sendMsg(int aSessionIndex,BaseMsg* aMsg)
{
   // Send a message on socket at the session index.
   mTcpMsgServerThread->sendMsg(aSessionIndex,aMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a message via the child thread.

void ServerThread::sendTestMsg(int aAppNumber)
{
   // Get the session index associated with the application number.
   int tSessionIndex = mSessionStateList.getIndex(aAppNumber);
   if (tSessionIndex == Ris::Net::SessionStateList::InvalidValue) return;

   // Send a message on socket at the session index.
   TestMsg* msg = new TestMsg;
   msg->mCode1=201;

   mTcpMsgServerThread->sendMsg(tSessionIndex,msg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// executeOnTimer, base class overload.

void ServerThread::executeOnTimer(int aTimerCount)
{
   Prn::print(Prn::ThreadRun3, "ServerThread::executeOnTimer %d", mPeriodicCount++);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
