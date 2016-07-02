#ifndef _SOMESERIALTXTHREAD_H_
#define _SOMESERIALTXTHREAD_H_

/*==============================================================================

Sensor receiver thread classes.

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsTimerThread.h"
#include "risSerialPort.h"


namespace Some
{

//******************************************************************************
// Sensor receiver thread class.
//

class SerialTxThread : public Ris::Threads::BaseTimerThread
{
public:
   typedef Ris::Threads::BaseTimerThread BaseClass;

   SerialTxThread();

   //---------------------------------------------------------------------------
   // Thread base class overloads:

   // threadInitFunction opens the serial port.
   // threadRunFunction does a while loop that does reads from the serial port
   // threadExitFunction closes the serial port.

   void threadInitFunction(); 
   void executeOnTimer(int aTimeCount);
   void threadExitFunction(); 

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
   bool mTPFlag;
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

