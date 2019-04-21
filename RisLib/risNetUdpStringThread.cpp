/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "prnPrint.h"

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
   BaseClass::setThreadPrintLevel(aSettings.mPrintLevel);

   // Store settings.
   mSettings = aSettings;
   mRxStringQCall = aSettings.mRxStringQCall;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immediately 
// after the thread starts running. It configures the socket.

void UdpStringThread::threadInitFunction()
{
   // Initialize and configure the sockets.
   mRxSocket.initialize(mSettings);
   mRxSocket.configure();
   mTxSocket.initialize(mSettings);
   mTxSocket.configure();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function. This is called by the base class for the main 
// thread processing. Execute a while loop that does read calls.
// The loop exits when the thread is canceled.

void  UdpStringThread::threadRunFunction()
{
   bool tGoing=mRxSocket.mValidFlag;

   while(tGoing)
   {
      // Try to receive a message with a blocking receive call.
      // If a message was received then process it.
      if (mRxSocket.doRecvString())
      {
         // Message was correctly received.
         // Call the receive callback qcall.
         processRxString(new std::string(mRxSocket.mRxString));
      }
      else
      {
         // Message was not correctly received.
      }

      // If termination request then exit the loop.
      // This is set by shutdown, see below.
      if (mTerminateFlag)
      {
         tGoing=false;
      }  
   }         
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immediately
// before the thread is terminated.

void UdpStringThread::threadExitFunction()
{
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
   shutdownThreadPrologue();
   BaseThreadWithTermFlag::mTerminateFlag = true;

   mRxSocket.doClose();

   BaseThreadWithTermFlag::waitForThreadTerminate();
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

   TS::print(3, "UdpRxStringThread processRxString call qcall");

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
   mTxSocket.doSendString(aString->c_str());
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
   mTxSocket.doSendString(aString);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

