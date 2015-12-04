/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risByteMessage.h"

namespace Ris
{

BaseMessageParser::BaseMessageParser()
{
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
   // Guard
   if (!mHeaderValidFlag)
   {
      return 0;
   }

   ByteContent* message = 0;

   // Set buffer direction for get
   aBuffer->setCopyFrom();

   // Save the original buffer position
   char* originalPosition = aBuffer->getPosition();
   
   // call the inheritor's overload to create a new message
   // based on the message type
   message = createMessage(mMessageType);

   // guard
   if (!message)
   {
      return 0;
   }

   // copy data from the buffer into the new message object
   aBuffer->setPosition(originalPosition);
   aBuffer->getFromBuffer(message);
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




