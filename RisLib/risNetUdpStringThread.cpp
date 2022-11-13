/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "prnPrint.h"
#include "trcTrace.h"

#include "risThreadsPriorities.h"
#include "risNetUdpStringThread.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

UdpStringThread::UdpStringThread(Settings& aSettings)
{
   // Base class variables.
   BaseClass::setThreadName("UdpString");
   BaseClass::setThreadPriority(aSettings.mThreadPriority);

   // Store settings.
   mSettings = aSettings;
   mRxStringQCall = aSettings.mRxStringQCall;

   // Initialize variables.
   mTI = mSettings.mTraceIndex;
   mConnectionFlag = false;
   mErrorCount = 0;
   mRestartCount = 0;
   mRxCount = 0;
   mTxCount = 0;

}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immediately 
// after the thread starts running. It configures the socket.

void UdpStringThread::threadInitFunction()
{
   Trc::write(mTI, 1, "UdpStringThread::threadInitFunction");
   Trc::write(mTI, 1, "UdpStringThread::threadInitFunction done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function. This is called by the base class immediately
// after the thread init function. It runs a loop that blocks on 
// the udp port receives and then processes them. The loop terminates
// when the serial port receive is aborted.

void UdpStringThread::threadRunFunction()
{
   Trc::write(mTI, 0, "UdpStringThread::threadRunFunction");

   // Top of the loop.
   mRestartCount = 0;
   mConnectionFlag = false;

restart:
   // Guard.
   if (mTerminateFlag) return;
   int tRet = 0;

   // Sleep.
   if (mRestartCount > 0)
   {
      BaseClass::threadSleep(1000);
   }
   Trc::write(mTI, 0, "UdpStringThread restart %d", mRestartCount);
   Prn::print(Prn::Show1, "UdpStringThread restart %d", mRestartCount);
   mRestartCount++;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Open device.

   // If the socket is open then close it.
   if (mStringSocket.mValidFlag)
   {
      mStringSocket.doClose();
   }

   // Initialize and configure the message socket.
   mStringSocket.initialize(mSettings);
   mStringSocket.configure();
   if (!mStringSocket.mValidFlag)
   {
      // If error then restart.
      goto restart;
   }

   // Connection was established.
   mConnectionFlag = true;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Loop to receive strings.

   while (!BaseClass::mTerminateFlag)
   {
      //************************************************************************
      //************************************************************************
      //************************************************************************
      // Receive.

      // Try to receive a message with a blocking receive call. If a message
      // was received then process it.
      if (mStringSocket.doRecvString())
      {
         // Message was correctly received.
         // Call the receive callback qcall.
         processRxString(new std::string(mStringSocket.mRxString));
      }
      else
      {
         Trc::write(mTI, 1, "UdpStringThread::threadRunFunction recv String ERROR");

         // Check for terminate.
         if (BaseClass::mTerminateFlag)
         {
            Trc::write(mTI, 0, "UdpStringThread read TERMINATE");
            goto end;
         }
         else
         {
            mStringSocket.mValidFlag = false;
            goto restart;
         }
      }
   }

   // Done.
end:
   Trc::write(mTI, 0, "UdpStringThread::threadRunFunction done");
   return;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immediately
// before the thread is terminated.

void UdpStringThread::threadExitFunction()
{
   Trc::write(mTI, 1, "UdpStringThread::threadExitFunction");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread shutdown function. This is called in the context of the parent
// thread. Set the termination flag, close the socket and wait for the
// thread to terminate.
//
// If the while loop in the threadRunFunction is blocked on doReceiveString then
// closing the socket will cause doReceiveString to return with false and 
// then the terminate request flag will be polled and the threadRunFunction 
// will exit.

void UdpStringThread::shutdownThread()
{
   Trc::write(mTI, 1, "UdpStringThread::shutdownThread");
   BaseThread::mTerminateFlag = true;

   mStringSocket.doClose();

   BaseThread::waitForThreadTerminate();
   Trc::write(mTI, 1, "UdpStringThread::shutdownThread done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Pass a received string to the parent thread. This is called by the
// threadRunFunction when a string is received. It invokes the
// mRxStringQCall that is registered at initialization.

void UdpStringThread::processRxString(std::string* aString)
{
   // Guard.
   if (!mRxStringQCall.isValid()) return;

   //printf("UdpRxStringThread processRxString call qcall\n");

   // Invoke the receive callback qcall.
   mRxStringQCall(aString);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a transmit string through the socket to the peer. It executes a
// blocking send call in the context of the calling thread. Delete the
// string when done.

void UdpStringThread::sendString (std::string* aString)
{

   // Send the string.
   mStringSocket.doSendString(aString->c_str());
   delete aString;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a transmit string through the socket to the peer. It executes a
// blocking send call in the context of the calling thread. Delete the
// string when done.

void UdpStringThread::sendString(const char* aString)
{
   // Send the string.
   mStringSocket.doSendString(aString);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

