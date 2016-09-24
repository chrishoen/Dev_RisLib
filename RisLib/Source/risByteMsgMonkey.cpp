/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risByteMsgMonkey.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor

   BaseMsgMonkey::BaseMsgMonkey(BaseMsgCreator* aCreator,BaseMsgCopier* aCopier)
{
   mCreator = aCreator;
   mCopier  = aCopier;

   mHeaderLength=0;
   mMessageLength=0;
   mMessageType=0;
   mPayloadLength=0;
   mHeaderValidFlag=false;
   mNetworkOrder=false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Buffer management

void BaseMsgMonkey::setNetworkOrder (bool aNetworkOrder)
{
   mNetworkOrder=aNetworkOrder;
}

void BaseMsgMonkey::configureByteBuffer(ByteBuffer* aBuffer)
{
   aBuffer->setNetworkOrder(mNetworkOrder);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Copy a message to a byte buffer.

void BaseMsgMonkey::copyMsgToBuffer(Ris::ByteBuffer* aBuffer, Ris::ByteMsg* aMsg)
{
   // Call inheritor's override to preprocess the message before it is sent.
   processBeforeSend(aMsg);

   // Set buffer direction for put.
   aBuffer->setCopyTo();

   // Call inheritor's copier to copy from the message to the buffer.
   mCopier->copyToFrom(aBuffer,aMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Copy a message from a byte buffer
//
// 1) Create a new message object of the type specifed by the identifiers 
//    that were extracted from the header
// 2) Copy the data from the byte buffer into the new message object
// and returns a pointer to the base class.

void BaseMsgMonkey::copyMsgFromBuffer (Ris::ByteBuffer* aBuffer,Ris::ByteMsg*& aMsg)
{
   // Guard
   aMsg = 0;
   if (!mHeaderValidFlag) return;

   // Call inheritor's creator to create a new message based on the
   // message type that was extracted from the header.
   aMsg = mCreator->createMsg(mMessageType);

   // Guard
   if (!aMsg) return;

   // Set buffer direction for get.
   aBuffer->setCopyFrom();

   // Call inheritor's copier to copy from the buffer to the message.
   mCopier->copyToFrom(aBuffer,aMsg);
}

//******************************************************************************
}//namespace




