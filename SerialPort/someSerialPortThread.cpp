/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"


#include "risProgramTime.h"
#include "someSerialParms.h"

#define  _SOMESERIALPORTTHREAD_CPP_
#include "someSerialPortThread.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

SerialPortThread::SerialPortThread()
{
   // Initialize base class members.
   BaseClass::setThreadPriorityHigh();
   BaseClass::setThreadName("Serial");

   // Initialize member variables.
   mCount=0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function, base class overload.
// It configures the serial port.

void SerialPortThread::threadInitFunction()
{
   Prn::print(Prn::View14, "SerialPortThread::threadInitFunction");

   // Serial port settings.
   mSettings.reset();
   mSettings.setPortDevice(gSerialParms.mSerialPortDevice);
   mSettings.setPortSetup(gSerialParms.mSerialPortDevice);
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

void  SerialPortThread::threadRunFunction()
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
         Prn::print(Prn::View11,"SerialPortThread receive $$ %d  %d$ %s",mCount++,tStatus,tString);
      }
      else if (tStatus == Ris::SerialPort::cRetCodeTimeout)
      {
         Prn::print(Prn::View11,"SerialPortThread receive TIMEOUT %d",tStatus);
      }
      else
      {
         Prn::print(Prn::View11,"SerialPortThread receive ERROR %d",tStatus);
      }
   }         
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function, base class overload.

void SerialPortThread::threadExitFunction()
{
   Prn::print(Prn::View14, "SerialPortThread::threadExitFunction");
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

void SerialPortThread::shutdownThread()
{
   BaseThread::mTerminateFlag = true;

   mSerialPort.doClose();

   BaseThread::waitForThreadTerminate();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a null terminated string via the serial port. A newline terminator
// is appended to the string before transmission. This executes in the
// context of the calling thread.

void SerialPortThread::sendString(const char* aString)
{
   mSerialPort.doSendLine(aString);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

