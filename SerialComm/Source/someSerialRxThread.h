#ifndef _SOMESERIALRXTHREAD_H_
#define _SOMESERIALRXTHREAD_H_

/*==============================================================================

Sensor receiver thread classes.

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsThreads.h"
#include "risSerialPort.h"

namespace Some
{

//******************************************************************************
// Sensor receiver thread class.
//

class SerialRxThread : public Ris::Threads::BaseThreadWithTermFlag
{
public:
   typedef Ris::Threads::BaseThreadWithTermFlag BaseClass;

   SerialRxThread();

   //---------------------------------------------------------------------------
   // Thread base class overloads:

   // threadInitFunction opens the serial port.
   // threadRunFunction does a while loop that does reads from the serial port
   // threadExitFunction closes the serial port.

   void threadInitFunction(); 
   void threadRunFunction(); 
   void threadExitFunction(); 
   void shutdownThread(); 

   //---------------------------------------------------------------------------
   // Configure:

   // PortNumber  is the com port number
   // PortSetup   is the com port setup, ie 9600N81
   // RxTimeout   is the com port receive timeout, zero means no timeout

   void configure(
      int    aPortNumber,
      char*  aPortSetup,
      int    aRxTimeout);

   //---------------------------------------------------------------------------
   // Stored configuration parameters:

   int       mPortNumber;
   char      mPortSetup[16];
   int       mRxTimeout;

   //---------------------------------------------------------------------------
   // Serial port:

   Ris::SerialPort mSerialPort;

   //---------------------------------------------------------------------------
   // Control variables:

   int mCount;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global instance
       
#ifdef _SOMESERIALRXTHREAD_CPP_
          SerialRxThread* gSerialRxThread;
#else
   extern SerialRxThread* gSerialRxThread;
#endif

}//namespace
#endif

