#pragma once

/*==============================================================================
Serial port message thread.
==========================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsThreads.h"
#include "risThreadsQCall.h"
#include "risByteContent.h"
#include "risByteMsgMonkey.h"
#include "risSerialMsgPort.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Serial message thread.
//
// This is a thread that provides the execution context for byte content
// message communication via a serial port.
//
// It contains a serial message port.
//
// The thread is structured around a while loop that does a read call to
// receive a message on the serial port.
//
// The thread provides serialized access to the serial port and associated 
// state variables and it provides the context for the blocking of the 
// read call.

class SerialMsgThread : public Ris::Threads::BaseThreadWithTermFlag
{
public:
   typedef Ris::Threads::BaseThreadWithTermFlag BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Serial settings
   SerialSettings mSettings;

   // Serial message port.
   SerialMsgPort mSerialMsgPort;

   // Transmit mutex is used by doSendMsg for mutual exclusion.
   Threads::MutexSemaphore  mTxMutex;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // This is a qcall callback that is called when a message is received.
   typedef Ris::Threads::QCall1<Ris::ByteContent*> RxMsgQCall;

   // This is a qcall callback that is called when a message is received.
   RxMsgQCall mRxMsgQCall;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Metrics.
   int mTxCount;
   int mTxLength;
   int mRxCount;
   int mRxError;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   SerialMsgThread(SerialSettings aSettings);
   ~SerialMsgThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods, thread base class overloads.

   // Initialize and open the serial port.
   void threadInitFunction()override;

   // Execute a while loop that does receive calls. The loop exits
   // when the serial port is closed and the termination flag is true.
   void threadRunFunction()override;

   // Print.
   void threadExitFunction()override;

   // Set the termination flag, close the serial port and wait for the thread
   // to terminate.
   void shutdownThread()override; 

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Send a transmit message via the serial port. It executes a blocking
   // send call in the context of the caller.
   bool sendMsg (Ris::ByteContent* aMsg);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

