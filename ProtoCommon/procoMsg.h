#ifndef _PROCOMSG_H_
#define _PROCOMSG_H_
/*==============================================================================
This file contains a set of classes that encapsulate a message set.

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
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risByteContent.h"
#include "risByteMsgMonkey.h"
#include "procoMsgBase.h"

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

   //***************************************************************************
   // Members:

   int mCode1;
   int mCode2;
   int mCode3;
   int mCode4;

   //***************************************************************************
   // Methods:

   TestMsg();
   void copyToFrom (Ris::ByteBuffer* aBuffer);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

class FirstMessageMsg : public BaseMsg
{
public:

   //***************************************************************************
   // Members:

   int mCode1;

   //***************************************************************************
   // Methods:

   FirstMessageMsg();
   void copyToFrom (Ris::ByteBuffer* aBuffer);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

class StatusRequestMsg : public BaseMsg
{
public:

   //***************************************************************************
   // Members:

   int    mCode1;
   int    mCode2;
   int    mCode3;
   int    mCode4;

   enum {MaxWords=1000};
   int  mNumOfWords;
   int  mWords[MaxWords];

   //***************************************************************************
   // Methods:

   StatusRequestMsg();
   void copyToFrom (Ris::ByteBuffer* aBuffer);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

class StatusResponseMsg : public BaseMsg
{
public:

   //***************************************************************************
   // Members:

   int   mCode1;
   int   mCode2;
   int   mCode3;
   int   mCode4;

   enum { MaxWords = 1000 };
   int   mNumOfWords;
   int   mWords[MaxWords];

   //***************************************************************************
   // Methods:

   StatusResponseMsg();
   void copyToFrom(Ris::ByteBuffer* aBuffer);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

class DataRecord : public Ris::ByteContent
{
public:

   //***************************************************************************
   // Members:

   int mCode1;
   int mCode2;
   int mCode3;
   int mCode4;

   //***************************************************************************
   // Methods:

   DataRecord();
   void copyToFrom (Ris::ByteBuffer* aBuffer);
};

class DataMsg : public BaseMsg
{
public:

   //***************************************************************************
   // Members:

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

   //***************************************************************************
   // Methods:

   DataMsg();
   void copyToFrom(Ris::ByteBuffer* aBuffer);
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

