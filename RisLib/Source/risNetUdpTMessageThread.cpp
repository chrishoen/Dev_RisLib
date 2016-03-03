/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "prnPrint.h"

#include "ris_priorities.h"
#include "risNetUdpTMessageThread.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

UdpTMessageThread::UdpTMessageThread()
{
   mThreadPriority = get_default_udp_rx_thread_priority();
   mLocalIpAddress[0]=0;
   mLocalIpPort=0;
}

//******************************************************************************
// Configure:

void UdpTMessageThread::configure(
   char*                aLocalIpAddress,
   int                  aLocalIpPort,
   char*                aRemoteIpAddress,
   int                  aRemoteIpPort,
   Ris::BaseTMessageCopier* aMsgCopier,
   RxMessageQCall*      aRxMessageQCall)
{
   strcpy(mLocalIpAddress,aLocalIpAddress);
   mLocalIpPort = aLocalIpPort;

   strcpy(mRemoteIpAddress, aRemoteIpAddress);
   mRemoteIpPort = aRemoteIpPort;

   mMsgCopier  = aMsgCopier;
   mRxMessageQCall = *aRxMessageQCall;
}

//******************************************************************************
// Thread init function, base class overload.
// It configures the socket.

void UdpTMessageThread::threadInitFunction()
{
   Prn::print(Prn::SocketInit1, "UdpTMessageThread::threadInitFunction BEGIN");

   mRxSocket.configure(
      mLocalIpAddress,
      mLocalIpPort,
      mMsgCopier);

   mTxSocket.configure(
      mRemoteIpAddress,
      mRemoteIpPort,
      mMsgCopier);
   
   Prn::print(Prn::SocketInit1, "UdpTMessageThread::threadInitFunction END");
}

//******************************************************************************
// Thread run function, base class overload.
// It contains a while loop that manages the connection to the server
// and receives messages.

void  UdpTMessageThread::threadRunFunction()
{
   Prn::print(Prn::SocketRun1, "UdpRxTMessageThread::threadRunFunction");

   //-----------------------------------------------------------
   // Loop

   bool going=mRxSocket.mValidFlag;

   while(going)
   {
      // Try to receive a message with a blocking receive call
      // If a message was received then process it.
      // If a message was not received then the connection was lost.  
      ByteTMessage* tMsg=0;
      if (mRxSocket.doReceiveMessage(tMsg))
      {
         // Message was correctly received
         Prn::print(Prn::SocketRun1, "Recv message %d %d",mRxSocket.mRxCount,tMsg->mTMessageType);

         // Call the receive method
         processRxMessage(tMsg);
      }
      else
      {
         // Message was not correctly received, so
         // Connection was lost
         Prn::print(Prn::SocketRun1, "Recv failed, Connection lost");
      }
      //-----------------------------------------------------------
      // If termination request, exit the loop
      // This is set by shutdown, see below.
      if (mTerminateFlag)
      {
         going=false;
      }  
   }         
}

//******************************************************************************
// Thread exit function, base class overload.

void UdpTMessageThread::threadExitFunction()
{
   Prn::print(Prn::SocketInit1, "UdpTMessageThread::threadExitFunction");
}

//******************************************************************************
// Shutdown, base class overload.
// This sets the terminate request flag and closes the socket.
//
// If the while loop in the threadRunFunction is blocked on doReceiveMessage then
// closing the socket will cause doReceiveMessage to return with false and 
// then the terminate request flag will be polled and the threadRunFunction 
// will exit.

void UdpTMessageThread::shutdownThread()
{
   BaseThreadWithTermFlag::mTerminateFlag = true;

   mRxSocket.doClose();

   BaseThreadWithTermFlag::waitForThreadTerminate();
}

//******************************************************************************

void UdpTMessageThread::processRxMessage(Ris::ByteTMessage* aMsg)
{
   // Invoke the receive QCall
   // Create a new qcall, copied from the original, and invoke it.
   mRxMessageQCall(aMsg);
}

//******************************************************************************
// This sends a record 

void UdpTMessageThread::sendMessage (Ris::ByteTMessage* aMsg)
{
   mTxSocket.doSendMessage(aMsg);
}

//******************************************************************************

}//namespace
}//namespace

