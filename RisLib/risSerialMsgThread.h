#pragma once

/*==============================================================================
Serial port message thread.
==========================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsThreads.h"
#include "risThreadsQCall.h"
#include "risThreadsSynch.h"
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
// The data that is communicated via the serial port is encapsulated according
// to the byte content messaging scheme. It sends and receives byte content
// messages.
//
// The thread is structured around a while loop that does a read call to
// receive a message on the serial port.
//
// The thread provides serialized access to the serial port and associated 
// state variables and it provides the context for the blocking of the 
// read call.
//
// An instance of this thread is created as a child thread of a parent thread
// that performs message processing. The parent creates the child and
// registers a receive message qcall callback to it. When the child thread
// receives a message it invokes the message qcall to pass it to the parent
// for processing.

class SerialMsgThread : public Ris::Threads::BaseThread
{
private:
   typedef Ris::Threads::BaseThread BaseClass;
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Serial settings
   SerialSettings mSettings;

   // Serial message port.
   SerialMsgPort mSerialMsgPort;

   // Program trace index.
   int mTI;

   // If true then the serial port is open. If false then it is closed
   // because of an error.
   bool mConnectionFlag;

   // If true then the last serial port abort was a suspension request.
   bool mSuspendReq;

   // If true then serial port is closed and sends and receives are 
   // suspended.
   bool mSuspendFlag;

   // If suspended then the thread blocks on this semaphore.
   Ris::Threads::BinarySemaphore mResumeSem;

   // This is a qcall that is invoked when a session is established or
   // disestablished. It is registered by the parent thread at
   // initialzation. It is called when the serial port is disconnected
   // because of an error and when it is reconnected.
   typedef Ris::Threads::QCall1<bool> SessionQCall;
   SessionQCall mSessionQCall;

   // This is a qcall callback that is called when a message is received.
   // It is registered by the parent thread at initialzation.
   typedef Ris::Threads::QCall1<Ris::ByteContent*> RxMsgQCall;
   RxMsgQCall mRxMsgQCall;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Metrics.
   int mErrorCount;
   int mRestartCount;
   int mRxCount;
   int mTxCount;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   SerialMsgThread(SerialSettings& aSettings);
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

   // Notify the parent thread that a session has changed. This is called by
   // the threadRunFunction when a new session is established or an existing
   // session is disestablished. It invokes the mSessionQCall that is
   // registered at initialization. The session is disestablished if the 
   // serial port is closed because of an error and it is established if
   // it is successfully reopened.
   virtual void processSessionChange(bool aEstablished);

   // Pass a received message to the parent thread. This is called by the
   // threadRunFunction when a message is received. It invokes the
   // mRxMsgQCall that is registered at initialization.
   virtual void processRxMsg(Ris::ByteContent* aMsg);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Send a message via the serial port. Return the number of bytes
   // transferred or a negative error code.
   int doSendMsg (Ris::ByteContent* aMsg);

   // Abort the serial message port blocked receive call.
   void doAbort();

   // Set the suspension request flag and abort the pending receive. This
   // will cause the thread to close the serial port and then block on the
   // resume semaphore.
   void doSuspend();

   // Post to the resume semaphore to wake up the thread and enter the 
   // restart loop.
   void doResume();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

