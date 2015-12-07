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
   mSyncWord1         = 0x11111111;
   mSyncWord2         = 0x22222222;
   mMessageIdentifier = 0;
   mMessageLength     = 0;
   mSourceId          = 0;
   mDestinationId     = 0;

   mInitialPosition   = 0;
   mInitialLength     = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//--------------------------------------------------------------------------
// If the byte buffer is configured for put operations then this puts the
// contents of the object into the byte buffer (it does a copy to, it
// copies the object to the byte buffer).
// If the byte buffer is configured for get operations then this gets the
// contents of the object from the byte buffer (it does a copy from, it
// copies the object from the byte buffer).
// Copy To and Copy From are symmetrical.
//--------------------------------------------------------------------------

void Header::copyToFrom (Ris::ByteBuffer* aBuffer)
{
   aBuffer->copy( &mSyncWord1         );
   aBuffer->copy( &mSyncWord2         );
   aBuffer->copy( &mMessageIdentifier );
   aBuffer->copy( &mMessageLength     );
   aBuffer->copy( &mSourceId          );
   aBuffer->copy( &mDestinationId     );
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//--------------------------------------------------------------------------
// For variable content messages, the message length cannot be known until
// the entire message has been written to a byte buffer. Therefore, the 
// message header cannot be written to a byte buffer until the entire
// message has been written and the length is known.
//
// The procedure to write a message to a byte buffer is to skip over the 
// buffer segment where the header is located, write the message payload
// to the buffer, set the header message length based on the now known
// payload length, and write the header to the buffer.
//
// These are called explicitly by inheriting messages at the
// beginning and end of their copyToFrom's to manage the headers.
// For "get" operations, headerCopyToFrom "gets" the header and
// headerReCopyToFrom does nothing. For "put" operations,
// headerCopyToFrom stores the buffer pointer and advances past where the
// header will be written and headerReCopyToFrom "puts" the header at the
// stored position. Both functions are passed a byte buffer pointer to
// where the copy is to take place. Both are also passed a MessageByte
// pointer to where they can get and mMessageType
// which they transfer into and out of the headers.
//--------------------------------------------------------------------------

void Header::headerCopyToFrom (Ris::ByteBuffer* aBuffer,Ris::ByteContent* aParent)
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
      // Store the buffer parameters for later use by the
      // headerReCopyToFrom
      mInitialPosition = aBuffer->getPosition();
      mInitialLength   = aBuffer->getLength();

      // Advance the buffer position to point past the header.
      aBuffer->forward(cLength);
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
      aParent->mMessageType = mMessageIdentifier;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Header::headerReCopyToFrom  (Ris::ByteBuffer* aBuffer,Ris::ByteContent* aParent)
{
   // If this is a put operation then this actually copies the header into
   // the buffer.
   // This sets some header length parameters and copies the header into the
   // buffer position that was stored when headerCopyToFrom was called.

   if (aBuffer->isCopyTo())
   {
      // Store the buffer parameters for later use by the
      // headerReCopyToFrom
      int tFinalPosition = aBuffer->getPosition();
      int tFinalLength   = aBuffer->getLength();

      // Get message parameters from parent
      mMessageIdentifier = aParent->mMessageType;
      mMessageLength     = aBuffer->getLength();

      // Restore buffer parameters
      // to the initial position
      aBuffer->setPosition (mInitialPosition);
      aBuffer->setLength   (mInitialPosition);

      // Copy the adjusted header into the buffer'
      // at the original position
      copyToFrom( aBuffer );

      // Restore buffer parameters
      // to the final position
      aBuffer->setPosition (tFinalPosition);
      aBuffer->setLength   (tFinalPosition);
   }
   else
   {
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
   return Header::cLength;
}

//******************************************************************************

bool MessageParser::extractMessageHeaderParms(Ris::ByteBuffer* aBuffer)
{
   // Extract header from buffer
   Header tHeader;   
   aBuffer->getFromBuffer(&tHeader);

   // Set header parameters
   mHeaderLength    = Header::cLength;
   mMessageLength   = tHeader.mMessageLength;
   mMessageType     = tHeader.mMessageIdentifier;
   mPayloadLength   = tHeader.mMessageLength - Header::cLength;

   // Test for error
   bool tError =
      tHeader.mSyncWord1 != 0x11111111 ||
      tHeader.mSyncWord2 != 0x22222222 ||
      tHeader.mMessageLength < Header::cLength  ||
      tHeader.mMessageLength > MsgBufferSize;

   // If no error then valid
   mHeaderValidFlag = !tError;

   // Return valid flag
   return mHeaderValidFlag;
}

//******************************************************************************

Ris::ByteContent* MessageParser::createMessage(int aMessageType)
{
   return MessageCreator::createMessage(aMessageType);
}

//******************************************************************************

BaseMsg* MessageParser::cloneMessage (BaseMsg* aMsg)
{
   // Create a new message based on message type
   BaseMsg* tMsg = MessageCreator::createMessage(aMsg->mMessageType);

   // Byte buffer
   Ris::ByteBuffer tBuffer(MsgBufferSize);  

   // Copy message to clone to buffer
   tBuffer.putToBuffer(aMsg);

   // Copy buffer to new message
   tBuffer.getFromBuffer(tMsg);

   return tMsg;
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

BaseMsg* MessageCreator::createMessage(int aMessageType)
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
      case MsgIdT::StatusResponse:
         message = new StatusResponseMsg;
         break;
      case MsgIdT::Data:
         message = new DataMsg;
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
   mCode1 = 901;
   mCode2 = 902;
   mCode3 = 903;
   mCode4 = 904;
} 

void TestMsg::copyToFrom (Ris::ByteBuffer* aBuffer)
{
   mHeader.headerCopyToFrom(aBuffer,this);

   aBuffer->copy( &mCode1 );
   aBuffer->copy( &mCode2 );
   aBuffer->copy( &mCode3 );
   aBuffer->copy( &mCode4 );

   mHeader.headerReCopyToFrom(aBuffer,this);
}

void TestMsg::initialize()
{
   mCode1 = 901;
   mCode2 = 902;
   mCode3 = 903;
   mCode4 = 904;
}

void TestMsg::show()
{
   printf("%d\n",  mCode1   );
   printf("%d\n",  mCode1   );
   printf("%d\n",  mCode2   );
   printf("%d\n",  mCode3   );
   printf("\n");
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

   mCode1 = 101;
   mCode2 = 102;
   mCode3 = 103;
   mCode4 = 104;

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

   mHeader.headerReCopyToFrom(aBuffer,this);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

StatusResponseMsg::StatusResponseMsg()
{
   mMessageType = MsgIdT::StatusResponse;

   mCode1 = 201;
   mCode2 = 202;
   mCode3 = 203;
   mCode4 = 204;

   mNumOfWords = 0;
   mNumOfWords = MaxWords;
}

void StatusResponseMsg::copyToFrom(Ris::ByteBuffer* aBuffer)
{
   mHeader.headerCopyToFrom(aBuffer, this);

   aBuffer->copy(&mCode1);
   aBuffer->copy(&mCode2);
   aBuffer->copy(&mCode3);
   aBuffer->copy(&mCode4);

   aBuffer->copy(&mNumOfWords);
   for (int i = 0; i<mNumOfWords; i++)
   {
      aBuffer->copy(&mWords[i]);
   }

   mHeader.headerReCopyToFrom(aBuffer, this);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

DataRecord::DataRecord ()
{
   mX1 = 0;
   mX2 = 0;
   mX3 = 0;
   mX4 = 0;
} 

void DataRecord::copyToFrom (Ris::ByteBuffer* aBuffer)
{
   aBuffer->copy( &mX1 );
   aBuffer->copy( &mX2 );
   aBuffer->copy( &mX3 );
   aBuffer->copy( &mX4 );
}

void DataRecord::initialize()
{
   mX1 = 701;
   mX2 = 702;
   mX3 = 703;
   mX4 = 704;
}

void DataRecord::show()
{
   printf("%d\n",  mX1   );
   printf("%d\n",  mX2   );
   printf("%d\n",  mX3   );
   printf("%d\n",  mX4   );
   printf("\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

DataMsg::DataMsg()
{
   mMessageType = MsgIdT::Data;

   mUChar  = 0;
   mUShort = 0;
   mUInt   = 0;
   mUInt64 = 0;
   mChar   = 0;
   mShort  = 0;
   mInt    = 0;
   mInt64  = 0;
   mFloat  = 0.0f;
   mDouble = 0.0;
   mBool   = false;

   mZString[0]=0;
}

void DataMsg::copyToFrom(Ris::ByteBuffer* aBuffer)
{
   mHeader.headerCopyToFrom(aBuffer, this);

   aBuffer->copy( &mUChar  );
   aBuffer->copy( &mUShort );
   aBuffer->copy( &mUInt   );
   aBuffer->copy( &mUInt64 );
   aBuffer->copy( &mChar   );
   aBuffer->copy( &mShort  );
   aBuffer->copy( &mInt    );
   aBuffer->copy( &mInt64  );
   aBuffer->copy( &mFloat  );
   aBuffer->copy( &mDouble );
   aBuffer->copy( &mBool   );
   aBuffer->copyZString( mZString, MyStringSize  );
   aBuffer->copy( &mDataRecord);

   mHeader.headerReCopyToFrom(aBuffer, this);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void DataMsg::initialize()
{
   mUChar  = 0x11;
   mUShort = 0x1234;
   mUInt   = 0x12345678;
   mUInt64 = 0x1112131415161718;
   mChar   = 0x11;
   mShort  = 0x1234;
   mInt    = 0x12345678;
   mInt64  = 0x1112131415161718;
   mFloat  = 12.34f;
   mDouble = 56.78;
   mBool   = true;
   strcpy(mZString, "abcdef");

   mDataRecord.initialize();
}

void DataMsg::show()
{
   printf("UChar    %hhX\n",  mUChar   );
   printf("UShort   %hX\n",   mUShort  );
   printf("UInt     %X\n",    mUInt    );
   printf("Unit64   %llX\n",  mUInt64  );
   printf("Char     %hhX\n",  mChar    );
   printf("Short    %hX\n",   mShort   );
   printf("Int      %X\n",    mInt     );
   printf("Int64    %llX\n",  mInt64   );
   printf("Float    %5.2f\n", mFloat   );
   printf("Double   %5.2f\n", mDouble  );
   printf("Bool     %d\n",    mBool    );
   printf("ZString  %s\n",    mZString );
   mDataRecord.show();
   printf("\n");
}

}//namespace


