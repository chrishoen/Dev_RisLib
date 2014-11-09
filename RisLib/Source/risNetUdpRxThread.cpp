/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "prnPrint.h"

#include "risNetUdpRxThread.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

BaseUdpRxThread::BaseUdpRxThread()
{
   mMulticastFlag=false;
}

//******************************************************************************
// Configure:

void BaseUdpRxThread::configure(
   char*                     aLocalIpAddr,
   int                       aLocalIpPort,
   BaseMessageParser*        aMessageParser)
{
   mRxSocketAddress.set(aLocalIpAddr,aLocalIpPort);
   mMessageParser = aMessageParser;
}

//******************************************************************************
// Configure:

void BaseUdpRxThread::configureForMulticast(
      char*                     aMulticastGroup,
      char*                     aLocalIpAddr,
      int                       aLocalIpPort,
      Ris::BaseMessageParser*   aMessageParser) 
{
   mMulticastGroup.set(aMulticastGroup);
   mRxSocketAddress.set(aLocalIpAddr,aLocalIpPort);
   mMessageParser = aMessageParser;
   mMulticastFlag = true;
}


//******************************************************************************
// Configure:

void UdpRxThreadWithQCall::configure(
   char*                     aLocalIpAddr,
   int                       aLocalIpPort,
   BaseMessageParser*        aMessageParser,
   RxMsgQCall*               aRxMsgQCall)
{
   Prn::print(Prn::SocketInit,Prn::Init1, "UdpRxThread::configure");

   BaseUdpRxThread::configure(
      aLocalIpAddr,
      aLocalIpPort,
      aMessageParser);

   mRxMsgQCall = *aRxMsgQCall;
}

//******************************************************************************
// Configure:

void UdpRxThreadWithQCall::configureForMulticast(
   char*                     aMulticastGroup,
   char*                     aLocalIpAddr,
   int                       aLocalIpPort,
   BaseMessageParser*        aMessageParser,
   RxMsgQCall*               aRxMsgQCall)
{
   Prn::print(Prn::SocketInit,Prn::Init1, "UdpRxThread::configureForMulticast");

   BaseUdpRxThread::configureForMulticast(
      aMulticastGroup,
      aLocalIpAddr,
      aLocalIpPort,
      aMessageParser);

   mRxMsgQCall = *aRxMsgQCall;
}

//******************************************************************************
// Configure:

void UdpRxThreadWithCallback::configure(
   char*                     aLocalIpAddr,
   int                       aLocalIpPort,
   BaseMessageParser*        aMessageParser,
   MsgCallPointer*           aRxCallback)
{
   Prn::print(Prn::SocketInit,Prn::Init1, "UdpRxThread::configure");

   BaseUdpRxThread::configure(
      aLocalIpAddr,
      aLocalIpPort,
      aMessageParser);

   mRxCallback = *aRxCallback;
}

//******************************************************************************
// Thread init function, base class overload.
// It configures the socket.

void BaseUdpRxThread::threadInitFunction()
{
   Prn::print(Prn::SocketInit,Prn::Init1, "UdpRxThread::threadInitFunction BEGIN");

   // Configure the socket
   if (!mMulticastFlag)
   {
      mRxSocket.configure(mRxSocketAddress,mMessageParser);
   }
   else
   {
      mRxSocket.configureForMulticast(mMulticastGroup,mRxSocketAddress,mMessageParser);
   }


   Prn::print(Prn::SocketInit,Prn::Init1, "UdpRxThread::threadInitFunction END");
}

//******************************************************************************
// Thread run function, base class overload.
// It contains a while loop that manages the connection to the server
// and receives messages.

void  BaseUdpRxThread::threadRunFunction()
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

void BaseUdpRxThread::threadExitFunction()
{
   Prn::print(Prn::SocketInit,Prn::Init1, "UdpRxThread::threadExitFunction");
}
//******************************************************************************
// Shutdown, base class overload.
// This sets the terminate request flag and closes the socket.
//
// If the while loop in the threadRunFunction is blocked on doRecvMsg then
// closing the socket will cause doRecvMsg to return with false and 
// then the terminate request flag will be polled and the the
// threadRunFunction will exit.

void BaseUdpRxThread::shutdownThread()
{
   BaseThreadWithTermFlag::mTerminateFlag = true;

   mRxSocket.doClose();

   BaseThreadWithTermFlag::waitForThreadTerminate();
}

//******************************************************************************

void UdpRxThreadWithQCall::processRxMsg(Ris::ByteContent* aRxMsg)
{
   // Invoke the receive QCall
   // Create a new qcall, copied from the original, and invoke it.
   mRxMsgQCall.invoke(aRxMsg);
}

//******************************************************************************

void UdpRxThreadWithCallback::processRxMsg(Ris::ByteContent* aRxMsg)
{
   // Call the receive callback
   mRxCallback(aRxMsg);
}

//******************************************************************************

}//namespace
}//namespace

