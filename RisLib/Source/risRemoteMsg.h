#ifndef _RISREMOTEMSG_H_
#define _RISREMOTEMSG_H_
/*==============================================================================
Description:

This file contains a set of classes that encapsulate the message set
that is used for remote control.

There is a class for each particular message in the set and there is a
base class that all of the messages inherit from.

These messages follow the ByteContent pattern, where they all inherit
from ByteContent so that they can be copied to/from ByteBuffers.

The base class is used to specify set membership, any inheriting class
is a member of the message set, and message objects can be referenced
anonymously via pointers to the the base class.

The base class provides a member function, makeFromByteBuffer, that
extracts particular messages from a byte buffer and returns a pointer
to the base class.

These messages all have the same common form, they all contain a
common message header. The base class has a Header member object that
encapsulates the header.

These are the messages:

Test
FirstMessage
RunTest
Acknowledge

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risByteContent.h"
#include "risByteMessage.h"

namespace Ris
{
namespace RemoteMsg
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Message Types

namespace MsgIdT
{
   // Message family identifier
   enum{Family=301};

   //--------------------------------------------------------------------------
   // Specific values:

   enum Enum
   {
      Unspecified                     =    0,
      RunTest                         =   11,
      RunTestAck                      =   12,
      TestCompletion                  =   13,
      PrintStr                        =   14,
      CmdLineStr                      =   15,
   };

   //--------------------------------------------------------------------------
   // Support:

   inline char const * const asString(int x)
   {
      switch (x)
      {
         case Unspecified                  : return "Unspecified";                   break;
         case RunTest                      : return "RunTest";                       break;
         case RunTestAck                   : return "RunTestAck";                    break;
         case TestCompletion               : return "TestCompletion";                break;
         case PrintStr                     : return "PrintStr";                      break;
         case CmdLineStr                   : return "CmdLineStr";                    break;
         default                           : return "BadValue";                      break;
      }
   };
}//namespace

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This encapsualtes a message header.

class Header : public Ris::ByteContent
{
public:
   Header();

   //------------------------------------------------
   // Header Content

   Ris::Int32   mSyncWord1;
   Ris::Int32   mSyncWord2;
   Ris::Int32   mMessageType;
   Ris::Int32   mMessageLength;
   Ris::Int32   mFamily;
   Ris::Int32   mSourceId;
   Ris::Int32   mDestinationId;

   // Header Content
   //------------------------------------------------

   enum {Length = 28};

   //--------------------------------------------------------------------------
   // If the byte buffer is configured for put operations then this puts the
   // contents of the object into the byte buffer (it does a copy to, it
   // copies the object to the byte buffer).
   // If the byte buffer is configured for get operations then this gets the
   // contents of the object from the byte buffer (it does a copy from, it
   // copies the object from the byte buffer).
   // Copy To and Copy From are symmetrical.
   //--------------------------------------------------------------------------

   virtual void copyToFrom (Ris::ByteBuffer* aBuffer);

   //--------------------------------------------------------------------------
   // This returns true if the buffer length is large enough
   // to contain a header
   //--------------------------------------------------------------------------

   static bool validateBuffer(Ris::ByteBuffer* aBuffer);

   //--------------------------------------------------------------------------
   // This returns true if the header contains valid
   // values for some of its members
   //--------------------------------------------------------------------------

   bool validateContent();

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
   // where the copy is to take place. Both are also passed a MessageContent
   // pointer to where they can get MessageContent::mFamily and mMessageType
   // which they transfer into and out of the headers.
   //--------------------------------------------------------------------------

   virtual void   headerCopyToFrom    (Ris::ByteBuffer* aBuffer,Ris::MessageContent* parent);
   virtual void   headerReCopyToFrom  (Ris::ByteBuffer* aBuffer,Ris::MessageContent* parent);

   //---------------------------------------------------------------------------
   // This is set by headerCopyToFrom and used by headerReCopyToFrom,
   // for "put" operations. It contains the byte buffer position of
   // where the headerReCopyToFrom will take place, which should be
   // where headerCopyToFrom was told to do the copy.

   Ris::ByteBuffer  mOriginalBuffer;
};

//******************************************************************************
// This is a message parser that can be used by code that receives messages
// into byte buffers and then extracts them into message objects that live
// on the heap.

class MessageParser : public Ris::BaseMessageParser
{
public:
   //-------------------------------------------------------
   // Configure:

   void configure(int aSourceId);

   //-------------------------------------------------------
   // Base class overloads:

   // Return a contant header length
   int  getHeaderLength();

   // Extract message header parameters from a buffer and validate them
   void getMessageHeaderParms(Ris::ByteBuffer* aBuffer,Ris::MessageHeaderParms* aParms);

   // create a new message based on a message type
   Ris::ByteContent* createMessage(int aMessageType);

   // Preprocess a message before it is sent
   void processBeforeSend(Ris::ByteContent* msg);

   //-------------------------------------------------------
   // Data:

   int mSourceId;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Base message class.
// All particular messages for this message set inherit from this.

class BaseMsg : public Ris::MessageContent
{
public:
   //--------------------------------------------------------------------------
   // The constructor sets default MessageContent information
   //--------------------------------------------------------------------------

   BaseMsg();

   //---------------------------------------------------------------------------
   // Message Header 

   Header mHeader;

   //--------------------------------------------------------------------------
   // create a new message based on a message type

   static BaseMsg* createMessage(int aMessageType);

   //--------------------------------------------------------------------------
   // clone a new message from an existing message

   BaseMsg* cloneMessage();

   //--------------------------------------------------------------------------
   // support

   const char* getNameOf ();
};

//******************************************************************************
// use this for a buffer size for these messages

   enum {MsgBufferSize = 200000};

//******************************************************************************
//******************************************************************************
//******************************************************************************

class RunTestMsg : public BaseMsg
{
public:
   RunTestMsg();
   void copyToFrom (Ris::ByteBuffer* aBuffer);

   //------------------------------------------------
   // Content
   
   enum {StringSize=100};
   char       mTestName[StringSize];

   // Content
   //------------------------------------------------
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

class RunTestAckMsg : public BaseMsg
{
public:
   RunTestAckMsg();
   void copyToFrom (Ris::ByteBuffer* aBuffer);

   //------------------------------------------------
   // Content
   
   // Content
   //------------------------------------------------
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

class TestCompletionMsg : public BaseMsg
{
public:
   TestCompletionMsg();
   void copyToFrom (Ris::ByteBuffer* aBuffer);

   //------------------------------------------------
   // Content
   
   enum {StringSize=100};
   char       mTestName[StringSize];

   // Content
   //------------------------------------------------
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

class PrintStrMsg : public BaseMsg
{
public:
   PrintStrMsg();
   PrintStrMsg(char* aPrintStr);
   void copyToFrom (Ris::ByteBuffer* aBuffer);

   //------------------------------------------------
   // Content
   
   enum {StringSize=400};
   char       mPrintStr[StringSize];

   // Content
   //------------------------------------------------
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

class CmdLineStrMsg : public BaseMsg
{
public:
   CmdLineStrMsg();
   CmdLineStrMsg(char* aCmdLine);
   void copyToFrom (Ris::ByteBuffer* aBuffer);

   //------------------------------------------------
   // Content
   
   enum {StringSize=400};
   char       mCmdLineStr[StringSize];

   // Content
   //------------------------------------------------
};

}//namespace
}//namespace
#endif

