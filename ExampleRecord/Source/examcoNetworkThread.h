#ifndef _EXAMCONETWORKTHREAD_H_
#define _EXAMCONETWORKTHREAD_H_

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risContainers.h"
#include "risTimeMarker.h"
#include "risNetUdpRecordSocket.h"
#include "risNetUdpRecordThread.h"
#include "risThreadsQCallThread.h"

#include "examcoRecord.h"

namespace ExampleComm
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

   Ris::Net::UdpRecordThread*  mUdpRecordThread;

   // Message parser used by mUdpRecordThread
   RecordCopier mRecordCopier;

   //--------------------------------------------------------------
   // QCall:

   // QCalls registered to mUdpRecordThread
   Ris::Net::UdpRecordThread::RxRecordQCall    mRxRecordQCall;

   // Associated QCall methods, these are called by the
   // threadRunFunction to process conditions sent from 
   // mTcpServerThread.
   void executeRxRecord   (Ris::ByteRecord* aRxRecord);

   //--------------------------------------------------------------
   //--------------------------------------------------------------
   //--------------------------------------------------------------
   // Rx message handlers

   void processRxRecord (TestRecord*  aRecord);
   void processRxRecord (StatusRecord* aRecord);
   void processRxRecord (Data1Record* aRecord);
   void processRxRecord (Data2Record* aRecord);

   int  mStatusCount1;
   int  mStatusCount2;

   //--------------------------------------------------------------
   // Send a record

   void sendRecord (Ris::ByteRecord* aRecord);
   void sendTestRecord();   

};

//******************************************************************************
// Global instance

#ifdef _EXAMCONETWORKTHREAD_CPP_
         NetworkThread* gNetworkThread;
#else
extern   NetworkThread* gNetworkThread;
#endif

//******************************************************************************
}//namespace


#endif
