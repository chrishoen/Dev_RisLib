/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "prnPrint.h"
#include "trcTrace.h"

#include "risNetTcpMsgSocket.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.
   
TcpMsgSocket::TcpMsgSocket()
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

TcpMsgSocket::~TcpMsgSocket()
{
   if (mRxMemory) free(mRxMemory);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize variables.

void TcpMsgSocket::initialize(Settings* aSettings)
{
   // Store a copy of the settings.
   mSettings = *aSettings;

   // Store the message monkey.
   mMsgMonkey = mSettings.mMsgMonkey;

   // Allocate memory for byte buffers.
   mMemorySize = mMsgMonkey->getMaxBufferSize();
   mRxMemory = (char*)malloc(mMemorySize);

   // Trace.
   mTI = mSettings.mTraceIndex;

   // Not valid until configured.
   mValidFlag = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Configure the socket.

void TcpMsgSocket::configure(bool aShowFlag)
{
   // Configure the socket.
   BaseClass::reset();
   BaseClass::mRemote.setByAddress(mSettings.mRemoteIpAddr, mSettings.mRemoteIpPort);
   if (!BaseClass::doSocket()) goto ConfigDone;
   if (!BaseClass::setOptionKeepAlive()) goto ConfigDone;
   if (!BaseClass::setOptionNoDelay()) goto ConfigDone;
   if (!BaseClass::setOptionDontLinger()) goto ConfigDone;
   mValidFlag = true;

ConfigDone:

   // Show.
   if (mValidFlag && aShowFlag)
   {
      Trc::write(mTI, 0, "TcpMsgSocket       PASS %16s : %5d",
         BaseClass::mRemote.mString,
         BaseClass::mRemote.mPort);
      if (aShowFlag)
      {
         printf("TcpMsgSocket       PASS %16s : %5d\n",
            BaseClass::mRemote.mString,
            BaseClass::mRemote.mPort);
      }
   }
   else if (!mValidFlag)
   {
      Trc::write(mTI, 0, "TcpMsgSocket       FAIL %16s : %5d $ %d %d",
         BaseClass::mRemote.mString,
         BaseClass::mRemote.mPort,
         BaseClass::mStatus,
         BaseClass::mError);
      if (aShowFlag)
      {
         printf("TcpMsgSocket       FAIL %16s : %5d $ %d %d\n",
            BaseClass::mRemote.mString,
            BaseClass::mRemote.mPort,
            BaseClass::mStatus,
            BaseClass::mError);
      }
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

bool TcpMsgSocket::doReceiveMsg (ByteContent*& aMsg)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize a receive byte buffer.

   // Do this first.
   aMsg=0;
   bool tRet=false;
   int tStatus=0;

   // Guard.
   if (!mValidFlag)
   {
      Trc::write(mTI, 0, "TcpMsgSocket ERROR INVALID SOCKET");
      printf("TcpMsgSocket ERROR INVALID SOCKET\n");
      return false;
   }

   // Create a byte buffer from preallocated memory.
   ByteBuffer tByteBuffer(mRxMemory, mMemorySize);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read the message header into the receive buffer.

   // Header varaibles.
   int   tHeaderLength = mMsgMonkey->getHeaderLength();
   char* tHeaderBuffer = tByteBuffer.getBaseAddress();

   // Read the header from the socket.
   tByteBuffer.setCopyTo();
   tRet = BaseClass::doRecv(tHeaderBuffer,tHeaderLength,tStatus);
   //printf("doRecvH %d %d\n",mStatus,mError);

   // Guard. If bad status then return false.
   if (!tRet || tStatus<=0)
   {
      Trc::write(mTI, 0, "TcpMsgSocket ERROR INVALID READ %d", mError);
      return false;
   }
   Trc::write(mTI, 9, "TcpMsgSocket::doReceiveMsg recv header");

   // Set the buffer length.
   tByteBuffer.setLength(tHeaderLength);

   // Copy from the receive buffer into the message monkey 
   // and validate the header.
   mMsgMonkey->extractMessageHeaderParms(&tByteBuffer);

   // If the header is not valid then error.
   if (!mMsgMonkey->mHeaderValidFlag)
   {
      Trc::write(mTI, 0, "TcpMsgSocket ERROR INVALID HEADER");
      printf("TcpMsgSocket ERROR INVALID HEADER\n");
      return false;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read the message payload into the receive buffer.

   // Payload variables.
   int   tPayloadLength = mMsgMonkey->mPayloadLength;
   char* tPayloadBuffer = tByteBuffer.getBaseAddress() + tHeaderLength;

   // Read the payload from the socket.
   tByteBuffer.setCopyTo();
   tRet = BaseClass::doRecv(tPayloadBuffer,tPayloadLength,tStatus);
   //printf("doRecvP %d %d %d\n",mStatus,mError,tPayloadLength);

   // If bad status then return false.
   if (!tRet || tStatus<=0)
   {
      Trc::write(mTI, 0, "TcpMsgSocket ERROR INVALID RECV");
      printf("TcpMsgSocket ERROR INVALID RECV\n");
      return false;
   }
   Trc::write(mTI, 9, "TcpMsgSocket::doReceiveMsg payload %d", tPayloadLength);

   // Set the buffer length.
   tByteBuffer.setLength(mMsgMonkey->mMessageLength);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // At this point the buffer contains the complete message. Extract the 
   // message from the byte buffer into a new message object and return it.

   // Extract the message.
   tByteBuffer.rewind();
   aMsg = mMsgMonkey->getMsgFromBuffer(&tByteBuffer);

   // Test for errors.
   if (aMsg==0)
   {
      Trc::write(mTI, 0, "TcpMsgSocket ERROR INVALID MESSAGE");
      printf("TcpMsgSocket ERROR INVALID MESSAGE\n");
      mStatus=tByteBuffer.getError();
      return false;
   }

   // Metrics.
   mMsgMonkey->updateRxMsgMetrics(aMsg, mMsgMonkey->mMessageLength);

   // Done.
   Trc::write(mTI, 1, "doReceiveMsg %d %d", mRxCount, mMsgMonkey->mMessageLength);
   mRxCount++;
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Copy a message into a byte buffer and then send the byte buffer to the
// socket with a blocking send call. Return true if successful.

bool TcpMsgSocket::doSendMsg(ByteContent* aMsg)
{
   // Guard.
   if (!mValidFlag)
   {
      Trc::write(mTI, 0, "TcpMsgSocket ERROR INVALID SOCKET");
      printf("TcpMsgSocket ERROR INVALID SOCKET\n");
      delete aMsg;
      return false;
   }

   // Create a byte buffer using heap memory.
   ByteBuffer tByteBuffer(mMemorySize);

   // Copy the message to the buffer.
   mMsgMonkey->putMsgToBuffer(&tByteBuffer, aMsg);

   // Transmit the buffer.
   int tTxLength = tByteBuffer.getLength();
   bool tRet = BaseClass::doSend(tByteBuffer.getBaseAddress(), tTxLength);

   if (tRet)
   {
      //printf("TcpMsgSocket tx message %d\n", mTxLength);
   }
   else
   {
      Trc::write(mTI, 0, "TcpMsgSocket ERROR INVALID SEND %d\n", mError);
      printf("TcpMsgSocket ERROR INVALID SEND %d\n", mError);
   }

   // Metrics.
   mMsgMonkey->updateTxMsgMetrics(aMsg, mTxLength);

   // Delete the message.
   delete aMsg;

   // Metrics.
   mTxCount++;
   mTxLength = tTxLength;

   // Done. Return true if successful.
   Trc::write(mTI, 1, "doSendMsg    %d %d", mTxCount, tTxLength);
   return tRet;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

