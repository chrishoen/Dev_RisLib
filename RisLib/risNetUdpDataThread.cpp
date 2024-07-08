/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "prnPrint.h"
#include "trcTrace.h"

#include "risThreadsPriorities.h"
#include "risNetUdpDataThread.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

UdpDataThread::UdpDataThread(Settings& aSettings)
{
   // Base class variables.
   BaseClass::setThreadName("UdpData");
   BaseClass::setThreadPriority(aSettings.mThreadPriority);

   // Store settings.
   mSettings = aSettings;
   mRxDataQCall = aSettings.mRxDataQCall;
   mMaxDataSize = aSettings.mMaxDataSize;

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

void UdpDataThread::threadInitFunction()
{
   Trc::write(mTI, 1, "UdpDataThread::threadInitFunction");
   Trc::write(mTI, 1, "UdpDataThread::threadInitFunction done");

   // Initialize the socket according to the settings.
   mDataSocket.initialize(&mSettings);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function. This is called by the base class immediately
// after the thread init function. It runs a loop that blocks on 
// udp port receives and then processes them. The loop terminates
// when the socket is closed by the shutdown call.

void UdpDataThread::threadRunFunction()
{
   Trc::write(mTI, 0, "UdpDataThread::threadRunFunction");

   // Top of the loop.
   mRestartCount = 0;

Restart:

   // Guard.
   if (mTerminateFlag) return;
   int tRet = 0;

   // If not first time then sleep.
   if (mRestartCount > 0)
   {
      BaseClass::threadSleep(1000);
   }
   Trc::write(mTI, 0, "UdpDataThread restart %d", mRestartCount);
   Prn::print(Prn::Show1, "UdpDataThread restart %d", mRestartCount);
   mRestartCount++;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Open device.

   // Close the socket.
   mDataSocket.doClose();

   // Configure the socket.
   mDataSocket.configure();
   if (!mDataSocket.mValidFlag)
   {
      // If error then restart.
      goto Restart;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Loop to receive data.

   while (!BaseClass::mTerminateFlag)
   {
      //************************************************************************
      //************************************************************************
      //************************************************************************
      // Receive.

      // Try to receive data with a blocking receive call. If data
      // was received then process it.
      char* tData = new char[mMaxDataSize];
      int tSize = 0;
      if (mDataSocket.doRecvData(tData, &tSize))
      {
         // Data was correctly received.
         // Call the receive callback qcall.
         processRxData(tData, tSize);
      }
      else
      {
         // Data was not correctly received. 
         Trc::write(mTI, 1, "UdpDataThread::threadRunFunction recv Data ERROR");

         // Check for terminate.
         if (BaseClass::mTerminateFlag)
         {
            // Terminate.
            Trc::write(mTI, 0, "UdpDataThread read TERMINATE");
            break;
         }
         else
         {
            // Restart.
            mDataSocket.mValidFlag = false;
            goto Restart;
         }
      }
   }

   // Done.
   Trc::write(mTI, 0, "UdpDataThread::threadRunFunction done");
   return;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immediately
// before the thread is terminated.

void UdpDataThread::threadExitFunction()
{
   Trc::write(mTI, 1, "UdpDataThread::threadExitFunction");
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

void UdpDataThread::shutdownThread()
{
   Trc::write(mTI, 1, "UdpDataThread::shutdownThread");

   // Set the terminate flag, close the socket, and wait for the thread
   // to terminate.
   BaseThread::mTerminateFlag = true;
   mDataSocket.doClose();
   BaseThread::waitForThreadTerminate();

   Trc::write(mTI, 1, "UdpDataThread::shutdownThread done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Pass a received data to the parent thread. This is called by the
// threadRunFunction when a data is received. It invokes the
// mRxDataQCall that is registered at initialization.

void UdpDataThread::processRxData(char* aData, int aSize)
{
   // Guard.
   if (!mRxDataQCall.isValid()) return;

   //printf("UdpRxDataThread processRxData call qcall\n");

   // Invoke the receive callback qcall.
   mRxDataQCall(aData, aSize);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a transmit data through the socket to the peer. It executes a
// blocking send call in the context of the calling thread. Delete the
// data when done.

void UdpDataThread::sendData (const char* aData, int aSize)
{

   // Send the data.
   mDataSocket.doSendData(aData, aSize);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

