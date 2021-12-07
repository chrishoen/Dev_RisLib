#pragma once

/*==============================================================================
Sensor receiver thread classes.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsThreads.h"
#include "risSerialPort.h"
#include "risSerialSettings.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Sensor receiver thread class.

class SerialPortThread : public Ris::Threads::BaseThread
{
public:
   typedef Ris::Threads::BaseThread BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Serial port settings.
   Ris::SerialSettings mSettings;

   // Serial port.
   Ris::SerialPort mSerialPort;

   // Metrics.
   int mCount;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   SerialPortThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Base class overloads.

   // Thread init function. This is called by the base class immediately
   // after the thread starts running. It initializes something.
   void threadInitFunction() override;

   // Thread run function. This is called by the base class immediately
   // after the thread init function. It runs a loop that waits for the
   // hid keyboard input.
   void threadRunFunction() override;

   // Thread exit function. This is called by the base class immediately
   // before the thread is terminated. It is a placeholder.
   void threadExitFunction() override;

   // Thread shutdown function. This posts to the close event to
   // terminate the thread and it closes the files.
   void shutdownThread() override;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Send a null terminated string via the serial port. A newline terminator
   // is appended to the string before transmission. This executes in the
   // context of the calling thread.
   void sendString(const char* aString);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global singular instance.
       
#ifdef _SOMESERIALPORTTHREAD_CPP_
          SerialPortThread* gSerialPortThread;
#else
   extern SerialPortThread* gSerialPortThread;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

