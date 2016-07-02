/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risProgramTime.h"
#include "prnPrint.h"

#define  _SOMESERIALTXTHREAD_CPP_
#include "someSerialTxThread.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

SerialTxThread::SerialTxThread()
{
   //---------------------------------------------------------------------------
   // Initialize base class members.

   BaseClass::setThreadPriorityHigh();

   //---------------------------------------------------------------------------
   // Defaults.

   mPortNumber = 0;
   mPortSetup[0]=0;
   mCount=0;
}

//******************************************************************************
// Configure:

void SerialTxThread::configure(
   int             aPortNumber,
   char*           aPortSetup)
{
   // Store configuration parameters.
   mPortNumber = aPortNumber;
   strcpy(mPortSetup,aPortSetup);
}

//******************************************************************************
// Thread init function, base class overload.
// It configures the serial port.

void SerialTxThread::threadInitFunction()
{
   Prn::print(Prn::ThreadInit1, "SerialTxThread::threadInitFunction");

   mSerialPort.doOpen(mPortNumber,mPortSetup);
   
}

//******************************************************************************
// Thread exit function, base class overload.

void SerialTxThread::threadExitFunction()
{
   Prn::print(Prn::ThreadInit1, "SerialTxThread::threadExitFunction");
}

//******************************************************************************
// Shutdown, base class overload.
// This sets the terminate request flag and closes the socket.
//
// If the while loop in the threadRunFunction is blocked on doReceiveMessage then
// closing the socket will cause doReceiveMessage to return with false and 
// then the terminate request flag will be polled and the threadRunFunction 
// will exit.

void SerialTxThread::shutdownThread()
{
   BaseThreadWithTermFlag::mTerminateFlag = true;

   mSerialPort.doClose();

   BaseThreadWithTermFlag::waitForThreadTerminate();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function, base class overload.
// It receives serial data from the sensor and passes it to the owner thread
// via the qcall.

void  SerialTxThread::threadRunFunction()
{
   //---------------------------------------------------------------------------
   // Loop

   while(true)
   {
      //------------------------------------------------------------------------
      // If termination request, exit the loop.
      if (mTerminateFlag) break;

      //------------------------------------------------------------------------
      // Read sample data from sensor via the serial port and send to consumer.
      BaseClass::threadSleep(1000);

      char tString[100];
      sprintf(tString,"ABCDEFGH\r");

      int tStatus = mSerialPort.doSend(tString);
      if (tStatus < 0)
      {
         Prn::print(Prn::ThreadRun1,"SerialPort doSendOne ERROR %d",tStatus);
      }
    }         
}

//******************************************************************************

}//namespace

