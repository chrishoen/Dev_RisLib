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
   // Initialize base class members.
   BaseClass::setThreadPriorityHigh();

   // Initialize member variables.
   mCount=0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function, base class overload.
// It configures the serial port.

void SerialRxThread::threadInitFunction()
{
   Prn::print(Prn::ThreadInit1, "SerialRxThread::threadInitFunction");

   // Serial port settings.
   mSettings.reset();
   mSettings.setPortDevice("COM6");
   mSettings.setPortSetup("9600,N,8,1");
   mSettings.mRxTimeout = 2000;

   // Initialize and open the serial port.
   mSerialPort.initialize(mSettings);
   mSerialPort.doOpen();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function, base class overload.
// Receive a string from the serial port and print it.

void  SerialRxThread::threadRunFunction()
{
   while(true)
   {
      // Read a string from the serial port.
      char tString[100];
      int tStatus = mSerialPort.doReceiveLine(tString, 100);

      // If termination request, exit the loop.
      if (mTerminateFlag) break;

      // Test returned status and print the string.
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
//******************************************************************************
//******************************************************************************
// Thread exit function, base class overload.

void SerialRxThread::threadExitFunction()
{
   Prn::print(Prn::ThreadInit1, "SerialRxThread::threadExitFunction");
}

//******************************************************************************
//******************************************************************************
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
//******************************************************************************
//******************************************************************************
}//namespace

