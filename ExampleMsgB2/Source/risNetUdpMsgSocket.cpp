/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdlib.h>

#include "prnPrint.h"

#include "risByteMsgHeader.h"
#include "risNetUdpMsgSocket.h"

namespace Ris
{
namespace Net
{
//***************************************************************************
//***************************************************************************
//***************************************************************************

UdpRxMsgSocket::UdpRxMsgSocket()
{
   mRxLength     = 0;
   mRxCount      = 0;
   mValidFlag    = false;
   mMonkey    = 0;
}

//***************************************************************************
// Configure the socket

void UdpRxMsgSocket::configure(
   char*                aLocalIpAddr,
   int                  aLocalIpPort,
   Ris::BaseMsgMonkey* aMonkey)
{
   mRxCount=0;

   mLocal.set(aLocalIpAddr,aLocalIpPort);
   mMonkey = aMonkey;

   doSocket();
   doBind();

   if (mStatus==0)
   {
      Prn::print(Prn::SocketInit2, "UdpRxMsgSocket     $ %16s : %d",
         mLocal.mIpAddr.mString,
         mLocal.mPort);
   }
   else
   {
      Prn::print(Prn::SocketInit2, "UdpRxMsgSocket     $ %16s : %d $ %d %d",
         mLocal.mIpAddr.mString,
         mLocal.mPort,
         mStatus,
         mError);
   }

   mValidFlag=mStatus==0;
}

//***************************************************************************
// This receives a datagram from the socket into a byte buffer and then
// extracts a message from the byte buffer

bool UdpRxMsgSocket::doReceiveMsg (Ris::ByteMsg*& aMsg)
{
   //-------------------------------------------------------------------------
   // Initialize
   aMsg=0;

   // Guard
   if (!mValidFlag) return false;

   // Byte buffer, constructor takes size
   Ris::ByteBuffer tBuffer(mMonkey->bufferSize());
   tBuffer.setCopyFrom();

   //-------------------------------------------------------------------------
   // Read the message into the receive buffer

   doRecvFrom  (mFromAddress,tBuffer.getBaseAddress(),mRxLength,mMonkey->bufferSize());

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
   // Copy from the receive buffer into an instance of the header
   // and validate the header

   MsgHeader tHeader;

   tBuffer.setCopyFrom();
   tBuffer.getFromBuffer(&tHeader);

   // If the header is not valid then error
      
   if (!tHeader.validate())
   {
      Prn::print(Prn::SocketRun1, "ERROR doRecv1 INVALID HEADER ");
      return false;
   }

   //--------------------------------------------------------------
   // At this point the buffer contains the complete message.
   // Extract the message from the byte buffer into a new message
   // object and return it.

   // Create a record based on the record type
   aMsg = mMonkey->createMessage(tHeader.mMessageIdentifier);

   // Copy from the buffer into the record
   mMonkey->copyToFrom(&tBuffer, aMsg);

   // Test for errors and return.
   // If the pointer is zero then message is bad
   if (aMsg==0)
   {
      mStatus=tBuffer.getError();
   }

   // Returning true  means socket was not closed
   // Returning false means socket was closed
   mRxCount++;
   return true;
}

//***************************************************************************
//***************************************************************************
//***************************************************************************

UdpTxMsgSocket::UdpTxMsgSocket()
{
   mTxCount      = 0;
   mTxLength     = 0;
   mValidFlag    = false;
   mMonkey = 0;
}

//***************************************************************************
// Configure the socket. Use with the next doSendMsg.

void UdpTxMsgSocket::configure(
   char*                aRemoteIpAddr,
   int                  aRemoteIpPort,
   Ris::BaseMsgMonkey* aMonkey)
{
   mTxCount=0;

   mRemote.set(aRemoteIpAddr,aRemoteIpPort);
   mMonkey = aMonkey;

   doSocket();

   if (mStatus==0)
   {
      Prn::print(Prn::SocketInit2, "UdpTxMsgSocket     $ %16s : %d",
         mRemote.mIpAddr.mString,
         mRemote.mPort);
   }
   else
   {
      Prn::print(Prn::SocketInit2, "UdpTxMsgSocket     $ %16s : %d $ %d %d",
         mRemote.mIpAddr.mString,
         mRemote.mPort,
         mStatus,
         mError);
   }

   mValidFlag=mStatus==0;
}

//***************************************************************************
// This copies a message into a byte buffer and then sends the byte buffer 
// out the socket. Use with the previous configure.

bool UdpTxMsgSocket::doSendMsg(Ris::ByteMsg* aMsg)
{
   // Guard
   if (!mValidFlag) return false;

   // Create byte buffer, constructor takes size
   Ris::ByteBuffer tBuffer(mMonkey->bufferSize());

   //------------------------------------------------------------------------
   // Instance of a header,set members

   MsgHeader tHeader;
   tHeader.mMessageIdentifier = aMsg->mMessageType;

   //------------------------------------------------------------------------
   // Copy

   // Copy header to buffer
   tBuffer.setCopyTo();
   tHeader.headerCopyToFrom(&tBuffer,aMsg);

   // Copy record to buffer
   mMonkey->copyToFrom(&tBuffer,aMsg);
      
   // ReCopy header to buffer
   tHeader.headerReCopyToFrom(&tBuffer,aMsg);

   // Delete the record
   delete aMsg;

   //------------------------------------------------------------------------
   // Send buffer to socket

   // Mutex
   mTxMutex.lock();

   // Transmit the buffer
   mTxLength=tBuffer.getLength();
   doSendTo(mRemote,tBuffer.getBaseAddress(),mTxLength);
   Prn::print(Prn::SocketRun1, "UdpTxMsgSocket  doSendTo   $ %d",mTxLength);

   mTxCount++;

   // Mutex
   mTxMutex.unlock();

   return true;
}
}//namespace
}//namespace

