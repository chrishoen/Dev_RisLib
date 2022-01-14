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
// This is a thread contains a serial port. It initializes and configures
// the port based on a parms file and then loops on port receives.

class SerialThread : public Ris::Threads::BaseThread
{
public:
   typedef Ris::Threads::BaseThread BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants.

   // Device path for usb acm.
   static const int cBufferSize = 1024*1024;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

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

   // Status.
   int mErrorCount;
   int mRestartCount;
   int mRxCount;
   int mTxCount;

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
   // serial port reads and then processes them.
   void threadRunFunction() override;

   // Thread exit function. This is called by the base class immediately
   // before the thread is terminated. It is close the serial port.
   void threadExitFunction() override;

   // Thread shutdown function. This aborts the serial port receive and
   // waits for the thread to terminate..
   void shutdownThread() override;

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

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Abort a pending receive.
   void abort();

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

