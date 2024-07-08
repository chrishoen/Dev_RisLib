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

   // Initialize the socket according to the settings.
   mStringSocket.initialize(&mSettings);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function. This is called by the base class immediately
// after the thread init function. It runs a loop that blocks on 
// udp port receives and then processes them. The loop terminates
// when the socket is closed by the shutdown call.

void UdpStringThread::threadRunFunction()
{
   Trc::write(mTI, 0, "UdpStringThread::threadRunFunction");

   // Top of the loop.
   mRestartCount = 0;

Restart:

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

   // Close the socket.
   mStringSocket.doClose();

   // Configure the socket.
   mStringSocket.configure();
   if (!mStringSocket.mValidFlag)
   {
      // If error then restart.
      goto Restart;
   }

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

      // Try to receive a string with a blocking receive call. If a string
      // was received then process it.
      if (mStringSocket.doRecvString())
      {
         // String was correctly received.
         // Call the receive callback qcall.
         processRxString(new std::string(mStringSocket.mRxString));
      }
      else
      {
         // String was not correctly received.
         Trc::write(mTI, 1, "UdpStringThread::threadRunFunction recv String ERROR");

         // Check for terminate.
         if (BaseClass::mTerminateFlag)
         {
            // Terminate.
            Trc::write(mTI, 0, "UdpStringThread read TERMINATE");
            goto End;
         }
         else
         {
            // Restart.
            mStringSocket.mValidFlag = false;
            goto Restart;
         }
      }
   }

   // Done.
End:
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
// Shutdown, base class overload. This sets the terminate request flag
// and closes the socket.
//
// If the while loop in the thread run function is blocked on receiving a 
// message then closing the socket will cause the receive call to return with
// false and then the terminate request flag will be polled and the
// thread run function will exit.

void UdpStringThread::shutdownThread()
{
   Trc::write(mTI, 1, "UdpStringThread::shutdownThread");

   // Set the terminate flag, close the socket, and wait for the thread
   // to terminate.
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

