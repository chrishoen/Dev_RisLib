#ifndef _PROCOMSG_MESSAGE_H_
#define _PROCOMSG_MESSAGE_H_
/*==============================================================================
This file contains a set of classes that encapsulate the message set
that is used to communicate with Intranet. The messages are specified
in the IDD.

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
StatusRequest
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risByteContent.h"
#include "risByteMsgMonkey.h"
#include "procoMsg_Base.h"

namespace ProtoComm
{
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Message Types

class MsgIdT
{
public:

   static const int   cUnspecifiedMsg    = 0;
   static const int   cTestMsg           = 1;
   static const int   cFirstMessageMsg   = 2;
   static const int   cStatusRequestMsg  = 3;
   static const int   cStatusResponseMsg = 4;
   static const int   cDataMsg           = 5;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Particular message classes.
// There is one class for each message in the message set.

class TestMsg : public BaseMsg
{
public:
   //------------------------------------------------
   // Content

   int mCode1;
   int mCode2;
   int mCode3;
   int mCode4;

   // Content
   //------------------------------------------------

   TestMsg();
   void copyToFrom (Ris::ByteBuffer* aBuffer);

   void initialize();
   void show();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

class FirstMessageMsg : public BaseMsg
{
public:
   //------------------------------------------------
   // Content

   int mCode1;

   // Content
   //------------------------------------------------

   FirstMessageMsg();
   void copyToFrom (Ris::ByteBuffer* aBuffer);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

class StatusRequestMsg : public BaseMsg
{
public:
   //------------------------------------------------
   // Content

   int    mCode1;
   int    mCode2;
   int    mCode3;
   int    mCode4;

   enum {MaxWords=1000};
   int  mNumOfWords;
   int  mWords[MaxWords];

   // Content
   //------------------------------------------------

   StatusRequestMsg();
   void copyToFrom (Ris::ByteBuffer* aBuffer);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

class StatusResponseMsg : public BaseMsg
{
public:
   //------------------------------------------------
   // Content

   int   mCode1;
   int   mCode2;
   int   mCode3;
   int   mCode4;

   enum { MaxWords = 1000 };
   int   mNumOfWords;
   int   mWords[MaxWords];

   // Content
   //------------------------------------------------

   StatusResponseMsg();
   void copyToFrom(Ris::ByteBuffer* aBuffer);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

class DataRecord : public Ris::ByteContent
{
public:

   //------------------------------------------------
   // Content

   int mX1;
   int mX2;
   int mX3;
   int mX4;

   // Content
   //------------------------------------------------

   DataRecord();
   void copyToFrom (Ris::ByteBuffer* aBuffer);

   void initialize();
   void show();
};

class DataMsg : public BaseMsg
{
public:

   //------------------------------------------------
   // Content

   unsigned char       mUChar;
   unsigned short      mUShort;
   unsigned int        mUInt;
   unsigned long long  mUInt64;
   char                mChar;
   short               mShort;
   int                 mInt;
   long long           mInt64;
   float               mFloat;
   double              mDouble;
   bool                mBool;

   char                mString1[100];
   char                mString2[100];

   DataRecord          mDataRecord;

   // Content
   //------------------------------------------------

   DataMsg();
   void copyToFrom(Ris::ByteBuffer* aBuffer);

   void initialize();
   void show();

};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Message creator:

class MsgCreator : public Ris::BaseMsgCreator
{
public:
   //***********************************************************************
   // Create a new message, based on a message type.

   Ris::ByteContent* createMsg (int aMessageType) override;
};

}//namespace
#endif

