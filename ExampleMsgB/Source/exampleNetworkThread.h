#ifndef _EXAMPLENETWORKTHREAD_H_
#define _EXAMPLENETWORKTHREAD_H_

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risContainers.h"
#include "risNetUdpMsgBSocket.h"
#include "risNetUdpMsgBThread.h"
#include "risThreadsQCallThread.h"

#include "exampleMsgTraits.h"
#include "exampleMsg.h"

namespace ExampleMsg
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

class  NetworkThread : public Ris::Threads::BaseQCallThread
{
public:
   typedef Ris::Threads::BaseQCallThread BaseClass;

   NetworkThread();
  ~NetworkThread();

   //--------------------------------------------------------------
   // Configure:

   void configure();

   //--------------------------------------------------------------
   // Thread base class overloads:

   // launch starts the child thread + this thread
   // threadExitFunction shuts down the child thread
   // executeOnTimer sends a periodic status message.
   void launchThread();
   void threadExitFunction(); 
   void executeOnTimer(int);

   //--------------------------------------------------------------
   // Tcp client thread, this manages session connections and 
   // message transmission and reception

// typedef Ris::Net::UdpMsgBThread<ExampleMsg::MsgTraits> UdpMsgThread;
   typedef Ris::Net::UdpMsgBThread UdpMsgThread;
   UdpMsgThread*  mUdpMsgThread;

   //--------------------------------------------------------------
   // QCall:

   // QCalls registered to mUdpMsgBThread
   UdpMsgThread::RxMsgQCall    mRxMsgQCall;

   // Associated QCall methods, these are called by the
   // threadRunFunction to process conditions sent from 
   // mTcpServerThread.
   void executeRxMessage (Ris::ByteMsgB* aMsg);

   //--------------------------------------------------------------
   //--------------------------------------------------------------
   //--------------------------------------------------------------
   // Rx message handlers

   void processRxMsg (TestMsg*   aMsg);
   void processRxMsg (StatusMsg* aMsg);
   void processRxMsg (Data1Msg*  aMsg);
   void processRxMsg (Data2Msg*  aMsg);
   void processRxMsg (Data3Msg*  aMsg);
   void processRxMsg (Data4Msg*  aMsg);

   int  mStatusCount1;
   int  mStatusCount2;

   //--------------------------------------------------------------
   // Send a record

   void sendMsg (Ris::ByteMsgB* aMsg);
   void sendTestMsg();   
};

//******************************************************************************
// Global instance

#ifdef _EXAMPLENETWORKTHREAD_CPP_
         NetworkThread* gNetworkThread;
#else
extern   NetworkThread* gNetworkThread;
#endif

//******************************************************************************
}//namespace


#endif

