/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdlib.h>

#include "prnPrint.h"

#include "risNetUdpMsgASocket.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

UdpRxMsgASocket::UdpRxMsgASocket()
{
   mRxLength      = 0;
   mRxMsgCount    = 0;
   mValidFlag     = false;
   mMsgParser = 0;
}

//******************************************************************************

UdpRxMsgASocket::~UdpRxMsgASocket()
{
   if (mMsgParser) delete mMsgParser;
}

//******************************************************************************
// configure the socket

void UdpRxMsgASocket::configure(
   char*                       aLocalIpAddr,
   int                         aLocalIpPort,
   BaseMsgAParserCreator*   aMsgParserCreator)
{
   mRxMsgCount=0;

   mLocal.set(aLocalIpAddr,aLocalIpPort);
   mMsgParser = aMsgParserCreator->createNew();

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

bool UdpRxMsgASocket::doReceiveMessage (ByteContent*& aMsg)
{
   //-------------------------------------------------------------------------
   // Initialize
   aMsg=0;

   // Guard
   if (!mValidFlag) return false;

   // Byte buffer, constructor takes size
   ByteBuffer tBuffer(MsgASocketDefT::cBufferSize);  
   tBuffer.setCopyFrom();

   //-------------------------------------------------------------------------
   // Read the message into the receive buffer
   
   doRecvFrom  (mFromAddress,tBuffer.getBaseAddress(),mRxLength,MsgASocketDefT::cBufferSize);

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

   mMsgParser->extractMessageHeaderParms(&tBuffer);

   // If the header is not valid then error
   if (!mMsgParser->mHeaderValidFlag)
   {
      Prn::print(Prn::SocketRun1, "ERROR doRecv1 INVALID HEADER ");
      return false;
   }

   //--------------------------------------------------------------
   // At this point the buffer contains the complete message.
   // Extract the message from the byte buffer into a new message
   // object and return it.

   tBuffer.rewind();
   aMsg = mMsgParser->makeFromByteBuffer(&tBuffer);

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

UdpTxMsgASocket::UdpTxMsgASocket()
{
   mTxCount=0;
   mTxLength      = 0;
   mValidFlag=false;
   mMsgParser=0;
}

//******************************************************************************

UdpTxMsgASocket::~UdpTxMsgASocket()
{
   if (mMsgParser) delete mMsgParser;
}

//******************************************************************************
// Configure the socket. Use with the next doSendMsg.

void UdpTxMsgASocket::configure(
   char*                      aRemoteIpAddr,
   int                        aRemoteIpPort,
   BaseMsgAParserCreator*  aMsgParserCreator)
{
   mTxCount=0;

   mRemote.set(aRemoteIpAddr,aRemoteIpPort);
   mMsgParser = aMsgParserCreator->createNew();

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

bool UdpTxMsgASocket::doSendMsg(ByteContent* aMsg)
{
   // Guard
   if (!mValidFlag) return false;

   // Process message before send
   mMsgParser->processBeforeSend(aMsg);

   // Create byte buffer, constructor takes size
   ByteBuffer tBuffer(MsgASocketDefT::cBufferSize);

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

//******************************************************************************
// Configure the socket. Use with the next doSendMsg.

void UdpTxMsgASocket::configure(
   BaseMsgAParser*     aMsgParser)
{
   mTxCount=0;

   mMsgParser = aMsgParser;

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

bool UdpTxMsgASocket::doSendMsg(
   Sockets::SocketAddress aRemote,
   ByteContent* aMsg)
{
   // Guard
   if (!mValidFlag) return false;

   // Byte buffer, constructor takes size
   ByteBuffer tBuffer(MsgASocketDefT::cBufferSize);  

   // Copy transmit message to buffer
   tBuffer.putToBuffer(aMsg);

   // Delete the message
   delete aMsg;

   // Mutex
   mTxMutex.lock();

   // Transmit the buffer
   int tLength=tBuffer.getLength();
   doSendTo(aRemote,tBuffer.getBaseAddress(),tLength);

   mTxCount++;

   // mutex
   mTxMutex.unlock();

   return true;
}

}//namespace
}//namespace

