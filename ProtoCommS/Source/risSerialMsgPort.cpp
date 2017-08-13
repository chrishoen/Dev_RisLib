/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <stdlib.h>

#include "prnPrint.h"

#include "risSerialMsgPort.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

SerialMsgPort::SerialMsgPort()
{
   mTxMsgCount=0;
   mRxMsgCount=0;
   mMonkey=0;
}

SerialMsgPort::~SerialMsgPort()
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
// Configure the serial port.

void SerialMsgPort::configure(
   BaseMsgMonkeyCreator*  aMonkeyCreator,
   int                    aPortNumber,
   char*                  aPortSetup,
   int                    aRxTimeout)
{
   mTxMsgCount=0;
   mRxMsgCount=0;

   mPortNumber = aPortNumber;
   strcpy(mPortSetup,aPortSetup);

   BaseClass::doOpen(mPortNumber,mPortSetup,aRxTimeout);

   mMonkey = aMonkeyCreator->createMonkey();

   if (mMonkey==0) mMonkey = aMonkeyCreator->createMonkey();

   if (BaseClass::mValidFlag)
   {
      Prn::print(Prn::SerialInit2, "SerialMsgPort configure PASS  $ %d : %16s",
         BaseClass::mPortNumber,
         BaseClass::mPortSetup);
   }
   else
   {
      Prn::print(Prn::SerialInit2, "SerialMsgPort configure FAIL  $ %d : %16s",
         BaseClass::mPortNumber,
         BaseClass::mPortSetup);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This copies a message into a byte buffer and then sends the byte buffer 
// out the socket.

bool SerialMsgPort::doSendMsg(ByteContent* aMsg)
{
   Prn::print(0, "LINE101");
   // Guard.
   if (!BaseClass::mValidFlag)
   {
      Prn::print(Prn::SerialRun2, "ERROR doSend when Invalid");
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

   Prn::print(0, "LINE102");

   // Transmit the buffer
   int tRet = 0;
   int tLength=tBuffer.getLength();
   tRet = doSendBytes(tBuffer.getBaseAddress(),tLength);
   Prn::print(Prn::SerialRun4, "doSendM %d %d",tRet,tLength);

   Prn::print(0, "LINE103");
   mTxMsgCount++;

   // Mutex
   mTxMutex.unlock();

   if (!tRet)
   {
      Prn::print(Prn::SerialRun2, "ERROR SerialMsgPort::doSendMsg FAIL");
   }

   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This receives data from the socket into a byte buffer and then
// extracts a message from the byte buffer

bool SerialMsgPort::doReceiveMsg (ByteContent*& aMsg)
{
   //-------------------------------------------------------------------------
   // Initialize
   aMsg=0;
   int tRet=0;

   // Create a byte buffer.
   ByteBuffer tBuffer(mMonkey->getMaxBufferSize());

   // Configure the byte buffer.
   mMonkey->configureByteBuffer(&tBuffer);
   tBuffer.setCopyTo();

   //-------------------------------------------------------------------------
   // Read the message header into the receive buffer

   int   tHeaderLength = mMonkey->getHeaderLength();
   char* tHeaderBuffer = tBuffer.getBaseAddress();

   tRet = doReceiveBytes(tHeaderBuffer,tHeaderLength);
   Prn::print(Prn::SerialRun4, "doRecvH %d",tRet);

   // Guard
   // If bad status then return false.

   if (!tRet)
   {
      return false;
   }

   // Set the buffer length
   tBuffer.setLength(tHeaderLength);

   //--------------------------------------------------------------
   // Copy from the receive buffer into the message monkey object
   // and validate the header

   mMonkey->extractMessageHeaderParms(&tBuffer);

   // If the header is not valid then error
   if (!mMonkey->mHeaderValidFlag)
   {
      Prn::print(Prn::SerialRun1, "ERROR doRecv1 INVALID HEADER");
      return false;
   }

   //-------------------------------------------------------------------------
   // Read the message payload into the receive buffer

   int   tPayloadLength = mMonkey->mPayloadLength;
   char* tPayloadBuffer = tBuffer.getBaseAddress() + tHeaderLength;

   tRet=doReceiveBytes(tPayloadBuffer,tPayloadLength);
   Prn::print(Prn::SerialRun4, "doRecvP %d %d",tRet,tPayloadLength);

   // Guard
   // If bad status then return false.

   if (!tRet)
   {
      Prn::print(Prn::SerialRun1, "ERROR doRecv2 INVALID RECV");
      return false;
   }

   // Set the buffer length
   tBuffer.setLength(mMonkey->mMessageLength);

   //--------------------------------------------------------------
   // At this point the buffer contains the complete message.
   // Extract the message from the byte buffer into a new message
   // object and return it.

   tBuffer.rewind();
   aMsg = mMonkey->getMsgFromBuffer(&tBuffer);

   // Test for errors.
   if (aMsg==0)
   {
      Prn::print(Prn::SerialRun1, "ERROR getMsgFromBuffer");
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

