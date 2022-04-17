/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "my_functions.h"
#include "prnPrint.h"
#include "trcTrace.h"

#include "risSerialMsgPort.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

SerialMsgPort::SerialMsgPort()
{
   mRxMemory = 0;
   mMemorySize = 0;
   mTxMsgCount = 0;
   mRxMsgCount = 0;
   mHeaderAllCount = 0;
   mHeaderOneCount = 0;
   mMsgMonkey = 0;
   mHeaderReadState = 0;
   mHeaderLength = 0;
}

SerialMsgPort::~SerialMsgPort()
{
   if (mRxMemory) free(mRxMemory);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize the serial port variables.

void SerialMsgPort::initialize(SerialSettings& aSettings)
{
   // Initialize the base class.
   BaseClass::initialize(aSettings);

   // Create a message monkey.
   mMsgMonkey = BaseClass::mSettings.mMsgMonkey;

   // Allocate memory for byte buffers.
   mMemorySize = mMsgMonkey->getMaxBufferSize();
   mRxMemory = (char*)malloc(mMemorySize);

   // Set the initial header read state.
   mHeaderReadState = cHeaderReadAll;
   mHeaderLength = mMsgMonkey->getHeaderLength();

   // Initialize the header buffer and allocate memory for it.
   mHeaderBuffer.initialize(mHeaderLength);

   // Initialize variables.
   mTxMsgCount = 0;
   mRxMsgCount = 0;
   mHeaderAllCount = 0;
   mHeaderOneCount = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive a message from the serial port with blocking read calls into a
// byte buffer and extract the message from the byte buffer. If the message
// cannot be extracted because the header is incorrect then enter a mode
// to resync the header. Return the message and a status code. If the status
// code is greater than zero then the receive was successful. If the status
// code is less than or equal to zero then there was an abort or error.

int SerialMsgPort::doReceiveMsg (ByteContent*& aMsg)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize a receive byte buffer.

   // Guard.
   aMsg=0;
   if (!BaseClass::mValidFlag) return cSerialRetError;
   int tRet=0;

   // Create a byte buffer from preallocated memory.
   ByteBuffer tByteBuffer(mRxMemory, mMemorySize);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read the header from the serial port into the receive byte buffer.
   // Read all header bytes.
   // 
   // This assumes that header synchronization has been attained and that the
   // first byte that is read will be the first byte in a header. If header
   // synchronization has been lost then this will fail and the state will
   // be switched to read the header one byte at a time until a header
   // is detected and synchronization is attained.

   if (mHeaderReadState == cHeaderReadAll)
   {
      // Read the header from the serial port into the byte buffer.
      tRet = BaseClass::doReceiveAllBytes(tByteBuffer.getBaseAddress(),mHeaderLength);

      // Test the return code.
      if (tRet > 0)
      {
         //printf("receive header all %d\n", tRet);
      }
      else
      {
         //printf("receive header all ERROR %d\n", tRet);
         return tRet;
      }

      // Set the buffer length.
      tByteBuffer.setLength(mHeaderLength);

      // Extract the header parameters from the byte buffer and validate
      // the header.
      mMsgMonkey->extractMessageHeaderParms(&tByteBuffer);

      // If the header is not valid then set the state to read it into 
      // the header buffer one byte at a time.
      if (mMsgMonkey->mHeaderValidFlag)
      {
         //printf("receive header all PASS\n");
         mHeaderAllCount++;
      }
      else
      {
         //printf("receive header all FAIL\n");
         mHeaderReadState = cHeaderReadOne;
      }
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read the header from the serial port into the serial header buffer.
   // Read the header one byte at a time.
   // 
   // This executes when header synchronization has been lost. It reads one
   // byte at a time and puts the received byte into the header buffer to
   // detect a header and attain synchronization. Once a header has been 
   // detected it sets the next state for synchronized and completes.

   if (mHeaderReadState == cHeaderReadOne)
   {
      // Initialize the header buffer, but do not allocate new memory for it.
      mHeaderBuffer.reinitialize();

      // Loop through the received byte stream to extract the message header.
      bool tGoing = true;
      while (tGoing)
      {
         // Read one byte.
         char tByte;
         tRet = BaseClass::doReceiveOneByte(&tByte);

         // Test the return code.
         if (tRet > 0)
         {
            printf("receive header2 %d\n", tRet);
         }
         else
         {
            printf("receive header2 ERROR %d\n", tRet);
            return tRet;
         }

         // Put it to the header buffer.
         mHeaderBuffer.put(tByte);

         // If the header buffer is full.
         if (mHeaderBuffer.isFull())
         {
            // Copy the header buffer to the beginning of the byte buffer.
            tByteBuffer.setCopyTo();
            tByteBuffer.reset();
            mHeaderBuffer.copyTo(&tByteBuffer);

            // Copy from the byte buffer into the message monkey object,
            // extract the header parameters and validate the header.
            tByteBuffer.setCopyFrom();
            tByteBuffer.rewind();
            mMsgMonkey->extractMessageHeaderParms(&tByteBuffer);

            // If the header is valid then set the header state to read
            // the next header with all of the bytes and exit the loop.
            // If the header is not valid then continue with the loop.
            if (mMsgMonkey->mHeaderValidFlag)
            {
               //printf("receive header one PASS\n");
               mHeaderOneCount++;
               mHeaderReadState = cHeaderReadAll;
               tGoing = false;
            }
         }
      }
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read the message payload into the receive buffer.
 
   // Set payload variables.
   int   tPayloadLength = mMsgMonkey->mPayloadLength;
   char* tPayloadBuffer = tByteBuffer.getBaseAddress() + mHeaderLength;

   tRet = BaseClass::doReceiveAllBytes(tPayloadBuffer,tPayloadLength);

   // Test the return code.
   if (tRet > 0)
   {
      //printf("receive payload %d\n", tRet);
   }
   else
   {
      //printf("receive payload ERROR %d\n", tRet);
      return tRet;
   }

   // Set the buffer length.
   tByteBuffer.setLength(mMsgMonkey->mMessageLength);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // At this point the buffer contains the complete message. Extract the 
   // message from the byte buffer into a new message object and return it.

   // Extract the message from the byte buffer.
   tByteBuffer.rewind();
   aMsg = mMsgMonkey->getMsgFromBuffer(&tByteBuffer);

   // Test for errors.
   if (aMsg==0)
   {
      printf("ERROR getMsgFromBuffer\n");
      return cSerialRetDataError;
   }

   // Test for message footer errors.
   if (!mMsgMonkey->validateMessageFooter(&tByteBuffer,aMsg))
   {
      printf("ERROR validateMessageFooter\n");
      return cSerialRetDataError;
   }

   // Metrics.
   mRxMsgCount++;
   mMsgMonkey->mRxMsgMetrics->update(aMsg, mMsgMonkey->mMessageLength);

   // Done.
   return cSerialRetSuccess;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Copy a message into a byte buffer and then send the byte buffer to the
// serial port with a blocking write call. Return true if successful.
// This is protected by the transmit mutex.

bool SerialMsgPort::doSendMsg(ByteContent* aMsg)
{
   Trc::write(mTI, 2, "SerialMsgPort::doSendMsg begin");
   // Guard.
   if (!BaseClass::mValidFlag)
   {
      Trc::write(mTI, 0, "ERROR doSend when Invalid");
      mMsgMonkey->destroyMsg(aMsg);
      return false;
   }

   // Create a byte buffer from heap memory.
   ByteBuffer tByteBuffer(mMemorySize);

   // Copy the message to the buffer.
   mMsgMonkey->putMsgToBuffer(&tByteBuffer, aMsg);

   // Transmit the buffer.
   int tRet = 0;
   int tLength = tByteBuffer.getLength();
   tRet = BaseClass::doSendBytes(tByteBuffer.getBaseAddress(), tLength);

   // Metrics.
   mTxMsgCount++;
   mMsgMonkey->mTxMsgMetrics->update(aMsg, tLength);

   // Delete the message.
   mMsgMonkey->destroyMsg(aMsg);

   // Test for errors.
   if (tRet < 0)
   {
      Trc::write(mTI, 2, "SerialMsgPort::doSendMsg FAIL");
      printf("SerialMsgPort::doSendMsg FAIL\n");
      return false;
   }

   // Success.
   Trc::write(mTI, 2, "SerialMsgPort::doSendMsg PASS");
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

}//namespace

