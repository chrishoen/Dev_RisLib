//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

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
   mRxLength = 0;
   mRxCount = 0;
   mValidFlag = false;
   mMonkey = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

UdpRxMsgSocket::~UdpRxMsgSocket()
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

void UdpRxMsgSocket::initialize(Settings* aSettings)
{
   // Store the settings pointer.
   mSettings = aSettings;

   // Create a message monkey.
   mMonkey = mSettings->mMonkeyCreator->createMonkey();

   // Metrics.
   mRxCount = 0;
   mRxLength = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Configure the socket.

void UdpRxMsgSocket::configure()
{
   // Configure the socket.
   BaseClass::mLocal.set(mSettings->mLocalIpAddr, mSettings->mLocalIpPort);
   BaseClass::doSocket();
   BaseClass::doBind();

   // Set valid flag from base class results.
   mValidFlag = BaseClass::mStatus == 0;

   // Show.
   if (mValidFlag)
   {
      Prn::print(Prn::SocketInit2, "UdpRxMsgSocket     $ %16s : %d",
         BaseClass::mLocal.mIpAddr.mString,
         BaseClass::mLocal.mPort);
   }
   else
   {
      Prn::print(Prn::SocketInit2, "UdpRxMsgSocket     $ %16s : %d $ %d %d",
         BaseClass::mLocal.mIpAddr.mString,
         BaseClass::mLocal.mPort,
         BaseClass::mStatus,
         BaseClass::mError);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive a datagram from the socket into a byte buffer and then extract a 
// message from it. Return true if successful.

bool UdpRxMsgSocket::doReceiveMsg(ByteContent*& aMsg)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize.

   // Do this first.
   aMsg = 0;

   // Guard.
   if (!mValidFlag) return false;

   // Create a byte buffer.
   ByteBuffer tBuffer(mMonkey->getMaxBufferSize());

   // Configure the byte buffer.
   mMonkey->configureByteBuffer(&tBuffer);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read the message into the receive buffer.

   tBuffer.setCopyFrom();
   BaseClass::doRecvFrom(mFromAddress, tBuffer.getBaseAddress(), mRxLength, mMonkey->getMaxBufferSize());

   // Guard.
   // If bad status then return false.
   // Returning true  means socket was not closed.
   // Returning false means socket was closed.

   if (mRxLength <= 0)
   {
      Prn::print(Prn::SocketRun1, "UdpRxMsgSocket ERROR  %d %d", mStatus, mError);
      if (BaseClass::mStatus < 0)
         switch (mError)
         {
         case 0: return false; break;
         default: return false; break;
         }
      else
         return false;
   }

   Prn::print(Prn::SocketRun2, "UdpRxMsgSocket rx message %d", mRxLength);

   // Set the buffer length.
   tBuffer.setLength(mRxLength);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Copy from the receive buffer into the message monkey object and validate
   // the header.

   mMonkey->extractMessageHeaderParms(&tBuffer);

   Prn::print(Prn::SocketRun3, "UdpRxMsgSocket rx header %d %d",
      mMonkey->mHeaderValidFlag,
      mMonkey->mHeaderLength);

   // If the header is not valid then error.
   if (!mMonkey->mHeaderValidFlag)
   {
      Prn::print(Prn::SocketRun1, "UdpRxMsgSocket ERROR INVALID HEADER", mStatus, mError);
      return false;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // At this point the buffer contains the complete message. Extract the
   // message from the byte buffer into a new message object and return it.

   tBuffer.rewind();
   aMsg = mMonkey->getMsgFromBuffer(&tBuffer);

   // Test for errors.
   if (aMsg == 0)
   {
      Prn::print(Prn::SocketRun1, "UdpRxMsgSocket ERROR INVALID MESSAGE", mStatus, mError);
      mStatus = tBuffer.getError();
      return false;
   }

   // Returning true  means socket was not closed.
   // Returning false means socket was closed.
   mRxCount++;
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

UdpTxMsgSocket::UdpTxMsgSocket()
{
   mTxCount = 0;
   mTxLength = 0;
   mValidFlag = false;
   mMonkey = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

UdpTxMsgSocket::~UdpTxMsgSocket()
{
   if (mMonkey) delete mMonkey;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize variables.

void UdpTxMsgSocket::initialize(Settings* aSettings)
{
   // Store the settings.
   mSettings = aSettings;

   // Create a message monkey.
   mMonkey = mSettings->mMonkeyCreator->createMonkey();

   // Metrics.
   mTxCount = 0;
   mTxLength = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Configure the socket.

void UdpTxMsgSocket::configure()
{
   // Configure the socket.
   BaseClass::mRemote.set(mSettings->mRemoteIpAddr, mSettings->mRemoteIpPort);
   BaseClass::doSocket();

   // Set valid flag from base class results.
   mValidFlag = BaseClass::mStatus == 0;

   // Show.
   if (mValidFlag)
   {
      Prn::print(Prn::SocketInit2, "UdpTxMsgSocket     $ %16s : %d",
         BaseClass::mRemote.mIpAddr.mString,
         BaseClass::mRemote.mPort);
   }
   else
   {
      Prn::print(Prn::SocketInit2, "UdpTxMsgSocket     $ %16s : %d $ %d %d",
         BaseClass::mRemote.mIpAddr.mString,
         BaseClass::mRemote.mPort,
         BaseClass::mStatus,
         BaseClass::mError);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Copy a message into a byte buffer and then send the byte buffer out the 
// socket.

bool UdpTxMsgSocket::doSendMsg(ByteContent* aMsg)
{
   // Guard.
   if (!mValidFlag) return false;

   // Create a byte buffer.
   ByteBuffer tBuffer(mMonkey->getMaxBufferSize());

   // Configure the byte buffer.
   mMonkey->configureByteBuffer(&tBuffer);
   tBuffer.setCopyTo();

   // Copy the message to the buffer.
   mMonkey->putMsgToBuffer(&tBuffer, aMsg);

   // Delete the message.
   delete aMsg;

   // Mutex.
   mTxMutex.lock();

   // Transmit the buffer.
   mTxLength = tBuffer.getLength();
   doSendTo(mRemote, tBuffer.getBaseAddress(), mTxLength);

   Prn::print(Prn::SocketRun2, "UdpTxMsgSocket tx message %d", mTxLength);

   // Mutex.
   mTxMutex.unlock();

   // Done.
   mTxCount++;
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

