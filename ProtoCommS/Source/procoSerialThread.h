#ifndef _PROCONETWORKTHREAD_H_
#define _PROCONETWORKTHREAD_H_

/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risContainers.h"
#include "risNetUdpMsgThread.h"
#include "risNetUdpMsgSocket.h"
#include "risThreadsQCallThread.h"

#include "procoMsg.h"

namespace ProtoComm
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

class  SerialThread : public Ris::Threads::BaseQCallThread
{
public:
   typedef Ris::Threads::BaseQCallThread BaseClass;

   SerialThread();
  ~SerialThread();

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

   Ris::Net::UdpMsgThread*  mUdpMsgThread;

   // Message monkey used by mUdpMsgThread
   ProtoComm::MsgMonkeyCreator mMonkeyCreator;

   //--------------------------------------------------------------
   // QCall:

   // QCalls registered to mUdpMsgThread
   Ris::Net::UdpMsgThread::RxMsgQCall    mRxMsgQCall;

   // Associated QCall methods, these are called by the
   // threadRunFunction to process conditions sent from 
   // mTcpServerThread.
   void executeRxMessage   (Ris::ByteContent* aMsg);

   //--------------------------------------------------------------
   //--------------------------------------------------------------
   //--------------------------------------------------------------
   // Rx message handlers

   void processRxMsg (ProtoComm::TestMsg*  aMsg);
   void processRxMsg (ProtoComm::StatusRequestMsg* aMsg);
   void processRxMsg (ProtoComm::StatusResponseMsg* aMsg);
   void processRxMsg (ProtoComm::DataMsg* aMsg);

   int  mStatusCount1;
   int  mStatusCount2;

   //--------------------------------------------------------------
   // Send a message

   void sendMsg (ProtoComm::BaseMsg* aMsg);
   void sendTestMsg();   

};
//******************************************************************************
// Global instance

#ifdef _PROCONETWORKTHREAD_CPP_
         SerialThread* gSerialThread;
#else
extern   SerialThread* gSerialThread;
#endif

//******************************************************************************
}//namespace


#endif

