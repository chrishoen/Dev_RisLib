/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risByteMsgMonkey.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

BaseMsgMonkey::BaseMsgMonkey(
   CreateMsgFunctionT aCreate)
{
   mHeaderLength=0;
   mMessageLength=0;
   mMessageType=0;
   mPayloadLength=0;
   mHeaderValidFlag=false;

   mCreateMsgFunction = aCreate;
   mTxMsgMetrics = &mDefaultTxMsgMetrics;
   mRxMsgMetrics = &mDefaultRxMsgMetrics;

   mDefaultTxMsgMetrics.resetBaseVars();
   mDefaultRxMsgMetrics.resetBaseVars();
}

BaseMsgMonkey::BaseMsgMonkey(
   CreateMsgFunctionT aCreate,
   BaseMsgMetrics* aTxMsgMetrics,
   BaseMsgMetrics* aRxMsgMetrics)
{
   mHeaderLength = 0;
   mMessageLength = 0;
   mMessageType = 0;
   mPayloadLength = 0;
   mHeaderValidFlag = false;

   mCreateMsgFunction = aCreate;

   mTxMsgMetrics = aTxMsgMetrics;
   mRxMsgMetrics = aRxMsgMetrics;
   resetMsgMetrics();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Copy a message to a byte buffer.

void BaseMsgMonkey::putMsgToBuffer(Ris::ByteBuffer* aBuffer, Ris::ByteContent* aMsg)
{
   // Call inheritor's override to preprocess the message before it is sent.
   processBeforeSend(aMsg);

   // Set buffer direction for put.
   aBuffer->setCopyTo();

   // Call inheritor's copier to copy from the message to the buffer.
   aMsg->copyToFrom(aBuffer);

   // Call inheritor's override to postprocess the message after it is sent.
   processAfterSend(aBuffer, aMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Copy a message from a byte buffer.
//
// 1) Create a new message object of the type specifed by the identifiers 
//    that were extracted from the header
// 2) Copy the data from the byte buffer into the new message object
// and returns a pointer to the base class.

Ris::ByteContent* BaseMsgMonkey::getMsgFromBuffer (Ris::ByteBuffer* aBuffer)
{
   // Guard.
   if (!mHeaderValidFlag) return 0;

   // Call inheritor's creator to create a new message based on the
   // message type that was extracted from the header.
   Ris::ByteContent* aMsg = (Ris::ByteContent*)mCreateMsgFunction(mMessageType);

   // Guard
   if (!aMsg) return 0;

   // Set buffer direction for get.
   aBuffer->setCopyFrom();

   // Call inheritor's copier to copy from the buffer to the message.
   aMsg->copyToFrom(aBuffer);

   // Call inheritor's override to postprocess the message after it is received.
   processAfterReceive(aBuffer, aMsg);

   // Done.
   return aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Copy a message from a byte buffer.
//
// 1) Extract the header parameters.
// 2) Create a new message object of the type specifed by the identifiers 
//    that were extracted from the header
// 3) Copy the data from the byte buffer into the new message object
// and returns a pointer to the base class.

Ris::ByteContent* BaseMsgMonkey::makeMsgFromBuffer (Ris::ByteBuffer* aBuffer)
{
   // Set buffer direction for get.
   aBuffer->setCopyFrom();

   // Extract the header parameters.
   aBuffer->rewind();
   extractMessageHeaderParms(aBuffer);
   aBuffer->rewind();

   // Guard.
   if (!mHeaderValidFlag) return 0;

   // Call inheritor's creator to create a new message based on the
   // message type that was extracted from the header.
   Ris::ByteContent* aMsg = (Ris::ByteContent*)mCreateMsgFunction(mMessageType);

   // Guard
   if (!aMsg) return 0;

   // Call inheritor's copier to copy from the buffer to the message.
   aMsg->copyToFrom(aBuffer);

   // Done.
   return aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Metrics.

// Reset the metrics.
void BaseMsgMonkey::resetMsgMetrics()
{
   mTxMsgMetrics->resetBaseVars();
   mRxMsgMetrics->resetBaseVars();
}

// Update the metrics with a message and a length.
void BaseMsgMonkey::updateTxMsgMetrics(ByteContent* aMsg, int aMsgLength)
{
   mTxMsgMetrics->updateBaseVars(aMsg, aMsgLength);
}
void BaseMsgMonkey::updateRxMsgMetrics(ByteContent* aMsg, int aMsgLength)
{
   mRxMsgMetrics->updateBaseVars(aMsg, aMsgLength);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// No constructor.

void BaseMsgMetrics::resetBaseVars()
{
   mMsgCount = 0;
   mByteCount = 0;
}

// Update the metrics with a message and a length.
void BaseMsgMetrics::updateBaseVars(ByteContent* aMsg, int aMsgLength)
{
   mMsgCount++;
   mByteCount += aMsgLength;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace




