/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "risProgramTime.h"

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
   mRxTimeout=0;
   mCount=0;
}

//******************************************************************************
// Configure:

void SerialRxThread::configure(
   int     aPortNumber,
   char*   aPortSetup,
   int     aRxTimeout)
{
   // Store configuration parameters.
   mPortNumber = aPortNumber;
   strcpy(mPortSetup,aPortSetup);
   mRxTimeout = aRxTimeout;
}

//******************************************************************************
// Thread init function, base class overload.
// It configures the serial port.

void SerialRxThread::threadInitFunction()
{
   Prn::print(Prn::ThreadInit1, "SerialRxThread::threadInitFunction");

   mSerialPort.doOpen(mPortNumber,mPortSetup,mRxTimeout);
   
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
      //------------------------------------------------------------------------
      // Read sample data from sensor via the serial port and send to consumer.

      char tString[100];
      int tStatus = mSerialPort.doReceiveUntilCR(tString,100);

      //------------------------------------------------------------------------
      // If termination request, exit the loop.
      if (mTerminateFlag) break;

      //------------------------------------------------------------------------
      // Test returned status.

      if (tStatus >= 0)
      {
         Prn::print(Prn::ThreadRun1,"SerialRxThread receive $$ %d  %d$ %s",mCount++,tStatus,tString);
      }
      else if (tStatus == Ris::SerialPort::cRetCodeTimeout)
      {
         Prn::print(Prn::ThreadRun1,"SerialRxThread receive TIMEOUT %d",tStatus);
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
// If the while loop in the threadRunFunction is blocked on doReceiveMsg then
// closing the socket will cause doReceiveMsg to return with false and 
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

