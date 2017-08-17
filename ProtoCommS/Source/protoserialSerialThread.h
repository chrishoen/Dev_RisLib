#pragma once

/*==============================================================================
ProtoSerial serial thread.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risSerialMsgThread.h"
#include "risThreadsQCallThread.h"

#include "protoserialMsg.h"

namespace ProtoSerial
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
   ProtoSerial::MsgMonkeyCreator mMonkeyCreator;

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

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Thread base class overloads.

   // launch starts the child thread + this thread
   // threadExitFunction shuts down the child thread
   // executeOnTimer sends a periodic status message.
   void threadInitFunction();
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
   void processRxMsg (ProtoSerial::TestMsg*  aMsg);
   void processRxMsg (ProtoSerial::StatusRequestMsg* aMsg);
   void processRxMsg (ProtoSerial::StatusResponseMsg* aMsg);
   void processRxMsg (ProtoSerial::DataMsg* aMsg);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Send a message.
   void sendMsg (ProtoSerial::BaseMsg* aMsg);
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
