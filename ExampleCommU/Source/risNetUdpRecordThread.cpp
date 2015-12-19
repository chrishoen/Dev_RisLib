/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "prnPrint.h"

#include "ris_priorities.h"
#include "risNetUdpRecordThread.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

UdpRecordThread::UdpRecordThread()
{
   mThreadPriority = get_default_udp_rx_thread_priority();
   mLocalIpAddress[0]=0;
   mLocalIpPort=0;
}

//******************************************************************************
// Configure:

void UdpRecordThread::configure(
   char*              aLocalIpAddress,
   int                aLocalIpPort,
   BaseRecordCopier*  aRecordCopier,
   RxRecordQCall*     aRxRecordQCall)
{
   strcpy(mLocalIpAddress,aLocalIpAddress);
   mLocalIpPort = aLocalIpPort;

   mRecordCopier  = aRecordCopier;
   mRxRecordQCall = *aRxRecordQCall;
}

//******************************************************************************
// Thread init function, base class overload.
// It configures the socket.

void UdpRecordThread::threadInitFunction()
{
   Prn::print(Prn::SocketInit1, "UdpRecordThread::threadInitFunction BEGIN");

   mRxSocket.configure(
      mLocalIpAddress,
      mLocalIpPort,
      mRecordCopier);

   Prn::print(Prn::SocketInit1, "UdpRecordThread::threadInitFunction END");
}

//******************************************************************************
// Thread run function, base class overload.
// It contains a while loop that manages the connection to the server
// and receives messages.

void  UdpRecordThread::threadRunFunction()
{
   Prn::print(Prn::SocketRun1, "UdpRxRecordThread::threadRunFunction");
   
   //-----------------------------------------------------------
   // Loop

   bool going=mRxSocket.mValidFlag;

   while(going)
   {
      // Try to receive a message with a blocking receive call
      // If a message was received then process it.
      // If a message was not received then the connection was lost.  
      ByteRecord* rxRecord=0;
      if (mRxSocket.doRecvRecord(rxRecord))
      {
         // Message was correctly received
         Prn::print(Prn::SocketRun1, "Recv message %d",mRxSocket.mRxCount);

         // Call the receive method
         processRxRecord(rxRecord);
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

void UdpRecordThread::threadExitFunction()
{
   Prn::print(Prn::SocketInit1, "UdpRecordThread::threadExitFunction");
}

//******************************************************************************
// Shutdown, base class overload.
// This sets the terminate request flag and closes the socket.
//
// If the while loop in the threadRunFunction is blocked on doRecvRecord then
// closing the socket will cause doRecvRecord to return with false and 
// then the terminate request flag will be polled and the threadRunFunction 
// will exit.

void UdpRecordThread::shutdownThread()
{
   BaseThreadWithTermFlag::mTerminateFlag = true;

   mRxSocket.doClose();

   BaseThreadWithTermFlag::waitForThreadTerminate();
}

//******************************************************************************

void UdpRecordThread::processRxRecord(Ris::ByteRecord* aRxRecord)
{
   // Invoke the receive QCall
   // Create a new qcall, copied from the original, and invoke it.
   mRxRecordQCall.invoke(aRxRecord);
}

//******************************************************************************

}//namespace
}//namespace

