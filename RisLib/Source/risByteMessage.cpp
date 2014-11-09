/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risByteMessage.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

MessageHeaderParms::MessageHeaderParms()
{
   mHeaderLength=0;
   mMessageLength=0;
   mMessageType=0;
   mPayloadLength=0;
   mValidFlag=false;
};

BaseMessageParser::BaseMessageParser()
{
   mNetworkOrder=false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//------------------------------------------------------------------------------
// This extracts a particular message from a byte buffer.
// It does the following:
//
// 1) It validates the data in the byte buffer.
// 2) It examines message identifiers and determines the type
//    of message to be extracted. 
// 3) It creates a new message object of the type specifed by
//    the identifiers
// 4) It copies the data from the byte buffer into the new message
//    object and returns a pointer to the base class.
//------------------------------------------------------------------------------

ByteContent* BaseMessageParser::makeFromByteBuffer(ByteBuffer* aBuffer)
{
   ByteContent* message = 0;

   // set buffer direction for get
   aBuffer->setCopyFrom();

   // save the original buffer position
   char* originalPosition = aBuffer->getPosition();

   // call the inheritor's overload to extract the
   // message header from the buffer
   MessageHeaderParms tHeaderParms;
   getMessageHeaderParms(aBuffer,&tHeaderParms);
   
   // call the inheritor's overload to validate the message header
   if (!tHeaderParms.mValidFlag)
   {
      return 0;
   }

   // call the inheritor's overload to get the message type
   // from the message header
   int messageType = tHeaderParms.mMessageType;

   // call the inheritor's overload to create a new message
   // based on the message type
   message = createMessage(messageType);

   // guard
   if (!message)
   {
      return 0;
   }

   // copy data from the buffer into the new message object
   aBuffer->setPosition(originalPosition);
   aBuffer->get(message);
   return message;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
void BaseMessageParser::setNetworkOrder (bool aNetworkOrder)
{
   mNetworkOrder=aNetworkOrder;
}

void BaseMessageParser::configureByteBuffer(ByteBuffer* aBuffer)
{
   aBuffer->setNetworkOrder(mNetworkOrder);
}

//******************************************************************************
}//namespace




