#pragma once

/*==============================================================================
Serial bluetooth monitor thread.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsThreads.h"
#include "risSerialPort.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This thread is used to monitor serial bluetooth. If it detects that 
// the serial port bluetooth has been disconnected then it resets the
// serial port.

class SerialBthThread : public Ris::Threads::BaseThread
{
private:
   typedef Ris::Threads::BaseThread BaseClass;
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.
   
   // Serial port to which the bluetooth is monitored
   Ris::SerialPort* mSerialPort;

   // Trace index.
   int mTI;

   // Portable specifics.
   class Specific;
   Specific* mSpecific;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   SerialBthThread(Ris::SerialPort* mSerialPort);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Base class overloads.

   // Thread init function. This is called by the base class immedidately 
   // after the thread starts running.
   void threadInitFunction() override;

   // Thread run function. This is called by the base class immediately 
   // after the thread init function. It runs a loop that monitors the
   // bluetooth. If it detects an error then it resets the serial port.
   void threadRunFunction() override;

   // Thread exit function. This is called by the base class immedidately
   // before the thread is terminated.
   void threadExitFunction() override;

   // Thread shutdown function. Set the termination flag, post to the 
   // waitable event and wait for the thread to terminate.
   void shutdownThread() override;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace


