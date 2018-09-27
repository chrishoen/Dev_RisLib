/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "prnPrint.h"

#include "risNetTcpMsgSocket.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
TcpMsgSocket::TcpMsgSocket()
{
   mTxMsgCount=0;
   mRxMsgCount=0;
   mValidFlag=false;
   mMonkey=0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

TcpMsgSocket::~TcpMsgSocket()
{
   if (mMonkey != 0)
   {
      delete mMonkey;
      mMonkey = 0;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize variables.

void TcpMsgSocket::initialize(Settings* aSettings)
{
   // Store the settings pointer.
   mSettings = aSettings;

   // Create a message monkey.
   mMonkey = mSettings->mMonkeyCreator->createMonkey();

   // Metrics.
   mTxMsgCount = 0;
   mRxMsgCount = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Configure the socket.

void TcpMsgSocket::configure()
{
   // Configure the socket.
   BaseClass::reset();
   BaseClass::mRemote.set(mSettings->mRemoteIpAddr, mSettings->mRemoteIpPort);
   BaseClass::doSocket();
   BaseClass::setOptionKeepAlive();
   BaseClass::setOptionNoDelay();

   // Set valid flag from base class results.
   mValidFlag = BaseClass::mStatus == 0;

   // Show.
   if (mValidFlag)
   {
      Prn::print(Prn::SocketInit2, "TcpMsgSocket     $ %16s : %d",
         BaseClass::mRemote.mIpAddr.mString,
         BaseClass::mRemote.mPort);
   }
   else
   {
      Prn::print(Prn::SocketInit2, "TcpMsgSocket     $ %16s : %d $ %d %d",
         BaseClass::mRemote.mIpAddr.mString,
         BaseClass::mRemote.mPort,
         BaseClass::mStatus,
         BaseClass::mError);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Reconfigure the socket.

void TcpMsgSocket::reconfigure()
{
   // Configure the socket.
   BaseClass::doSocket();
   BaseClass::setOptionKeepAlive();
   BaseClass::setOptionNoDelay();

   // Metrics.
   mTxMsgCount = 0;
   mRxMsgCount = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Copy a message into a byte buffer and then send the byte buffer 
// via the socket.
// Return true if successful.

bool TcpMsgSocket::doSendMsg(ByteContent* aMsg)
{
   // Guard.
   if (!mValidFlag)
   {
      Prn::print(Prn::SocketRun2, "ERROR doSend when Invalid");
      delete aMsg;
      return false;
   }

   // Create a byte buffer.
   ByteBuffer tBuffer(mMonkey->getMaxBufferSize());

   // Configure the byte buffer.
   mMonkey->configureByteBuffer(&tBuffer);
   tBuffer.setCopyTo();

   // Copy the message to the buffer.
   mMonkey->putMsgToBuffer(&tBuffer,aMsg);

   // Delete the message.
   delete aMsg;

   // Mutex.
   mTxMutex.lock();

   // Transmit the buffer
   bool tRet=false;
   int tLength=tBuffer.getLength();
   tRet = doSend(tBuffer.getBaseAddress(),tLength);
   Prn::print(Prn::SocketRun4, "doSendM %d %d %d",mStatus,mError,tLength);

   mTxMsgCount++;

   // Mutex
   mTxMutex.unlock();

   if (!tRet)
   {
      Prn::print(Prn::SocketRun2, "ERROR TcpMsgSocket::doSendMsg FAIL");
   }

   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive a message from the socket via blocking recv calls into a
// byte buffer and extract a message from the byte buffer.
// Return true if successful.

bool TcpMsgSocket::doReceiveMsg (ByteContent*& aMsg)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize a receive byte buffer.

   // Do this forst.
   aMsg=0;
   bool tRet=false;
   int tStatus=0;

   // Create a byte buffer.
   ByteBuffer tBuffer(mMonkey->getMaxBufferSize());

   // Configure the byte buffer.
   mMonkey->configureByteBuffer(&tBuffer);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read the message header into the receive buffer.

   // Header varaibles.
   int   tHeaderLength = mMonkey->getHeaderLength();
   char* tHeaderBuffer = tBuffer.getBaseAddress();

   // Read the header from the socket.
   tBuffer.setCopyTo();
   tRet = BaseClass::doRecv(tHeaderBuffer,tHeaderLength,tStatus);
   Prn::print(Prn::SocketRun4, "doRecvH %d %d",mStatus,mError);

   // If bad status then return false.
   if (!tRet || tStatus<=0)
   {
      return false;
   }

   // Set the buffer length.
   tBuffer.setLength(tHeaderLength);

   // Copy from the receive buffer into the message monkey object
   // and validate the header.
   mMonkey->extractMessageHeaderParms(&tBuffer);

   // If the header is not valid then error.
   if (!mMonkey->mHeaderValidFlag)
   {
      Prn::print(Prn::SocketRun1, "ERROR doRecv1 INVALID HEADER");
      return false;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read the message payload into the receive buffer.

   // Payload variables.
   int   tPayloadLength = mMonkey->mPayloadLength;
   char* tPayloadBuffer = tBuffer.getBaseAddress() + tHeaderLength;

   // Read the payload from the socket.
   tBuffer.setCopyTo();
   tRet = BaseClass::doRecv(tPayloadBuffer,tPayloadLength,tStatus);
   Prn::print(Prn::SocketRun4, "doRecvP %d %d %d",mStatus,mError,tPayloadLength);

   // If bad status then return false.
   if (!tRet || tStatus<=0)
   {
      Prn::print(Prn::SocketRun1, "ERROR doRecv2 INVALID RECV");
      return false;
   }

   // Set the buffer length.
   tBuffer.setLength(mMonkey->mMessageLength);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // At this point the buffer contains the complete message. Extract the 
   // message from the byte buffer into a new message object and return it.

   tBuffer.rewind();
   aMsg = mMonkey->getMsgFromBuffer(&tBuffer);

   // Test for errors.
   if (aMsg==0)
   {
      Prn::print(Prn::SocketRun1, "ERROR getMsgFromBuffer");
      mStatus=tBuffer.getError();
      return false;
   }

   // Returning true  means socket was not closed
   // Returning false means socket was closed
   mRxMsgCount++;
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

