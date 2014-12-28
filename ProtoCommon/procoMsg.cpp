/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdio.h>
#include <string.h>
#include "procoMsg.h"


namespace ProtoComm
{
//******************************************************************************
//******************************************************************************
//******************************************************************************

Header::Header()
{
   mSyncWord1      = 0xaaaaaaaa;
   mSyncWord2      = 0xbbbbbbbb;
   mMessageType    = 0;
   mMessageLength  = 0;
   mFamily         = 0;
   mSourceId       = 0;
   mDestinationId  = 0;

   mSyncWord3      = 0xcccccccc;
   mSyncWord4      = 0xdddddddd;
}

void Header::copyToFrom(Ris::ByteBuffer* aBuffer)
{
   aBuffer->copy(& mSyncWord1     );
   aBuffer->copy(& mSyncWord2     );
   aBuffer->copy(& mMessageType   );
   aBuffer->copy(& mMessageLength );
   aBuffer->copy(& mFamily        );
   aBuffer->copy(& mSourceId      );
   aBuffer->copy(& mDestinationId );
}

bool Header::validateBuffer(Ris::ByteBuffer* aBuffer)
{
   bool error =
      aBuffer->getError() ||
      aBuffer->getLength() < Length ||
      aBuffer->getLength() > MsgBufferSize;

   return !error;
}

bool Header::validateContent()
{
   bool error =
      mSyncWord1 != 0xaaaaaaaa ||
      mSyncWord2 != 0xbbbbbbbb ||
      mMessageLength < Length  ||
      mMessageLength > MsgBufferSize;

   return !error;
}

//------------------------------------------------------------------------------
// These are called explicitly by inheriting messages at the
// beginning and end of their copyToFrom's to manage the headers.
// For "get" operations, headerCopyToFrom "gets" the header and
// headerReCopyToFrom does nothing. For "put" operations,
// headerCopyToFrom does nothing except store the buffer pointer and
// headerReCopyToFrom "puts" the header at the stored position. Both
// functions are passed a byte buffer pointer to where the copy is
// to take place. Both are also passed a MessageContent pointer to
// where they can get MessageContent::mFamily and mMessageType which
// they transfer into and out of the headers.
//------------------------------------------------------------------------------

void Header::headerCopyToFrom (Ris::ByteBuffer* aBuffer,Ris::MessageContent* parent)
{

   //---------------------------------------------------------------------
   // Instances of this class are members of parent message classes.
   // A call to this function should be the first line of code in a
   // containing parent message class's copyToFrom. It performs pre-copyToFrom
   // operations. It's purpose is to copy headers to/from byte buffers. The
   // corresponding function headerReCopyToFrom should be called as the last
   // line of code in the containing message class' copyToFrom. Lines of code
   // in between should copy individual data elements into/out of the buffer.

   //---------------------------------------------------------------------
   // for a "copy to" put
   //
   // If this is a "copy to" put operation then the header copy will actually
   // be done by the headerReCopyToFrom, after the rest of the message has been
   // copied into the buffer. This is because some of the fields in the header
   // cannot be set until after the rest of the message has been put into the
   // buffer. (You don't know the length of the message until you put all of
   // the data into it, you also can't compute a checksum). This call stores
   // the original buffer position that is passed to it when it is called for
   // later use by the headerReCopyToFrom. The original buffer position points
   // to where the header should be copied. This call then forward advances
   // the buffer to point past the header. Forward advancing the buffer
   // position to point just after where the header should be is the same as
   // doing a pretend copy of the header. After this pretend copy of the
   // header, the buffer position points to where the data should be put into
   // the buffer.
   //
   // Store the original buffer position for later use by the
   // headerReCopyToFrom and advance the buffer position forward
   // to point past the header.

   if (aBuffer->isCopyTo())
   {
      // Store the buffer object for later use.
      mOriginalBuffer = *aBuffer;
      // Advance the buffer position to point past the header.
      aBuffer->forward(Header::Length);
   }

   //---------------------------------------------------------------------
   // for a "copy from" get
   //
   // If this is a "copy from" get operation then copy the header from the
   // buffer into the header member. Also set the message content type from
   // the variable datum id

   else
   {
      // Copy the buffer content into the header object.
      copyToFrom(aBuffer);
      // Set the message content type.
      parent->mMessageType = mMessageType;
      parent->mFamily      = mFamily;
   }
}

//------------------------------------------------------------------------------
// These are called explicitly by inheriting messages at the
// beginning and end of their copyToFrom's to manage the headers.
// For "get" operations, headerCopyToFrom "gets" the header and
// headerReCopyToFrom does nothing. For "put" operations,
// headerCopyToFrom does nothing except store the buffer pointer and
// headerReCopyToFrom "puts" the header at the stored position. Both
// functions are passed a byte buffer pointer to where the copy is
// to take place. Both are also passed a MessageContent pointer to
// where they can get MessageContent::mFamily and mMessageType which
// they transfer into and out of the headers.
//------------------------------------------------------------------------------

void Header::headerReCopyToFrom (Ris::ByteBuffer* aBuffer,Ris::MessageContent* parent)
{
   // If this is a put operation then this actually copies the header into
   // the buffer.
   // This sets some header length parameters and copies the header into the
   // buffer position that was stored when headerCopyToFrom was called.

   if (aBuffer->isCopyTo())
   {
      aBuffer->copy(& mSyncWord3 );
      aBuffer->copy(& mSyncWord4 );

      mMessageType   = parent->mMessageType;
      mFamily        = parent->mFamily;
      mMessageLength = aBuffer->getLength();

      copyToFrom(&mOriginalBuffer);
   }
   else
   {
      int   tSyncWord3;
      int   tSyncWord4;

      aBuffer->copy(& tSyncWord3 );
      aBuffer->copy(& tSyncWord4 );
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void  MessageParser::configure(int aSourceId)
{
   mSourceId=aSourceId;
}

//******************************************************************************

int  MessageParser::getHeaderLength()
{
   return Header::Length;
}

//******************************************************************************

void MessageParser::getMessageHeaderParms(Ris::ByteBuffer* aBuffer,Ris::MessageHeaderParms* aParms)
{
   Header tHeader;   
   aBuffer->get(&tHeader);

   aParms->mHeaderLength   = Header::Length;;
   aParms->mMessageLength  = tHeader.mMessageLength;
   aParms->mMessageType    = tHeader.mMessageType;
   aParms->mPayloadLength  = tHeader.mMessageLength - Header::Length;
   aParms->mValidFlag      = tHeader.validateContent();
}

//******************************************************************************

Ris::ByteContent* MessageParser::createMessage(int aMessageType)
{
   return BaseMsg::createMessage(aMessageType);
}

//******************************************************************************

void MessageParser::processBeforeSend(Ris::ByteContent* aMsg)
{
   BaseMsg* tx = (BaseMsg*)aMsg;

   if (tx->mHeader.mSourceId==0)
   {
      tx->mHeader.mSourceId=mSourceId;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// MessageParser creator

MessageParserCreator::MessageParserCreator()
{
   mSourceId = 0;
}

void MessageParserCreator::configure(int aSourceId)
{
   mSourceId = aSourceId;
}

Ris::BaseMessageParser* MessageParserCreator::createNew()
{
   // New message parser
   MessageParser* tMessageParser = new MessageParser();
   // Configure 
   tMessageParser->configure(mSourceId);
   // Return base message parser pointer
   return (Ris::BaseMessageParser*)tMessageParser;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

BaseMsg::BaseMsg ()
{
   mFamily=MsgIdT::Family;
}

//******************************************************************************

BaseMsg* BaseMsg::cloneMessage()
{
   // Create a new message based on message type
   BaseMsg* msg = createMessage(mMessageType);

   // Byte buffer
   Ris::ByteBuffer buffer(MsgBufferSize);  

   // Copy this message to buffer
   buffer.putToBuffer(this);

   // Copy buffer to new message
   buffer.rewind();
   buffer.getFromBuffer(msg);

   return msg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

const char* BaseMsg::getNameOf ()
{
   return MsgIdT::asString(mMessageType);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This creates a new message, based on a message type

BaseMsg* BaseMsg::createMessage(int aMessageType)
{
   BaseMsg* message = 0;

   switch (aMessageType)
   {
      case MsgIdT::Test :
         message = new TestMsg;
         break;
      case MsgIdT::FirstMessage :
         message = new FirstMessageMsg;
         break;
      case MsgIdT::StatusRequest :
         message = new StatusRequestMsg;
         break;
      case MsgIdT::StatusResponse :
         message = new StatusResponseMsg;
         break;
      default :
         return 0;
         break;
   }
   return message;
}
//******************************************************************************
//******************************************************************************
//******************************************************************************

TestMsg::TestMsg ()
{
   mMessageType = MsgIdT::Test;
   mCode1 = 0;
   mCode2 = 0;
} 

void TestMsg::copyToFrom (Ris::ByteBuffer* aBuffer)
{
   mHeader.headerCopyToFrom(aBuffer,this);

   aBuffer->copy( & mCode1 );
   aBuffer->copyFString(mData,DataSize);
   aBuffer->copy( & mCode2 );

   mHeader.headerReCopyToFrom(aBuffer,this);
}

//******************************************************************************

FirstMessageMsg::FirstMessageMsg ()
{
   mMessageType = MsgIdT::FirstMessage;
   mCode1 = 0;
} 

void FirstMessageMsg::copyToFrom (Ris::ByteBuffer* aBuffer)
{
   mHeader.headerCopyToFrom(aBuffer,this);

   aBuffer->copy( & mCode1 );

   mHeader.headerReCopyToFrom(aBuffer,this);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

StatusRequestMsg::StatusRequestMsg ()
{
   mMessageType = MsgIdT::StatusRequest;

   mCode1 = 0;
   mCode2 = 0;
   mCode3 = 0;
   mCode4 = 0;

   mNumOfWords=0;
   mNumOfWords=MaxWords;
} 

void StatusRequestMsg::copyToFrom (Ris::ByteBuffer* aBuffer)
{
   mHeader.headerCopyToFrom(aBuffer,this);

   aBuffer->copy        (& mCode1  );
   aBuffer->copy        (& mCode2  );
   aBuffer->copy        (& mCode3  );
   aBuffer->copy        (& mCode4  );

   aBuffer->copy        (& mNumOfWords  );
   for (int i=0;i<mNumOfWords;i++)
   {
      aBuffer->copy     (& mWords[i] );
   }

   aBuffer->copyData    (& mTimeMarker,sizeof(mTimeMarker) );

   mHeader.headerReCopyToFrom(aBuffer,this);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

StatusResponseMsg::StatusResponseMsg ()
{
   mMessageType = MsgIdT::StatusResponse;

   mCode1 = 0;
   mCode2 = 0;
   mCode3 = 0;
   mCode4 = 0;

   mNumOfWords=0;
   mNumOfWords=MaxWords;
} 

void StatusResponseMsg::copyToFrom (Ris::ByteBuffer* aBuffer)
{
   mHeader.headerCopyToFrom(aBuffer,this);

   aBuffer->copy        (& mCode1  );
   aBuffer->copy        (& mCode2  );
   aBuffer->copy        (& mCode3  );
   aBuffer->copy        (& mCode4  );

   aBuffer->copy        (& mNumOfWords  );
   for (int i=0;i<mNumOfWords;i++)
   {
      aBuffer->copy     (& mWords[i] );
   }

   aBuffer->copyData    (& mTimeMarker,sizeof(mTimeMarker) );

   mHeader.headerReCopyToFrom(aBuffer,this);
}

}//namespace


