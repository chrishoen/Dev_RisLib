/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdlib.h>

#include "prnPrint.h"

#include "risNetUdpRecordSocket.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

UdpRxRecordSocket::UdpRxRecordSocket()
{
   mRxBuffer = (char*)malloc(BUFFER_SIZE);
   mRxLength=0;
   mRxMsgCount=0;
   mValidFlag=false;

   mRecordCopier=0;
}

//******************************************************************************

UdpRxRecordSocket::~UdpRxRecordSocket()
{
   if (mRecordCopier) delete mRecordCopier;
   free(mRxBuffer);
}

//******************************************************************************
// configure the socket

void UdpRxRecordSocket::configure(
   Sockets::SocketAddress      aLocal,
   BaseRecordCopier*   aRecordCopier)
{
   mRxMsgCount=0;

   mLocal         = aLocal;
   mRecordCopier = aRecordCopier;

   doSocket();
   doBind();

   if (mStatus==0)
   {
      Prn::print(Prn::SocketInit2, "UdpRxRecordSocket     $ %16s : %d",
         aLocal.mIpAddr.mString,
         aLocal.mPort);
   }
   else
   {
      Prn::print(Prn::SocketInit2, "UdpRxRecordSocket     $ %16s : %d $ %d %d",
         aLocal.mIpAddr.mString,
         aLocal.mPort,
         mStatus,
         mError);
   }

   mValidFlag=mStatus==0;
}

//******************************************************************************
// This receives a datagram from the socket into a byte buffer and then
// extracts a message from the byte buffer

bool UdpRxRecordSocket::doRecvMsg (ByteRecord*& aRxMsg)
{
   //-------------------------------------------------------------------------
   // Initialize
   aRxMsg=0;

   // Guard
   if (!mValidFlag) return false;

   // Byte buffer, constructor takes address and size
   ByteBuffer tBuffer(mRxBuffer,BUFFER_SIZE);  
   tBuffer.setCopyFrom();

   //-------------------------------------------------------------------------
   // Read the message into the receive buffer
   
   doRecvFrom  (mFromAddress,mRxBuffer,mRxLength,BUFFER_SIZE);

   // Guard
   // If bad status then return false.
   // Returning true  means socket was not closed
   // Returning false means socket was closed

   if (mRxLength<=0)
   {
      if (mStatus<0)
         switch (mError)
         {
            case 0              : return false  ;break;
            default             : return false  ;break;
         }   
      else
         return false;
   }

   // Set the buffer length
   tBuffer.setLength(mRxLength);
#if 0
   //--------------------------------------------------------------
   // Copy from the receive buffer into the message parser object
   // and validate the header

   mRecordCopier->extractMessageHeaderParms(&tBuffer);

   // If the header is not valid then error
   if (!mRecordCopier->mHeaderValidFlag)
   {
      Prn::print(Prn::SocketRun1, "ERROR doRecv1 INVALID HEADER ");
      return false;
   }

   //--------------------------------------------------------------
   // At this point the buffer contains the complete message.
   // Extract the message from the byte buffer into a new message
   // object and return it.

   tBuffer.rewind();
   aRxMsg = mRecordCopier->makeFromByteBuffer(&tBuffer);

   // Test for errors and return.
   // If the pointer is zero then message is bad
   if (aRxMsg==0)
   {
      mStatus=tBuffer.getError();
   }
#endif
   // Returning true  means socket was not closed
   // Returning false means socket was closed
   mRxMsgCount++;
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

UdpTxRecordSocket::UdpTxRecordSocket()
{
   mTxBuffer = (char*)malloc(BUFFER_SIZE);
   mTxLength=0;
   mTxMsgCount=0;
   mValidFlag=false;

   mRecordCopier=0;
}

//******************************************************************************

UdpTxRecordSocket::~UdpTxRecordSocket()
{
   if (mRecordCopier) delete mRecordCopier;
   free(mTxBuffer);
}

//******************************************************************************
// Configure the socket. Use with the next doSendMsg.

void UdpTxRecordSocket::configure(
   Sockets::SocketAddress     aRemote,
   BaseRecordCopier*  aRecordCopier)
{
   mTxMsgCount=0;

   mRemote        = aRemote;
   mRecordCopier = aRecordCopier;

   doSocket();

   if (mStatus==0)
   {
      Prn::print(Prn::SocketInit2, "UdpTxRecordSocket     $ %16s : %d",
         aRemote.mIpAddr.mString,
         aRemote.mPort);
   }
   else
   {
      Prn::print(Prn::SocketInit2, "UdpTxRecordSocket     $ %16s : %d $ %d %d",
         aRemote.mIpAddr.mString,
         aRemote.mPort,
         mStatus,
         mError);
   }

   mValidFlag=mStatus==0;
}

//******************************************************************************
// This copies a message into a byte buffer and then sends the byte buffer 
// out the socket. Use with the previous configure.

bool UdpTxRecordSocket::doSendMsg(
   ByteRecord* aTxMsg)
{
   // Guard
   if (!mValidFlag) return false;

   // Process message before send
// mRecordCopier->processBeforeSend(aTxMsg);

   // Create byte buffer, constructor takes address and size
   ByteBuffer tBuffer(mTxBuffer,BUFFER_SIZE);

   // Copy transmit message to buffer
// tBuffer.putToBuffer(aTxMsg);

   // Delete the message
   delete aTxMsg;

   // Mutex
   mTxMutex.get();

   // Transmit the buffer
   int length=tBuffer.getLength();
   doSendTo(mRemote,tBuffer.getBaseAddress(),length);

   mTxLength=length;
   mTxMsgCount++;

   // Mutex
   mTxMutex.put();

   return true;
}


}//namespace
}//namespace

