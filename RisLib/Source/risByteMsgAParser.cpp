/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risByteMsgAParser.h"

namespace Ris
{

BaseMsgAParser::BaseMsgAParser()
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

ByteContent* BaseMsgAParser::makeFromByteBuffer(ByteBuffer* aBuffer)
{
   // Guard
   if (!mHeaderValidFlag)
   {
      return 0;
   }

   // Set buffer direction for get
   aBuffer->setCopyFrom();

   // Call the inheritor's overload to create a new message
   // based on the message type
   ByteContent* tMsg = createMessage(mMessageType);

   // Guard
   if (!tMsg)
   {
      return 0;
   }

   // copy data from the buffer into the new message object
   aBuffer->getFromBuffer(tMsg);

   return tMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
void BaseMsgAParser::setNetworkOrder (bool aNetworkOrder)
{
   mNetworkOrder=aNetworkOrder;
}

void BaseMsgAParser::configureByteBuffer(ByteBuffer* aBuffer)
{
   aBuffer->setNetworkOrder(mNetworkOrder);
}

//******************************************************************************
}//namespace




