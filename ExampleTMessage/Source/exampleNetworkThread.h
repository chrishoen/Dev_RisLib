#ifndef _EXAMPLENETWORKTHREAD_H_
#define _EXAMPLENETWORKTHREAD_H_

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risContainers.h"
#include "risTimeMarker.h"
#include "risNetUdpTMessageSocket.h"
#include "risNetUdpTMessageThread.h"
#include "risThreadsQCallThread.h"

#include "exampleTMessage.h"

namespace Example
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

   Ris::Net::UdpTMessageThread*  mUdpTMessageThread;

   // Message parser used by mUdpTMessageThread
   TMessageCopier mMsgCopier;

   //--------------------------------------------------------------
   // QCall:

   // QCalls registered to mUdpTMessageThread
   Ris::Net::UdpTMessageThread::RxMessageQCall    mRxMessageQCall;

   // Associated QCall methods, these are called by the
   // threadRunFunction to process conditions sent from 
   // mTcpServerThread.
   void executeRxMessage   (Ris::ByteTMessage* aMsg);

   //--------------------------------------------------------------
   //--------------------------------------------------------------
   //--------------------------------------------------------------
   // Rx message handlers

   void processRxMessage (TestMsg*   aMsg);
   void processRxMessage (StatusMsg* aMsg);
   void processRxMessage (Data1Msg*  aMsg);
   void processRxMessage (Data2Msg*  aMsg);

   int  mStatusCount1;
   int  mStatusCount2;

   //--------------------------------------------------------------
   // Send a record

   void sendMessage (Ris::ByteTMessage* aMsg);
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

