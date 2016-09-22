#ifndef _RISBYTEMSGB_H_
#define _RISBYTEMSGB_H_
/*==============================================================================

ByteContent and ByteBuffer classes.

ByteBuffer is a smart pointer that addresses a fixed section of memory.

ByteContent is a base class for something that can be copied to/from a
ByteBuffer.

These classes are intended to support the construction of message classes.
The idea here is that each message is encapsulated by a class that inherits
from ByteContent and supplies a member function that allows it to be copied
to/from a ByteBuffer, which can be transported via some communications
medium. All copies are done in network order. 

==============================================================================*/
#include "risByteBuffer.h"
//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a base class for classes that can be copied to/from ByteBuffers.

class ByteMsgB
{
public:
   // General purpose identifier
   int mMessageType;

   // Constructor
   ByteMsgB()
   {
      mMessageType = 0;
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is an abstract base class for a message creator. Inheriting classes 
// are used to create messages for a specific message set.

class BaseMsgBCreator
{
public:

   //***********************************************************************
   // Create a new record, based on a record type.

   virtual Ris::ByteMsgB* createMessage (int aType)=0;

};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is an abstract base class for a message copier. Inheriting classes 
// are used to copy messages for a specific message set to/from byte buffers.

class  BaseMsgBCopier
{
public:

   //***********************************************************************
   // Copy records to/from a byte buffer.

   virtual void copyToFrom( Ris::ByteBuffer* aBuffer, Ris::ByteMsgB* aMsgB)=0;
};

//******************************************************************************
}//namespace
#endif

