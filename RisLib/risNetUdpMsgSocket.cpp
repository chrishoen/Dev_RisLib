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

UdpRxMsgSocket::UdpRxMsgSocket()
{
   mRxMemory = 0;
   mMemorySize = 0;
   mRxLength = 0;
   mRxCount = 0;
   mValidFlag = false;
   mMsgMonkey = 0;
   mTI = 0;
}

UdpRxMsgSocket::~UdpRxMsgSocket()
{
   if (mRxMemory) free(mRxMemory);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize variables.

void UdpRxMsgSocket::initialize(Settings& aSettings)
{
   // Store the settings pointer.
   mSettings = aSettings;

   // Store the message monkey.
   mMsgMonkey = mSettings.mMsgMonkey;

   // Allocate memory for byte buffers.
   mMemorySize = mMsgMonkey->getMaxBufferSize();
   mRxMemory = (char*)malloc(mMemorySize);

   // Metrics.
   mRxCount = 0;
   mRxLength = 0;

   // Trace.
   mTI = mSettings.mTraceIndex;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Configure the socket.

void UdpRxMsgSocket::configure()
{
   // Configure the socket.
   BaseClass::mLocal.setForAny(mSettings.mLocalIpPort);
   BaseClass::doSocket();
   BaseClass::doBind();

   // Set valid flag from base class results.
   mValidFlag = BaseClass::mStatus == 0;

   // Show.
   if (mValidFlag)
   {
      Trc::write(mTI, 0, "UdpRxMsgSocket     PASS %16s : %5d",
         BaseClass::mLocal.mString,
         BaseClass::mLocal.mPort);

      printf("UdpRxMsgSocket     PASS %16s : %5d\n",
         BaseClass::mLocal.mString,
         BaseClass::mLocal.mPort);
   }
   else
   {
      Trc::write(mTI, 0, "UdpRxMsgSocket     FAIL %16s : %5d $ %d %d",
         BaseClass::mLocal.mString,
         BaseClass::mLocal.mPort,
         BaseClass::mStatus,
         BaseClass::mError);

      printf("UdpRxMsgSocket     FAIL %16s : %5d $ %d %d\n",
         BaseClass::mLocal.mString,
         BaseClass::mLocal.mPort,
         BaseClass::mStatus,
         BaseClass::mError);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive a message from the socket with a blocking recv call into a
// byte buffer and extract a message from the byte buffer. Return the
// message and true if successful. As part of the termination process,
// returning false means that the socket was closed or that there was
// an error.

bool UdpRxMsgSocket::doReceiveMsg(ByteContent*& aMsg)
{
   Trc::write(mTI, 0, "UdpRxMsgSocket::doReceiveMsg");
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize.

   // Do this first.
   aMsg = 0;

   // Guard.
   if (!mValidFlag)
   {
      Trc::write(mTI, 0, "ERROR UdpRxMsgSocket INVALID SOCKET");
      printf("ERROR UdpRxMsgSocket INVALID SOCKET\n");
      return false;
   }

   // Create a byte buffer from preallocated memory.
   ByteBuffer tByteBuffer(mRxMemory, mMemorySize);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read the message into the receive buffer.

   tByteBuffer.setCopyFrom();
   BaseClass::doRecvFrom(mFromAddress, tByteBuffer.getBaseAddress(), mRxLength, mMemorySize);

   // Guard.
   // If bad status then return false.
   // Returning true  means socket was not closed.
   // Returning false means socket was closed.
   if (mRxLength <= 0)
   {
      if (BaseClass::mError == 0)
      {
         Trc::write(mTI, 0, "UdpRxMsgSocket CLOSED");
      }
      else
      {
         Trc::write(mTI, 0, "ERROR UdpRxMsgSocket %d %d", BaseClass::mStatus, BaseClass::mError);
         printf("ERROR UdpRxMsgSocket %d %d\n", BaseClass::mStatus, BaseClass::mError);
      }
      return false;
   }

   //printf("UdpRxMsgSocket rx message %d\n", mRxLength);
   //printf("UdpRxMsgSocket     FROM %16s : %5d\n",
   //   mFromAddress.mString,
   //   mLocal.mPort);

   // Set the buffer length.
   tByteBuffer.setLength(mRxLength);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Copy from the receive buffer into the message monkey and validate
   // the header.

   // Extract the header.
   mMsgMonkey->extractMessageHeaderParms(&tByteBuffer);

   //printf("UdpRxMsgSocket rx header %d %d\n",
   //   mMsgMonkey->mHeaderValidFlag,
   //   mMsgMonkey->mHeaderLength);

   // If the header is not valid then error.
   if (!mMsgMonkey->mHeaderValidFlag)
   {
      Trc::write(mTI, 0, "ERROR UdpRxMsgSocket INVALID HEADER %d %d", mStatus, mError);
      printf("ERROR UdpRxMsgSocket INVALID HEADER %d %d\n", mStatus, mError);
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
      Trc::write(mTI, 0, "ERROR UdpRxMsgSocket INVALID MESSAGE %d %d", mStatus, mError);
      printf("ERROR UdpRxMsgSocket INVALID MESSAGE %d %d\n", mStatus, mError);
      mStatus = tByteBuffer.getError();
      return false;
   }

   // Metrics.
   mMsgMonkey->mRxMsgMetrics->update(aMsg, mMsgMonkey->mMessageLength);

   // Done.
   Trc::write(mTI, 1, "UdpRxMsgSocket doReceiveMsg done %d %d", mStatus, mError);
   mRxCount++;
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

UdpTxMsgSocket::UdpTxMsgSocket()
{
   mTxMemory = 0;
   mMemorySize = 0;
   mTxCount = 0;
   mTxLength = 0;
   mValidFlag = false;
   mMsgMonkey = 0;
   mTI = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

UdpTxMsgSocket::~UdpTxMsgSocket()
{
   if (mTxMemory) free(mTxMemory);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize variables.

void UdpTxMsgSocket::initialize(Settings& aSettings)
{
   // Store the settings.
   mSettings = aSettings;

   // Store the message monkey.
   mMsgMonkey = mSettings.mMsgMonkey;

   // Allocate memory for byte buffers.
   mMemorySize = mMsgMonkey->getMaxBufferSize();
   mTxMemory = (char*)malloc(mMemorySize);

   // Metrics.
   mTxCount = 0;
   mTxLength = 0;

   // Trace.
   mTI = mSettings.mTraceIndex;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Configure the socket.

void UdpTxMsgSocket::configure()
{
   // Configure the socket.
   BaseClass::mRemote.setByHostName(mSettings.mRemoteIpAddr, mSettings.mRemoteIpPort);
   BaseClass::doSocket();

   // Set valid flag from base class results.
   mValidFlag = BaseClass::mStatus == 0 && BaseClass::mRemote.mValid;

   // Show.
   if (mValidFlag)
   {
      Trc::write(mTI, 0, "UdpTxMsgSocket     PASS %16s : %5d",
         BaseClass::mRemote.mString,
         BaseClass::mRemote.mPort);

      printf("UdpTxMsgSocket     PASS %16s : %5d\n",
         BaseClass::mRemote.mString,
         BaseClass::mRemote.mPort);
   }
   else
   {
      Trc::write(mTI, 0, "UdpTxMsgSocket     FAIL %16s : %5d $ %d %d",
         BaseClass::mRemote.mString,
         BaseClass::mRemote.mPort,
         BaseClass::mStatus,
         BaseClass::mError);

      printf("UdpTxMsgSocket     FAIL %16s : %5d $ %d %d\n",
         BaseClass::mRemote.mString,
         BaseClass::mRemote.mPort,
         BaseClass::mStatus,
         BaseClass::mError);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Copy a message into a byte buffer and then send the byte buffer to the
// socket with a blocking send call. Return true if successful.
// It is protected by the transmit mutex.

bool UdpTxMsgSocket::doSendMsg(ByteContent* aMsg)
{
   Trc::write(mTI, 0, "UdpTxMsgSocket::doSendMsg");
   // Guard.
   if (!mValidFlag)
   {
      Trc::write(mTI, 0, "ERROR UdpTxMsgSocket INVALID SOCKET");
      printf("ERROR UdpTxMsgSocket INVALID SOCKET\n");
      delete aMsg;
      return false;
   }

   // Create a byte buffer from preallocated memory.
   ByteBuffer tByteBuffer(mTxMemory, mMemorySize);

   // Copy the message to the buffer.
   mMsgMonkey->putMsgToBuffer(&tByteBuffer, aMsg);

   // Transmit the buffer.
   mTxLength = tByteBuffer.getLength();
   bool tRet = doSendTo(mRemote, tByteBuffer.getBaseAddress(), mTxLength);
   mTxCount++;

   if (tRet)
   {
      //printf("UdpTxMsgSocket tx message %d\n", mTxLength);
   }
   else
   {
      Trc::write(mTI, 0, "ERROR UdpTxMsgSocket INVALID SEND");
      printf("ERROR UdpTxMsgSocket INVALID SEND\n");
   }

   // Metrics.
   mMsgMonkey->mTxMsgMetrics->update(aMsg, mTxLength);

   // Delete the message.
   delete aMsg;

   // Done.
   Trc::write(mTI, 1, "UdpRxMsgSocket doSendMsg done %d", mTxLength);
   return tRet;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

