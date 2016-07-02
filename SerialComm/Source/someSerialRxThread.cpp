/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risProgramTime.h"
#include "prnPrint.h"

#define  _SOMESERIALRXTHREAD_CPP_
#include "someSerialRxThread.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

SerialRxThread::SerialRxThread()
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

void SerialRxThread::configure(
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

void SerialRxThread::threadInitFunction()
{
   Prn::print(Prn::ThreadInit1, "SerialRxThread::threadInitFunction");

   mSerialPort.doOpen(mPortNumber,mPortSetup);
   
}

//******************************************************************************
// Thread run function, base class overload.
// It receives serial data from the sensor and passes it to the owner thread
// via the qcall.

void  SerialRxThread::threadRunFunction()
{
   //---------------------------------------------------------------------------
   // Loop

   while(true)
   {
      //---------------------------------------------------------------------------
      // If termination request, exit the loop.
      if (mTerminateFlag) break;

      //---------------------------------------------------------------------------
      // Read sample data from sensor via the serial port and send to consumer.

      char tString[100];
      int tStatus = mSerialPort.doReceiveUntilCR(tString,100,0);


      if (tStatus >= 0)
      {
         Prn::print(Prn::ThreadRun1,"SerialRxThread receive $$ %d $$ %s",tStatus,tString);
      }
      else
      {
         Prn::print(Prn::ThreadRun1,"SerialRxThread receive ERROR %d",tStatus);
      }
   }         
}

//******************************************************************************
// Thread exit function, base class overload.

void SerialRxThread::threadExitFunction()
{
   Prn::print(Prn::ThreadInit1, "SerialRxThread::threadExitFunction");
}

//******************************************************************************
// Shutdown, base class overload.
// This sets the terminate request flag and closes the socket.
//
// If the while loop in the threadRunFunction is blocked on doReceiveMessage then
// closing the socket will cause doReceiveMessage to return with false and 
// then the terminate request flag will be polled and the threadRunFunction 
// will exit.

void SerialRxThread::shutdownThread()
{
   BaseThreadWithTermFlag::mTerminateFlag = true;

   mSerialPort.doClose();

   BaseThreadWithTermFlag::waitForThreadTerminate();
}

//******************************************************************************

}//namespace

