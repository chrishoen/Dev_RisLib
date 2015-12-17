/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdlib.h>

#include "prnPrint.h"

#include "risNetUdpMsgSocket.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

UdpRxMsgSocket::UdpRxMsgSocket()
{
   mRxBuffer = (char*)malloc(BUFFER_SIZE);
   mRxLength=0;
   mRxMsgCount=0;
   mValidFlag=false;

   mMessageParser=0;
}

//******************************************************************************

UdpRxMsgSocket::~UdpRxMsgSocket()
{
   if (mMessageParser) delete mMessageParser;
   free(mRxBuffer);
}

//******************************************************************************
// configure the socket

void UdpRxMsgSocket::configure(
   Sockets::SocketAddress      aLocal,
   BaseMessageParserCreator*   aMessageParserCreator)
{
   mRxMsgCount=0;

   mLocal         = aLocal;
   mMessageParser = aMessageParserCreator->createNew();

   doSocket();
   doBind();

   if (mStatus==0)
   {
      Prn::print(Prn::SocketInit2, "UdpRxMsgSocket     $ %16s : %d",
         aLocal.mIpAddr.mString,
         aLocal.mPort);
   }
   else
   {
      Prn::print(Prn::SocketInit2, "UdpRxMsgSocket     $ %16s : %d $ %d %d",
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

bool UdpRxMsgSocket::doRecvMsg (ByteContent*& aRxMsg)
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

   //--------------------------------------------------------------
   // Copy from the receive buffer into the message parser object
   // and validate the header

   mMessageParser->extractMessageHeaderParms(&tBuffer);

   // If the header is not valid then error
   if (!mMessageParser->mHeaderValidFlag)
   {
      Prn::print(Prn::SocketRun1, "ERROR doRecv1 INVALID HEADER ");
      return false;
   }

   //--------------------------------------------------------------
   // At this point the buffer contains the complete message.
   // Extract the message from the byte buffer into a new message
   // object and return it.

   tBuffer.rewind();
   aRxMsg = mMessageParser->makeFromByteBuffer(&tBuffer);

   // Test for errors and return.
   // If the pointer is zero then message is bad
   if (aRxMsg==0)
   {
      mStatus=tBuffer.getError();
   }

   // Returning true  means socket was not closed
   // Returning false means socket was closed
   mRxMsgCount++;
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

UdpTxMsgSocket::UdpTxMsgSocket()
{
   mTxBuffer = (char*)malloc(BUFFER_SIZE);
   mTxLength=0;
   mTxMsgCount=0;
   mValidFlag=false;

   mMessageParser=0;
}

//******************************************************************************

UdpTxMsgSocket::~UdpTxMsgSocket()
{
   if (mMessageParser) delete mMessageParser;
   free(mTxBuffer);
}

//******************************************************************************
// Configure the socket. Use with the next doSendMsg.

void UdpTxMsgSocket::configure(
   Sockets::SocketAddress     aRemote,
   BaseMessageParserCreator*  aMessageParserCreator)
{
   mTxMsgCount=0;

   mRemote        = aRemote;
   mMessageParser = aMessageParserCreator->createNew();

   doSocket();

   if (mStatus==0)
   {
      Prn::print(Prn::SocketInit2, "UdpTxMsgSocket     $ %16s : %d",
         aRemote.mIpAddr.mString,
         aRemote.mPort);
   }
   else
   {
      Prn::print(Prn::SocketInit2, "UdpTxMsgSocket     $ %16s : %d $ %d %d",
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

bool UdpTxMsgSocket::doSendMsg(
   ByteContent* aTxMsg)
{
   // Guard
   if (!mValidFlag) return false;

   // Process message before send
   mMessageParser->processBeforeSend(aTxMsg);

   // Create byte buffer, constructor takes address and size
   ByteBuffer tBuffer(mTxBuffer,BUFFER_SIZE);

   // Copy transmit message to buffer
   tBuffer.putToBuffer(aTxMsg);

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

//******************************************************************************
// Configure the socket. Use with the next doSendMsg.

void UdpTxMsgSocket::configure(
   BaseMessageParser*     aMessageParser)
{
   mTxMsgCount=0;

   mMessageParser = aMessageParser;

   doSocket();

   Prn::print(Prn::SocketInit1, "UdpTxMsgSocket:: %s %s %d %d",
      mRemote.mIpAddr.mString,
      mStatus,
      mError);

   mValidFlag=mStatus==0;
}

//******************************************************************************
// This copies a message into a byte buffer and then sends the byte buffer 
// out the socket. Use with the previous configure.

bool UdpTxMsgSocket::doSendMsg(
   Sockets::SocketAddress aRemote,
   ByteContent* aTxMsg)
{
   // Guard
   if (!mValidFlag) return false;

   // mutex
   mTxMutex.get();

   // byte buffer, constructor takes address and size
   ByteBuffer tBuffer(mTxBuffer,BUFFER_SIZE);  

   // copy transmit message to buffer
   tBuffer.putToBuffer(aTxMsg);

   // delete the message
   delete aTxMsg;

   // transmit the buffer
   int length=tBuffer.getLength();
   doSendTo(aRemote,tBuffer.getBaseAddress(),length);

   mTxLength=length;
   mTxMsgCount++;

   // mutex
   mTxMutex.put();

   return true;
}

}//namespace
}//namespace

