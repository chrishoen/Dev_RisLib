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

#if 1
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

#endif

#if 0
   if (aRole==1)
   {
      mUdpRxThread->configure("0.0.0.0",50131,mMessageParser,&mRxMsgQCall);
      mTxSocket.configure(Ris::Sockets::SocketAddress("192.168.240.30",50132),mMessageParser);
   }
   if (aRole==2)
   {
      mUdpRxThread->configure("0.0.0.0",50132,mMessageParser,&mRxMsgQCall);
      mTxSocket.configure(Ris::Sockets::SocketAddress("192.168.240.10",50131),mMessageParser);
   }
#endif
 
#if 0
   if (aRole==1)
   {
      mUdpRxThread->configure("0.0.0.0",50131,mMessageParser,&mRxMsgQCall);
      mTxSocket.configure(Ris::Sockets::SocketAddress("235.2.3.5",50132),mMessageParser);
   }
   if (aRole==2)
   {
      mUdpRxThread->configureForMulticast("235.2.3.5","0.0.0.0",50132,mMessageParser,&mRxMsgQCall);
      mTxSocket.configure(Ris::Sockets::SocketAddress("192.168.240.30",50132),mMessageParser);
   }
#endif

#if 0
   if (aRole==1)
   {
      mTxSocket.configure(Ris::Sockets::SocketAddress("236.2.3.6",50132),mMessageParser);
   }
   if (aRole==2)
   {
      mUdpRxThread->configure("236.2.3.6",50132,mMessageParser,&mRxMsgQCall);
   }
#endif
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
      case ProtoComm::MsgIdT::StatusResponse :
         processRxMsg((ProtoComm::StatusResponseMsg*)tRxMsg);
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

   delete aRxMsg;
}

//******************************************************************************
// Rx message handler - StatusResponseMsg

void NetworkThread::processRxMsg(ProtoComm::StatusResponseMsg* aRxMsg)
{
   Prn::print(Prn::ThreadRun,Prn::Run1, "NetworkThread::processRxMsg_StatusResponseMsg");
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