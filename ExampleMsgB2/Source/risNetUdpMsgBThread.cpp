/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "prnPrint.h"

#include "ris_priorities.h"
#include "risNetUdpMsgBThread.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

UdpMsgBThread::UdpMsgBThread()
{
   mThreadPriority = get_default_udp_rx_thread_priority();
   mLocalIpAddress[0]=0;
   mLocalIpPort=0;
}

//******************************************************************************
// Configure:

void UdpMsgBThread::configure(
   char*                aLocalIpAddress,
   int                  aLocalIpPort,
   char*                aRemoteIpAddress,
   int                  aRemoteIpPort,
   Ris::BaseMsgBCopier* aMsgCopier,
   RxMsgQCall*          aRxMsgQCall)
{
   strcpy(mLocalIpAddress,aLocalIpAddress);
   mLocalIpPort = aLocalIpPort;

   strcpy(mRemoteIpAddress, aRemoteIpAddress);
   mRemoteIpPort = aRemoteIpPort;

   mMsgCopier  = aMsgCopier;
   mRxMsgQCall = *aRxMsgQCall;
}

//******************************************************************************
// Thread init function, base class overload.
// It configures the socket.

void UdpMsgBThread::threadInitFunction()
{
   Prn::print(Prn::SocketInit1, "UdpMsgBThread::threadInitFunction BEGIN");

   mRxSocket.configure(
      mLocalIpAddress,
      mLocalIpPort,
      mMsgCopier);

   mTxSocket.configure(
      mRemoteIpAddress,
      mRemoteIpPort,
      mMsgCopier);
   
   Prn::print(Prn::SocketInit1, "UdpMsgBThread::threadInitFunction END");
}

//******************************************************************************
// Thread run function, base class overload.
// It contains a while loop that manages the connection to the server
// and receives messages.

void  UdpMsgBThread::threadRunFunction()
{
   Prn::print(Prn::SocketRun1, "UdpRxMsgBThread::threadRunFunction");

   //-----------------------------------------------------------
   // Loop

   bool going=mRxSocket.mValidFlag;

   while(going)
   {
      // Try to receive a message with a blocking receive call
      // If a message was received then process it.
      // If a message was not received then the connection was lost.  
      ByteMsgB* tMsg=0;
      if (mRxSocket.doReceiveMsg(tMsg))
      {
         // Message was correctly received
         Prn::print(Prn::SocketRun1, "Recv message %d %d",mRxSocket.mRxCount,tMsg->mMessageType);

         // Call the receive method
         processRxMsg(tMsg);
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

void UdpMsgBThread::threadExitFunction()
{
   Prn::print(Prn::SocketInit1, "UdpMsgBThread::threadExitFunction");
}

//******************************************************************************
// Shutdown, base class overload.
// This sets the terminate request flag and closes the socket.
//
// If the while loop in the threadRunFunction is blocked on doReceiveMsg then
// closing the socket will cause doReceiveMsg to return with false and 
// then the terminate request flag will be polled and the threadRunFunction 
// will exit.

void UdpMsgBThread::shutdownThread()
{
   BaseThreadWithTermFlag::mTerminateFlag = true;

   mRxSocket.doClose();

   BaseThreadWithTermFlag::waitForThreadTerminate();
}

//******************************************************************************

void UdpMsgBThread::processRxMsg(Ris::ByteMsgB* aMsg)
{
   // Invoke the receive QCall
   // Create a new qcall, copied from the original, and invoke it.
   mRxMsgQCall(aMsg);
}

//******************************************************************************
// This sends a record 

void UdpMsgBThread::sendMsg (Ris::ByteMsgB* aMsg)
{
   mTxSocket.doSendMsg(aMsg);
}

//******************************************************************************

}//namespace
}//namespace

