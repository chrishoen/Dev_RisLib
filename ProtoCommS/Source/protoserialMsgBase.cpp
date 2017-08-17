/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "protoserialMsg.h"
#include "protoserialMsgBase.h"


namespace ProtoSerial
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
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

void Header::reset()
{
   mSyncWord1         = 0;
   mSyncWord2         = 0;
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

void Header::headerCopyToFrom (Ris::ByteBuffer* aBuffer,BaseMsg* aParent)
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

void Header::headerReCopyToFrom  (Ris::ByteBuffer* aBuffer,BaseMsg* aParent)
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
//******************************************************************************
//******************************************************************************
//******************************************************************************

Footer::Footer()
{
   mCheckSum           = 0x00000000;
   mInitialPosition   = 0;
}

void Footer::reset()
{
   mCheckSum           = 0x00000000;
   mInitialPosition   = 0;
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

void Footer::copyToFrom (Ris::ByteBuffer* aBuffer)
{
   aBuffer->copy( &mCheckSum           );
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//--------------------------------------------------------------------------
// For variable content messages, the message length cannot be known until
// the entire message has been written to a byte buffer. Therefore, the 
// message header footer be written to a byte buffer until the entire
// message has been written and the length is known.
//
// The procedure to write a message to a byte buffer is to write the entire 
// message header and payload to a buffer and then write the footer with 
// zeroed data. The header recopy is then called to obtain the correct
// message length. The last step is to call the footer recopy to fill
// in the footer checksum with the correct value.
// 
// These are called explicitly by inheriting messages at the
// end of their copyToFrom's to manage the footers.
// For "get" operations, footerCopyToFrom "gets" the footer and
// footerReCopyToFrom does nothing. For "put" operations,
// footerCopyToFrom stores the buffer pointer and advances past where the
// footer will be written and footerReCopyToFrom "puts" the footer at the
// stored position. Both functions are passed a byte buffer pointer to
// where the copy is to take place.
//--------------------------------------------------------------------------

void Footer::footerCopyToFrom (Ris::ByteBuffer* aBuffer)
{
   //---------------------------------------------------------------------
   // for a "copy to" put

   if (aBuffer->isCopyTo())
   {
      // Store the buffer parameters for later use by the
      // footerReCopyToFrom
      mInitialPosition = aBuffer->getPosition();
   }

   //---------------------------------------------------------------------
   // for a "copy from" get
   //
   // If this is a "copy from" get operation then copy the footer from the
   // buffer into the footer member. Also set the message content type from
   // the variable datum id

   // Copy the buffer content into the footer object.
   copyToFrom(aBuffer);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Footer::footerReCopyToFrom  (Ris::ByteBuffer* aBuffer)
{
   // If this is a put operation then this actually copies the footer into
   // the buffer.
   // This sets some footer length parameters and copies the footer into the
   // buffer position that was stored when footerCopyToFrom was called.

   if (aBuffer->isCopyTo())
   {
      // Calculate the checksum of all of the bytes in the buffer before the 
      // footer.
      mCheckSum = 0;
      for (int i = 0; i < mInitialPosition; i++)
      {
         mCheckSum += aBuffer->mBaseBytes[i];
      }

      // Set the buffer position to the start of the footer.
      aBuffer->setPosition (mInitialPosition);
      aBuffer->setLength   (mInitialPosition);

      // Copy the checksum to the buffer.
      aBuffer->copy( &mCheckSum );
   }
   else
   {
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

MsgMonkey::MsgMonkey()
   : Ris::BaseMsgMonkey(new MsgCreator)
{
   mSourceId=0;
}

void  MsgMonkey::configure(int aSourceId)
{
   mSourceId=aSourceId;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool MsgMonkey::extractMessageHeaderParms(Ris::ByteBuffer* aBuffer)
{
   // Guard.
   mHeaderValidFlag = false;
   if (aBuffer->mWorkingLength < Header::cLength) return false;

   // Extract header from buffer
   Header tHeader;
   tHeader.reset();
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
      tHeader.mMessageLength > MsgDefT::cMsgBufferSize;

   // If no error then valid
   mHeaderValidFlag = !tError;

   // Return valid flag
   return mHeaderValidFlag;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void MsgMonkey::processBeforeSend(Ris::ByteContent* aMsg)
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
//******************************************************************************
//******************************************************************************
//******************************************************************************
// MsgMonkey creator

MsgMonkeyCreator::MsgMonkeyCreator()
{
   mSourceId = 0;
}

void MsgMonkeyCreator::configure(int aSourceId)
{
   mSourceId = aSourceId;
}

Ris::BaseMsgMonkey* MsgMonkeyCreator::createMonkey()
{
   // New message monkey
   MsgMonkey* tMsgMonkey = new MsgMonkey();
   // Configure 
   tMsgMonkey->configure(mSourceId);
   // Return base message monkey pointer
   return (Ris::BaseMsgMonkey*)tMsgMonkey;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace


