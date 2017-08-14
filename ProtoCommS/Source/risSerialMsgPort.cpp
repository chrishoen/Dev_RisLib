/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <stdlib.h>

#include "prnPrint.h"

#include "risSerialMsgPort.h"
#include "risSerialHeaderBuffer.h"

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
   // Guard.
   if (!BaseClass::mValidFlag)
   {
      Prn::print(Prn::SerialRun2, "ERROR doSend when Invalid");
      delete aMsg;
      return false;
   }

   // Create a byte buffer.
   ByteBuffer tByteBuffer(mMonkey->getMaxBufferSize());

   // Configure the byte buffer.
   mMonkey->configureByteBuffer(&tByteBuffer);
   tByteBuffer.setCopyTo();

   // Copy the message to the buffer.
   mMonkey->putMsgToBuffer(&tByteBuffer,aMsg);

   // Delete the message.
   delete aMsg;

   // Mutex.
   mTxMutex.lock();

   // Transmit the buffer
   int tRet = 0;
   int tLength=tByteBuffer.getLength();
   tRet = doSendBytes(tByteBuffer.getBaseAddress(),tLength);
   Prn::print(Prn::SerialRun4, "doSendMsg %d %d",tRet,tLength);

   mTxMsgCount++;

   // Mutex
   mTxMutex.unlock();

   if (tRet)
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
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize.

   aMsg=0;
   int tRet=0;

   // Create a byte buffer.
   ByteBuffer tByteBuffer(mMonkey->getMaxBufferSize());

   // Configure the byte buffer.
   mMonkey->configureByteBuffer(&tByteBuffer);
   tByteBuffer.setCopyTo();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read from the serial port into a serial header buffer.

   // Header length.
   int tHeaderLength = mMonkey->getHeaderLength();

   // Serial header buffer.
   SerialHeaderBuffer tHeaderBuffer(mMonkey->getHeaderLength());

   // Loop through received byte stream to extract the message header.
   bool tGoing = true;
   while (tGoing)
   {
      // Read one byte.
      char tByte;
      BaseClass::doReceiveOne(&tByte);

      // Put it to the header buffer.
      tHeaderBuffer.put(tByte);
       
      // If the header buffer is full.
      if (tHeaderBuffer.isFull())
      {
         // Copy the header buffer to the byte buffer.
         tByteBuffer.setCopyTo();
         tByteBuffer.reset();
         tHeaderBuffer.copyTo(&tByteBuffer);

         // Copy from the byte buffer into the message monkey object
         // and validate the header.
         tByteBuffer.setCopyFrom();
         mMonkey->extractMessageHeaderParms(&tByteBuffer);

         // If the header is valid then exit the loop.
         if (mMonkey->mHeaderValidFlag)
         {
            Prn::print(Prn::SerialRun1, "doRecv1 HEADER PASS");
            tGoing = false;
         }
      }
   }
   
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read the message payload into the receive buffer.

   int   tPayloadLength = mMonkey->mPayloadLength;
   char* tPayloadBuffer = tByteBuffer.getBaseAddress() + tHeaderLength;

   tRet=doReceiveBytes(tPayloadBuffer,tPayloadLength);
   Prn::print(Prn::SerialRun4, "doRecvP %d %d",tRet,tPayloadLength);

   // If bad status then return false.
   if (!tRet)
   {
      Prn::print(Prn::SerialRun1, "ERROR doRecv2 INVALID RECV");
      return false;
   }

   // Set the buffer length
   tByteBuffer.setLength(mMonkey->mMessageLength);
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   //--------------------------------------------------------------
   // At this point the buffer contains the complete message.
   // Extract the message from the byte buffer into a new message
   // object and return it.

   tByteBuffer.rewind();
   aMsg = mMonkey->getMsgFromBuffer(&tByteBuffer);

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

