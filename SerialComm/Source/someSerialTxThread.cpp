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
   BaseClass::mTimerPeriod = 50;

   //---------------------------------------------------------------------------
   // Defaults.

   mPortNumber = 0;
   mPortSetup[0]=0;
   mCount=0;
   mTPFlag = true;
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

   mSerialPort.doClose();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function, base class overload.
// It receives serial data from the sensor and passes it to the owner thread
// via the qcall.

void SerialTxThread::executeOnTimer(int aTimeCount)
{
   if (!mTPFlag) return;

   char tString[100];
   sprintf(tString,"ABCDEFGH01234567\r");

   int tStatus = mSerialPort.doSend(tString);
   if (tStatus < 0)
   {
      Prn::print(Prn::ThreadRun1, "SerialPort doSend ERROR %d", tStatus);
   }
}

//******************************************************************************

}//namespace

