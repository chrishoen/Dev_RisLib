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

   // aPortNumber             is the sensor com port number
   // aPortSetup              is the sensor com port setup, ie 9600N81

   void configure(
      int             aPortNumber,
      char*           aPortSetup);

   //---------------------------------------------------------------------------
   // Stored configuration parameters:

   int                mPortNumber;
   char               mPortSetup[16];

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

