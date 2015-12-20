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
   mRemoteIpAddress[0]=0;
   mRemoteIpPort=0;
}

//******************************************************************************
// Configure:

void UdpMsgThread::configure(
   char*                      aLocalIpAddress,
   int                        aLocalIpPort,
   char*                      aRemoteIpAddress,
   int                        aRemoteIpPort,
   BaseMessageParserCreator*  aMessageParserCreator,
   RxMsgQCall*                aRxMsgQCall)
{
   strcpy(mLocalIpAddress,aLocalIpAddress);
   mLocalIpPort = aLocalIpPort;

   strcpy(mRemoteIpAddress, aRemoteIpAddress);
   mRemoteIpPort = aRemoteIpPort;

   mMessageParserCreator = aMessageParserCreator;

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
      mMessageParserCreator);

   mTxSocket.configure(
      mRemoteIpAddress,
      mRemoteIpPort,
      mMessageParserCreator);

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
      ByteContent* rxMsg=0;
      if (mRxSocket.doRecvMsg(rxMsg))
      {
         // Message was correctly received
         Prn::print(Prn::SocketRun1, "Recv message %d",mRxSocket.mRxMsgCount);

         // Call the receive method
         processRxMsg(rxMsg);
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
// If the while loop in the threadRunFunction is blocked on doRecvMsg then
// closing the socket will cause doRecvMsg to return with false and 
// then the terminate request flag will be polled and the threadRunFunction 
// will exit.

void UdpMsgThread::shutdownThread()
{
   BaseThreadWithTermFlag::mTerminateFlag = true;

   mRxSocket.doClose();

   BaseThreadWithTermFlag::waitForThreadTerminate();
}

//******************************************************************************

void UdpMsgThread::processRxMsg(Ris::ByteContent* aRxMsg)
{
   // Invoke the receive QCall
   // Create a new qcall, copied from the original, and invoke it.
   mRxMsgQCall.invoke(aRxMsg);
}

//******************************************************************************
// This sends a message via the tcp client thread

void UdpMsgThread::sendMsg (Ris::ByteContent* aTxMsg)
{
   mTxSocket.doSendMsg(aTxMsg);
}

//******************************************************************************

}//namespace
}//namespace

