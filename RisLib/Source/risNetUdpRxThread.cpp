/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "prnPrint.h"

#include "ris_priorities.h"
#include "risNetUdpRxThread.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

UdpRxThread::UdpRxThread()
{
   mThreadPriority = get_default_udp_rx_thread_priority();
}

//******************************************************************************
// Configure:

void UdpRxThread::configure(
   char*                      aLocalIpAddr,
   int                        aLocalIpPort,
   BaseMessageParserCreator*  aMessageParserCreator,
   RxMsgQCall*                aRxMsgQCall)
{
   Prn::print(Prn::SocketInit,Prn::Init1, "UdpRxThread::configure");

   mRxSocketAddress.set(aLocalIpAddr,aLocalIpPort);
   mMessageParserCreator = aMessageParserCreator;

   mRxMsgQCall = *aRxMsgQCall;
}

//******************************************************************************
// Thread init function, base class overload.
// It configures the socket.

void UdpRxThread::threadInitFunction()
{
   Prn::print(Prn::SocketInit,Prn::Init1, "UdpRxThread::threadInitFunction BEGIN");

   mRxSocket.configure(mRxSocketAddress,mMessageParserCreator);

   Prn::print(Prn::SocketInit,Prn::Init1, "UdpRxThread::threadInitFunction END");
}

//******************************************************************************
// Thread run function, base class overload.
// It contains a while loop that manages the connection to the server
// and receives messages.

void  UdpRxThread::threadRunFunction()
{
   Prn::print(Prn::SocketRun,Prn::Run1, "UdpRxMsgThread::threadRunFunction");
   
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
         Prn::print(Prn::SocketRun,Prn::Run1, "Recv message %d",mRxSocket.mRxMsgCount);

         // Call the receive method
         processRxMsg(rxMsg);
      }
      else
      {
         // Message was not correctly received, so
         // Connection was lost
         Prn::print(Prn::SocketRun,Prn::Run1, "Recv failed, Connection lost");
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

void UdpRxThread::threadExitFunction()
{
   Prn::print(Prn::SocketInit,Prn::Init1, "UdpRxThread::threadExitFunction");
}
//******************************************************************************
// Shutdown, base class overload.
// This sets the terminate request flag and closes the socket.
//
// If the while loop in the threadRunFunction is blocked on doRecvMsg then
// closing the socket will cause doRecvMsg to return with false and 
// then the terminate request flag will be polled and the threadRunFunction 
// will exit.

void UdpRxThread::shutdownThread()
{
   BaseThreadWithTermFlag::mTerminateFlag = true;

   mRxSocket.doClose();

   BaseThreadWithTermFlag::waitForThreadTerminate();
}

//******************************************************************************

void UdpRxThread::processRxMsg(Ris::ByteContent* aRxMsg)
{
   // Invoke the receive QCall
   // Create a new qcall, copied from the original, and invoke it.
   mRxMsgQCall.invoke(aRxMsg);
}

//******************************************************************************

}//namespace
}//namespace

