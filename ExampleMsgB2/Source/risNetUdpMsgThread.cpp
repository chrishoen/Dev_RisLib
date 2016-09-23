/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "prnPrint.h"

#include "ris_priorities.h"
#include "risNetUdpMsgThread.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

UdpMsgThread::UdpMsgThread()
{
   mThreadPriority = get_default_udp_rx_thread_priority();
   mLocalIpAddress[0]=0;
   mLocalIpPort=0;
}

//******************************************************************************
// Configure:

void UdpMsgThread::configure(
   char*                aLocalIpAddress,
   int                  aLocalIpPort,
   char*                aRemoteIpAddress,
   int                  aRemoteIpPort,
   Ris::BaseMsgMonkey* aMonkey,
   RxMsgQCall*          aRxMsgQCall)
{
   strcpy(mLocalIpAddress,aLocalIpAddress);
   mLocalIpPort = aLocalIpPort;

   strcpy(mRemoteIpAddress, aRemoteIpAddress);
   mRemoteIpPort = aRemoteIpPort;

   mMonkey  = aMonkey;
   mRxMsgQCall = *aRxMsgQCall;
}

//******************************************************************************
// Thread init function, base class overload.
// It configures the socket.

void UdpMsgThread::threadInitFunction()
{
   Prn::print(Prn::SocketInit1, "UdpMsgThread::threadInitFunction BEGIN");

   mRxSocket.configure(
      mLocalIpAddress,
      mLocalIpPort,
      mMonkey);

   mTxSocket.configure(
      mRemoteIpAddress,
      mRemoteIpPort,
      mMonkey);
   
   Prn::print(Prn::SocketInit1, "UdpMsgThread::threadInitFunction END");
}

//******************************************************************************
// Thread run function, base class overload.
// It contains a while loop that manages the connection to the server
// and receives messages.

void  UdpMsgThread::threadRunFunction()
{
   Prn::print(Prn::SocketRun1, "UdpRxMsgThread::threadRunFunction");

   //-----------------------------------------------------------
   // Loop

   bool going=mRxSocket.mValidFlag;

   while(going)
   {
      // Try to receive a message with a blocking receive call
      // If a message was received then process it.
      // If a message was not received then the connection was lost.  
      ByteMsg* tMsg=0;
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

void UdpMsgThread::threadExitFunction()
{
   Prn::print(Prn::SocketInit1, "UdpMsgThread::threadExitFunction");
}

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
   BaseThreadWithTermFlag::mTerminateFlag = true;

   mRxSocket.doClose();

   BaseThreadWithTermFlag::waitForThreadTerminate();
}

//******************************************************************************

void UdpMsgThread::processRxMsg(Ris::ByteMsg* aMsg)
{
   // Invoke the receive QCall
   // Create a new qcall, copied from the original, and invoke it.
   mRxMsgQCall(aMsg);
}

//******************************************************************************
// This sends a record 

void UdpMsgThread::sendMsg (Ris::ByteMsg* aMsg)
{
   mTxSocket.doSendMsg(aMsg);
}

//******************************************************************************

}//namespace
}//namespace

