//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"
#include "trcTrace.h"

#include "risNetUdpMsgSocket.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

UdpMsgSocket::UdpMsgSocket()
{
   mRxMemory = 0;
   mMemorySize = 0;
   mTxLength = 0;
   mRxLength = 0;
   mTxCount = 0;
   mRxCount = 0;
   mValidFlag = false;
   mMsgMonkey = 0;
   mTI = 0;
}

UdpMsgSocket::~UdpMsgSocket()
{
   if (mRxMemory) free(mRxMemory);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize variables.

void UdpMsgSocket::initialize(Settings* aSettings)
{
   // Store a copy of the settings.
   mSettings = *aSettings;

   // Store the message monkey.
   mMsgMonkey = mSettings.mMsgMonkey;

   // Allocate memory for byte buffers.
   mMemorySize = mMsgMonkey->getMaxBufferSize();
   mRxMemory = (char*)malloc(mMemorySize);

   // Metrics.
   mTxLength = 0;
   mRxLength = 0;
   mTxCount = 0;
   mRxCount = 0;

   // Trace.
   mTI = mSettings.mTraceIndex;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Configure the socket.

void UdpMsgSocket::configure()
{
   // Reset the socket.
   BaseClass::reset();

   // Set the socket receive address.
   if (mSettings.mLocalIpPort)
   {
      BaseClass::mLocal.setByAddress(mSettings.mLocalIpAddr, mSettings.mLocalIpPort);
   }

   // Set the socket send address.
   if (mSettings.mRemoteIpPort)
   {
      BaseClass::mRemote.setByAddress(mSettings.mRemoteIpAddr, mSettings.mRemoteIpPort);
   }

   // Configure the socket.
   if (!BaseClass::doSocket()) goto ConfigDone;


   // If listening then bind the socket to the receive address.
   if (BaseClass::mLocal.mPort)
   {
      if (!BaseClass::setOptionReuseAddr()) goto ConfigDone;
      if (!BaseClass::doBind()) goto ConfigDone;
   }

   // Test for broadcast.
   if (mSettings.mUdpBroadcast)
   {
      BaseClass::mRemote.setForBroadcast(mSettings.mRemoteIpPort);
      if (!BaseClass::setOptionBroadcast()) goto ConfigDone;
   }

   // Set valid.
   mValidFlag = true;

ConfigDone:

   // Show.
   if (mValidFlag)
   {
      if (!mSettings.mUdpWrapFlag)
      {
         Trc::write(mTI, 0, "UdpMsgSocket       PASS %16s : %5d Tx %16s : %5d",
            BaseClass::mLocal.mString,
            BaseClass::mLocal.mPort,
            BaseClass::mRemote.mString,
            BaseClass::mRemote.mPort);

         printf("UdpMsgSocket       PASS %16s : %5d Tx %16s : %5d\n",
            BaseClass::mLocal.mString,
            BaseClass::mLocal.mPort,
            BaseClass::mRemote.mString,
            BaseClass::mRemote.mPort);
      }
      else
      {
         Trc::write(mTI, 0, "UdpMsgSocket       PASS %16s : %5d Tx WRAP",
            BaseClass::mLocal.mString,
            BaseClass::mLocal.mPort);

         printf("UdpMsgSocket       PASS %16s : %5d Tx WRAP\n",
            BaseClass::mLocal.mString,
            BaseClass::mLocal.mPort);
      }
   }
   else
   {
      if (!mSettings.mUdpWrapFlag)
      {
         Trc::write(mTI, 0, "UdpMsgSocket       FAIL %16s : %5d Tx %16s : %5d  $ %d %d",
            BaseClass::mLocal.mString,
            BaseClass::mLocal.mPort,
            BaseClass::mRemote.mString,
            BaseClass::mRemote.mPort,
            BaseClass::mStatus,
            BaseClass::mError);

         printf("UdpMsgSocket       FAIL %16s : %5d Tx %16s : %5d  $ %d %d\n",
            BaseClass::mLocal.mString,
            BaseClass::mLocal.mPort,
            BaseClass::mRemote.mString,
            BaseClass::mRemote.mPort,
            BaseClass::mStatus,
            BaseClass::mError);
      }
      else
      {
         Trc::write(mTI, 0, "UdpMsgSocket       FAIL %16s : %5d Tx WRAP  $ %d %d",
            BaseClass::mLocal.mString,
            BaseClass::mLocal.mPort,
            BaseClass::mStatus,
            BaseClass::mError);

         printf("UdpMsgSocket       FAIL %16s : %5d Tx WRAP  $ %d %d\n",
            BaseClass::mLocal.mString,
            BaseClass::mLocal.mPort,
            BaseClass::mStatus,
            BaseClass::mError);
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive a message from the socket with a blocking recv call into a
// byte buffer and then extract a message from the byte buffer. Return
// the message and true if successful.

bool UdpMsgSocket::doReceiveMsg(ByteContent*& aMsg)
{
   Trc::write(mTI, 0, "UdpMsgSocket::doReceiveMsg");
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize.

   // Do this first.
   aMsg = 0;

   // Guard.
   if (!mValidFlag)
   {
      Trc::write(mTI, 0, "UdpMsgSocket::doReceiveMsg  ERROR INVALID SOCKET");
      printf("UdpMsgSocket::doReceiveMsg ERROR INVALID SOCKET\n");
      return false;
   }

   // Create a byte buffer at preallocated memory.
   ByteBuffer tByteBuffer(mRxMemory, mMemorySize);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read the message into the receive buffer.

   tByteBuffer.setCopyFrom();
   BaseClass::doRecvFrom(mFromAddress, tByteBuffer.getBaseAddress(), mRxLength, mMemorySize);

   // Guard. If bad status then return false.
   if (mRxLength <= 0)
   {
      if (BaseClass::mError == 0)
      {
         Trc::write(mTI, 0, "UdpMsgSocket::doReceiveMsg CLOSED");
      }
      else
      {
         Trc::write(mTI, 0, "UdpMsgSocket::doReceiveMsg ERROR %d %d", BaseClass::mStatus, BaseClass::mError);
         printf("UdpMsgSocket::doReceiveMsg ERROR %d %d\n", BaseClass::mStatus, BaseClass::mError);
      }
      return false;
   }

   // Set the buffer length.
   tByteBuffer.setLength(mRxLength);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Copy from the receive buffer into the message monkey and validate
   // the header.

   // Extract the header.
   mMsgMonkey->extractMessageHeaderParms(&tByteBuffer);

   // If the header is not valid then error.
   if (!mMsgMonkey->mHeaderValidFlag)
   {
      Trc::write(mTI, 0, "UdpMsgSocket::doReceiveMsg ERROR INVALID HEADER %d %d", mStatus, mError);
      printf("UdpMsgSocket::doReceiveMsg ERROR INVALID HEADER %d %d\n", mStatus, mError);
      return false;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // At this point the buffer contains the complete message. Extract the
   // message from the byte buffer into a new message object and return it.

   // Extract the message.
   tByteBuffer.rewind();
   aMsg = mMsgMonkey->getMsgFromBuffer(&tByteBuffer);

   // Test for errors.
   if (aMsg == 0)
   {
      Trc::write(mTI, 0, "UdpMsgSocket::doReceiveMsg ERROR INVALID MESSAGE %d %d", mStatus, mError);
      printf("UdpMsgSocket::doReceiveMsg ERROR INVALID MESSAGE %d %d\n", mStatus, mError);
      mStatus = tByteBuffer.getError();
      return false;
   }

   // Metrics.
   mMsgMonkey->updateRxMsgMetrics(aMsg, mMsgMonkey->mMessageLength);

   // Done.
   Trc::write(mTI, 1, "UdpMsgSocket::doReceiveMsg done %d %d", mStatus, mError);
   mRxCount++;
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Copy a message into a byte buffer and then send the byte buffer
// to the socket with a send call. Return true if successful.

bool UdpMsgSocket::doSendMsg(ByteContent* aMsg)
{
   Trc::write(mTI, 0, "UdpMsgSocket::doSendMsg");
   // Guard.
   if (!mValidFlag)
   {
      Trc::write(mTI, 0, "UdpMsgSocket::doSendMsg ERROR INVALID SOCKET");
      printf("UdpMsgSocket::doSendMsg ERROR INVALID SOCKET\n");
      delete aMsg;
      return false;
   }

   // Create a byte buffer on the heap.
   ByteBuffer tByteBuffer(mMemorySize);

   // Copy the message to the buffer.
   mMsgMonkey->putMsgToBuffer(&tByteBuffer, aMsg);

   // Transmit the buffer.
   int tTxLength = tByteBuffer.getLength();
   bool tRet = false;
   if (!mSettings.mUdpWrapFlag)
   {
      // If this is not wrapping then send to the remote address.
      tRet = doSendTo(mRemote, tByteBuffer.getBaseAddress(), tTxLength);
   }
   else if(mRxCount)
   {
      // If this is wrapping then send to the last received from address, 
      // if it is valid (rx count is greater than zero).
      tRet = doSendTo(mFromAddress, tByteBuffer.getBaseAddress(), tTxLength);
   }

   if (tRet)
   {
      // The send was successful. Update metrics.
      mTxCount++;
      mTxLength = tTxLength;
      mMsgMonkey->updateTxMsgMetrics(aMsg, tTxLength);
   }
   else
   {
      // The send was not successful.
      Trc::write(mTI, 0, "UdpMsgSocket::doSendMsg ERROR INVALID SEND");
      printf("UdpMsgSocket::doSendMsg ERROR INVALID SEND\n");
      // Set the socket invalid and close it.
      mValidFlag = false;
      doClose();
   }

   // Delete the message.
   delete aMsg;

   // Done. Return true if successful.
   Trc::write(mTI, 1, "UdpMsgSocket::doSendMsg done %d", mTxLength);
   return tRet;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

