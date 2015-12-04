/*==============================================================================
Copyright (C) 2006 Northrop Grumman Amherst Systems, Inc.
All rights reserved.

Northrop Grumman Amherst Systms, Inc.
1740 Wehrle Drive
Buffalo, New York 14221

File: NetworkThread.cpp

Description:

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risSockets.h"
#include "prnPrint.h"
#include "procoSettings.h"

#define  _PROCONETWORKTHREAD_CPP_
#include "procoNetworkThread.h"


namespace ProtoComm
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

NetworkThread::NetworkThread()
{
   mStatusCount1=0;
   mStatusCount2=0;

   mUdpRxThread = new Ris::Net::UdpRxThread;

   // Initialize QCalls
   mRxMsgQCall.bind   (this,&NetworkThread::executeRxMsg);
}

//******************************************************************************

NetworkThread::~NetworkThread()
{
   delete mUdpRxThread;
}

//******************************************************************************
// This sets configuration members

void NetworkThread::configure()
{
   Prn::print(Prn::ThreadInit,Prn::Init1, "NetworkThread::configure");

   //--------------------------------------------------------------------------- 
   // Configure message parser
   mMessageParserCreator.configure(gSettings.mMyAppNumber);

   //---------------------------------------------------------------------------
   // Configure receive socket thread

   mUdpRxThread->configure(
      "127.0.0.1",
      gSettings.mMyUdpPort,
      &mMessageParserCreator,
      &mRxMsgQCall);


   //---------------------------------------------------------------------------
   // Configure transmit socket

   Ris::Sockets::SocketAddress tSocketAddress;

   tSocketAddress.set(
      "127.0.0.1",
      gSettings.mOtherUdpPort);

   mTxSocket.configure(
      tSocketAddress,
      &mMessageParserCreator);

}

//******************************************************************************

void NetworkThread::launchThread()
{
   Prn::print(Prn::ThreadInit,Prn::Init1, "NetworkThread::launch");

   // Launch child thread
   mUdpRxThread->launchThread(); 
   
   // Launch this thread
   BaseClass::launchThread();
}

//******************************************************************************
// Thread exit function, base class overload.
void  NetworkThread::threadExitFunction()
{
   Prn::print(Prn::ThreadInit,Prn::Init1, "NetworkThread::threadExitFunction");

   // Shutdown the tcp client thread
   mUdpRxThread->shutdownThread();

   // Base class exit
   BaseClass::threadExitFunction();
}

//******************************************************************************
// QCall

void NetworkThread::executeRxMsg(Ris::ByteContent* aRxMsg)
{
   ProtoComm::BaseMsg* tRxMsg = (ProtoComm::BaseMsg*)aRxMsg;

   // Message jump table based on message type.
   // Calls corresponding specfic message handler method.
   switch (tRxMsg->mMessageType)
   {
      case ProtoComm::MsgIdT::Test :
         processRxMsg((ProtoComm::TestMsg*)tRxMsg);
         break;
      case ProtoComm::MsgIdT::StatusRequest :
         processRxMsg((ProtoComm::StatusRequestMsg*)tRxMsg);
         break;
      case ProtoComm::MsgIdT::StatusResponse:
         processRxMsg((ProtoComm::StatusResponseMsg*)tRxMsg);
         break;
      case ProtoComm::MsgIdT::Data:
         processRxMsg((ProtoComm::DataMsg*)tRxMsg);
         break;
      default :
         Prn::print(Prn::ThreadRun,Prn::Run1, "NetworkThread::executeServerRxMsg ??? %d",tRxMsg->mMessageType);
         delete tRxMsg;
         break;
   }
}

//******************************************************************************
// Message handler for TestMsg.

void NetworkThread::processRxMsg(ProtoComm::TestMsg*  aRxMsg)
{
   Prn::print(Prn::ThreadRun, Prn::Run1, "NetworkThread::processRxMsg_TestMsg %d %d", aRxMsg->mCode1, aRxMsg->mHeader.mSourceId);
}

//******************************************************************************
// Rx message handler - StatusRequestMsg

void NetworkThread::processRxMsg(ProtoComm::StatusRequestMsg* aRxMsg)
{
   if (true)
   {
      ProtoComm::StatusResponseMsg* tTxMsg = new ProtoComm::StatusResponseMsg;
      sendMsg(tTxMsg);
   }

   Prn::print(Prn::ThreadRun,Prn::Run1, "NetworkThread::processRxMsg_StatusRequestMsg %d",mStatusCount1++);
   Prn::print(Prn::ThreadRun, Prn::Run1, "Code1      %d", aRxMsg->mCode1);
   Prn::print(Prn::ThreadRun, Prn::Run1, "Code2      %d", aRxMsg->mCode2);
   Prn::print(Prn::ThreadRun, Prn::Run1, "Code3      %d", aRxMsg->mCode3);
   Prn::print(Prn::ThreadRun, Prn::Run1, "Code4      %d", aRxMsg->mCode4);

   delete aRxMsg;
}

//******************************************************************************
// Rx message handler - StatusResponseMsg

void NetworkThread::processRxMsg(ProtoComm::StatusResponseMsg* aRxMsg)
{
   Prn::print(Prn::ThreadRun, Prn::Run1, "NetworkThread::processRxMsg_StatusResponseMsg");
   delete aRxMsg;
}

//******************************************************************************
// Rx message handler - DataMsg

void NetworkThread::processRxMsg(ProtoComm::DataMsg* aRxMsg)
{
   Prn::print(Prn::ThreadRun, Prn::Run1, "NetworkThread::processRxMsg_DataMsg");

   aRxMsg->show();

   delete aRxMsg;



}

//******************************************************************************
// QCall

void NetworkThread::executeOnTimer(int aTimerCount)
{
   Prn::print(Prn::ThreadRun,Prn::Run2, "NetworkThread::executeRxMsg");

   return;

   ProtoComm::TestMsg* tx = new ProtoComm::TestMsg;
   tx->mCode1=101;

   sendMsg(tx);
}

//******************************************************************************
// This sends a message via the tcp client thread

void NetworkThread::sendMsg (ProtoComm::BaseMsg* aTxMsg)
{
   mTxSocket.doSendMsg(aTxMsg);
}

//******************************************************************************
// This sends a test message via the tcp client thread

void NetworkThread::sendTestMsg()
{
   ProtoComm::TestMsg* msg = new ProtoComm::TestMsg;
   msg->mCode1=201;
 
   mTxSocket.doSendMsg(msg);
}

}//namespace