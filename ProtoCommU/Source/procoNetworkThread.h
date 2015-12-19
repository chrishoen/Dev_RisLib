#ifndef _PROCONETWORKTHREAD_H_
#define _PROCONETWORKTHREAD_H_

/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risContainers.h"
#include "risTimeMarker.h"
#include "risNetUdpMsgThread.h"
#include "risNetUdpMsgSocket.h"
#include "risThreadsQCallThread.h"

#include "procoMsg.h"

namespace ProtoComm
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

   Ris::Net::UdpMsgThread*  mUdpMsgThread;

   // Message parser used by mUdpMsgThread
   ProtoComm::MessageParserCreator mMessageParserCreator;

   //--------------------------------------------------------------
   // QCall:

   // QCalls registered to mUdpMsgThread
   Ris::Net::UdpMsgThread::RxMsgQCall    mRxMsgQCall;

   // Associated QCall methods, these are called by the
   // threadRunFunction to process conditions sent from 
   // mTcpServerThread.
   void executeRxMsg   (Ris::ByteContent* aRxMsg);

   //--------------------------------------------------------------
   //--------------------------------------------------------------
   //--------------------------------------------------------------
   // Rx message handlers

   void processRxMsg (ProtoComm::TestMsg*  aRxMsg);
   void processRxMsg (ProtoComm::StatusRequestMsg* aRxMsg);
   void processRxMsg (ProtoComm::StatusResponseMsg* aRxMsg);
   void processRxMsg (ProtoComm::DataMsg* aRxMsg);

   int  mStatusCount1;
   int  mStatusCount2;

   //--------------------------------------------------------------
   // Send a message

   Ris::Net::UdpTxMsgSocket  mTxSocket;

   void sendMsg (ProtoComm::BaseMsg* aTxMsg);
   void sendTestMsg();   

   //--------------------------------------------------------------
   // State:

   Ris::TimeMarker mTimeMarker;
};
//******************************************************************************
// Global instance

#ifdef _PROCONETWORKTHREAD_CPP_
         NetworkThread* gNetworkThread;
#else
extern   NetworkThread* gNetworkThread;
#endif

//******************************************************************************
}//namespace


#endif

