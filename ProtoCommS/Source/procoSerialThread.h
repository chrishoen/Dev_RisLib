#pragma once

/*==============================================================================
ProtoComm serial thread.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risSerialMsgThread.h"
#include "risThreadsQCallThread.h"

#include "procoMsg.h"

namespace ProtoComm
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a qcall thread that obtains serial communications via a serial 
// message thread member variable.

class  SerialThread : public Ris::Threads::BaseQCallThread
{
public:
   typedef Ris::Threads::BaseQCallThread BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Serial message thread. This manages serial communications and 
   // message transmission and reception
   Ris::SerialMsgThread*  mSerialMsgThread;

   // Message monkey creator used by the serial thread.
   ProtoComm::MsgMonkeyCreator mMonkeyCreator;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Metrics.
   int  mStatusCount1;
   int  mStatusCount2;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Infrastructure.

   SerialThread();
  ~SerialThread();
   void configure();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Thread base class overloads.

   // launch starts the child thread + this thread
   // threadExitFunction shuts down the child thread
   // executeOnTimer sends a periodic status message.
   void launchThread();
   void threadExitFunction();
   void executeOnTimer(int);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // QCalls registered to mUdpMsgThread
   Ris::SerialMsgThread::RxMsgQCall    mRxMsgQCall;

   // Associated QCall methods, these are called by the
   // threadRunFunction to process conditions sent from 
   // mTcpServerThread.
   void executeRxMessage   (Ris::ByteContent* aMsg);

   // Rx message handlers
   void processRxMsg (ProtoComm::TestMsg*  aMsg);
   void processRxMsg (ProtoComm::StatusRequestMsg* aMsg);
   void processRxMsg (ProtoComm::StatusResponseMsg* aMsg);
   void processRxMsg (ProtoComm::DataMsg* aMsg);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Send a message.
   void sendMsg (ProtoComm::BaseMsg* aMsg);
   void sendTestMsg();   

};
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global singular instance.

#ifdef _PROCOSERIALTHREAD_CPP_
         SerialThread* gSerialThread;
#else
extern   SerialThread* gSerialThread;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
