#ifndef _RISBYTEMSGDEFAULT_H_
#define _RISBYTEMSGDEFAULT_H_

/*==============================================================================

UdpRxMsgSocket -- udp receive socket
UdpTxMsgSocket -- udp transmit socket

==============================================================================*/

//*********************************************************************************
//*********************************************************************************
//*********************************************************************************

#include "risByteMsg.h"
#include "risByteContent.h"
#include "risSockets.h"
#include "risThreadsThreads.h"

namespace Ris
{
//******************************************************************************
//******************************************************************************
//******************************************************************************
// This encapsualtes the message header.

class DefaultMsgHeader : public Ris::ByteContent
{
public:
   // Constructor
   DefaultMsgHeader();
   void reset();

   //------------------------------------------------------------------------
   // Header Content

   int   mSyncWord1;
   int   mSyncWord2;
   int   mMessageIdentifier;
   int   mMessageLength;
   int   mSourceId;
   int   mDestinationId;

   // Header Content
   //------------------------------------------------------------------------

   // Header length
   static const int cLength = 24;

   // Valid
   bool mHeaderValidFlag;

   //------------------------------------------------------------------------
   // Validate a received header

   bool validate();

   //------------------------------------------------------------------------
   // If the byte buffer is configured for put operations then this puts the
   // contents of the object into the byte buffer (it does a copy to, it
   // copies the object to the byte buffer).
   // If the byte buffer is configured for get operations then this gets the
   // contents of the object from the byte buffer (it does a copy from, it
   // copies the object from the byte buffer).
   // Copy To and Copy From are symmetrical.
   //------------------------------------------------------------------------

   void copyToFrom(Ris::ByteBuffer* aBuffer);

   //------------------------------------------------------------------------
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
   // where the copy is to take place. Both are also passed a Ris::ByteContent
   // pointer to where they can get and mMessageType
   // which they transfer into and out of the headers.
   //------------------------------------------------------------------------

   void headerCopyToFrom   (Ris::ByteBuffer* aBuffer, Ris::ByteMsg* aParent);
   void headerReCopyToFrom (Ris::ByteBuffer* aBuffer, Ris::ByteMsg* aParent);

   //------------------------------------------------------------------------
   // These are set by headerCopyToFrom and used by headerReCopyToFrom,
   // for "put" operations.Theyt contain the buffer position and length of
   // where the headerReCopyToFrom will take place, which should be
   // where headerCopyToFrom was told to do the copy.

   int mInitialPosition;
   int mInitialLength;
};

}//namespace
#endif

