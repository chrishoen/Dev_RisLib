/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "prnPrint.h"
#include "trcTrace.h"

#include "risThreadsPriorities.h"
#include "risNetUdpMsgThread.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

UdpMsgThread::UdpMsgThread(Settings& aSettings)
{
   // Base class variables.
   BaseClass::setThreadName("UdpMsg");
   BaseClass::setThreadPriority(aSettings.mThreadPriority);

   // Store settings.
   mSettings = aSettings;
   mRxMsgQCall = aSettings.mRxMsgQCall;

   // Initialize variables.
   mTI = mSettings.mTraceIndex;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function, base class overload.
// It configures the socket.

void UdpMsgThread::threadInitFunction()
{
   Trc::write(mTI, 1, "UdpMsgThread::threadInitFunction");
   // Initialize and configure the message socket.
   mMsgSocket.initialize(mSettings);
   mMsgSocket.configure();

   Trc::write(mTI, 1, "UdpMsgThread::threadInitFunction done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function, base class overload.
// It contains a while loop that receives messages.

void  UdpMsgThread::threadRunFunction()
{
   Trc::write(mTI, 1, "UdpMsgThread::threadRunFunction");
   bool tGoing = mMsgSocket.mValidFlag;
   bool tFirstFlag = true;

   while(tGoing)
   {
      // Try to receive a message with a blocking receive call. If a message
      // was received then process it.
      ByteContent* tMsg=0;
      if (mMsgSocket.doReceiveMsg(tMsg))
      {
         Trc::write(mTI, 1, "UdpMsgThread::threadRunFunction recv msg");
         // Message was correctly received.
         // Call the receive callback qcall.
         processRxMsg(tMsg);
      }
      else
      {
         Trc::write(mTI, 1, "UdpMsgThread::threadRunFunction recv msg ERROR");
      }

      // If termination request then exit the loop.
      // This is set by shutdown, see below.
      if (BaseThread::mTerminateFlag)
      {
         tGoing=false;
      }  
   }         
   Trc::write(mTI, 1, "UdpMsgThread::threadRunFunction done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function, base class overload.

void UdpMsgThread::threadExitFunction()
{
   Trc::write(mTI, 1, "UdpMsgThread::threadExitFunction");
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

void UdpMsgThread::shutdownThread()
{
   Trc::write(mTI, 1, "UdpMsgThread::shutdownThread");
   BaseThread::mTerminateFlag = true;

   mMsgSocket.doClose();

   BaseThread::waitForThreadTerminate();
   Trc::write(mTI, 1, "UdpMsgThread::shutdownThread done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Pass a received message to the parent thread. This is called by the
// threadRunFunction when a message is received. It invokes the
// mRxMsgQCall that is registered at initialization.

void UdpMsgThread::processRxMsg(Ris::ByteContent* aMsg)
{
   // Guard.
   if (!mRxMsgQCall.isValid()) return;

   // Invoke the receive callback qcall.
   mRxMsgQCall(aMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a transmit message through the socket to the peer. It executes a
// blocking send call in the context of the calling thread.

void UdpMsgThread::sendMsg (Ris::ByteContent* aMsg)
{
   // Send the message.
   mMsgSocket.doSendMsg(aMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

