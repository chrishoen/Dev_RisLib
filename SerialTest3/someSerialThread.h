#pragma once

/*==============================================================================
Serial test thread that contains a serial port.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risSerialPort2.h"
#include "risThreadsThreads.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a thread that uses a serial port. It initializes and configures
// the port based on a parms file and then loops on port receives.

class SerialThread : public Ris::Threads::BaseThread
{
public:
   typedef Ris::Threads::BaseThread BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants.

   // Buffers.
   static const int cBufferSize = 2*1024*1024;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Serial port settings.
   Ris::SerialSettings mSettings;

   // Serial port.
   Ris::SerialPort2 mSerialPort;

   // Buffers.
   char mRxBuffer[cBufferSize];
   char mTxBuffer[cBufferSize];

   // Metrics.
   int mErrorCount;
   int mRestartCount;
   int mRxCount;
   int mTxCount;

   // Requested number of bytes to receive.
   int mRxReqNumBytes;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   SerialThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Base class overloads.

   // Thread init function. This is called by the base class immediately
   // after the thread starts running. It initializes the serial port.
   void threadInitFunction() override;

   // Thread run function. This is called by the base class immediately
   // after the thread init function. It runs a loop that blocks on 
   // serial port receives and then processes them. The loop terminates
   // when the serial port receive is aborted.
   void threadRunFunction() override;

   // Thread exit function. This is called by the base class immediately
   // before the thread is terminated. It is close the serial port.
   void threadExitFunction() override;

   // Thread shutdown function. It is called out of the context of
   // this thread. It aborts the serial port receive and waits for the
   // thread to terminate after execution of the thread exit function.
   void shutdownThread() override;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Abort a pending receive.
   void abort();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Send bytes via the serial port. This executes in the context of
   // the calling thread.
   void sendBytes(const void* aBytes, int aNumBytes);

   // Send test bytes via the serial port. This executes in the context
   // of the calling thread.
   void sendTestBytes(int aNumBytes);

};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global instance.

#ifdef _SOMESERIALTHREAD_CPP_
SerialThread* gSerialThread = 0;
#else
extern  SerialThread* gSerialThread;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

