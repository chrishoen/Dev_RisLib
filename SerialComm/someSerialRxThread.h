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

class SerialRxThread : public Ris::Threads::BaseThread
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
   SerialRxThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Thread base class overloads.

   // threadInitFunction opens the serial port.
   // threadRunFunction does a while loop that does reads from the serial port
   // threadExitFunction closes the serial port.
   void threadInitFunction(); 
   void threadRunFunction(); 
   void threadExitFunction(); 
   void shutdownThread(); 
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global singular instance.
       
#ifdef _SOMESERIALRXTHREAD_CPP_
          SerialRxThread* gSerialRxThread;
#else
   extern SerialRxThread* gSerialRxThread;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

