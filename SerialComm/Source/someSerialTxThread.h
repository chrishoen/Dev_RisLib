#ifndef _SOMESERIALTXTHREAD_H_
#define _SOMESERIALTXTHREAD_H_

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

class SerialTxThread : public Ris::Threads::BaseThreadWithTermFlag
{
public:
   typedef Ris::Threads::BaseThreadWithTermFlag BaseClass;

   SerialTxThread();

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

   // aSensorIdent            is the sensor identifier
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
       
#ifdef _SOMESERIALTXTHREAD_CPP_
          SerialTxThread* gSerialTxThread;
#else
   extern SerialTxThread* gSerialTxThread;
#endif

//******************************************************************************
}//namespace
#endif

