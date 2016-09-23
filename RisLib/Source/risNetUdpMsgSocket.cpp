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
   mRxLength      = 0;
   mRxMsgCount    = 0;
   mValidFlag     = false;
   mMonkey     = 0;
}

//******************************************************************************

UdpRxMsgSocket::~UdpRxMsgSocket()
{
   if (mMonkey) delete mMonkey;
}

//******************************************************************************
// configure the socket

void UdpRxMsgSocket::configure(
   char*                  aLocalIpAddr,
   int                    aLocalIpPort,
   BaseMsgMonkeyCreator* aMsgMonkeyCreator)
{
   mRxMsgCount=0;

   mLocal.set(aLocalIpAddr,aLocalIpPort);
   mMonkey = aMsgMonkeyCreator->createNew();

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

//******************************************************************************
// This receives a datagram from the socket into a byte buffer and then
// extracts a message from the byte buffer

bool UdpRxMsgSocket::doReceiveMsg (ByteContent*& aMsg)
{
   //-------------------------------------------------------------------------
   // Initialize
   aMsg=0;

   // Guard
   if (!mValidFlag) return false;

   // Byte buffer, constructor takes size
   ByteBuffer tBuffer(mMonkey->getMaxBufferSize());
   tBuffer.setCopyFrom();

   //-------------------------------------------------------------------------
   // Read the message into the receive buffer
   
   doRecvFrom  (mFromAddress,tBuffer.getBaseAddress(),mRxLength,mMonkey->getMaxBufferSize());

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
   // Copy from the receive buffer into the message monkey object
   // and validate the header

   mMonkey->extractMessageHeaderParms(&tBuffer);

   // If the header is not valid then error
   if (!mMonkey->mHeaderValidFlag)
   {
      Prn::print(Prn::SocketRun1, "ERROR doRecv1 INVALID HEADER ");
      return false;
   }

   //--------------------------------------------------------------
   // At this point the buffer contains the complete message.
   // Extract the message from the byte buffer into a new message
   // object and return it.

   tBuffer.rewind();
   aMsg = mMonkey->makeFromByteBuffer(&tBuffer);

   // Test for errors and return.
   // If the pointer is zero then message is bad
   if (aMsg==0)
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
   mTxCount=0;
   mTxLength      = 0;
   mValidFlag=false;
   mMonkey=0;
}

//******************************************************************************

UdpTxMsgSocket::~UdpTxMsgSocket()
{
   if (mMonkey) delete mMonkey;
}

//******************************************************************************
// Configure the socket. Use with the next doSendMsg.

void UdpTxMsgSocket::configure(
   char*                  aRemoteIpAddr,
   int                    aRemoteIpPort,
   BaseMsgMonkeyCreator* aMsgMonkeyCreator)
{
   mTxCount=0;

   mRemote.set(aRemoteIpAddr,aRemoteIpPort);
   mMonkey = aMsgMonkeyCreator->createNew();

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

//******************************************************************************
// This copies a message into a byte buffer and then sends the byte buffer 
// out the socket. Use with the previous configure.

bool UdpTxMsgSocket::doSendMsg(ByteContent* aMsg)
{
   // Guard
   if (!mValidFlag) return false;

   // Process message before send
   mMonkey->processBeforeSend(aMsg);

   // Create byte buffer, constructor takes size
   ByteBuffer tBuffer(mMonkey->getMaxBufferSize());

   // Copy transmit message to buffer
   tBuffer.putToBuffer(aMsg);

   // Delete the message
   delete aMsg;

   // Mutex
   mTxMutex.lock();

   // Transmit the buffer
   mTxLength=tBuffer.getLength();
   doSendTo(mRemote,tBuffer.getBaseAddress(),mTxLength);

   mTxCount++;

   // Mutex
   mTxMutex.unlock();

   return true;
}


}//namespace
}//namespace

