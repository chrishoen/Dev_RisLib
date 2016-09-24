#ifndef _RISBYTEMSG_H_
#define _RISBYTEMSG_H_
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
#include "risByteContent.h"
//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a base class for classes that can be copied to/from ByteBuffers.

class ByteMsg : public Ris::ByteContent
{
public:
   // General purpose identifier
   int mMessageType;

   // Constructor
   ByteMsg()
   {
      mMessageType = 0;
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is an abstract base class for a message creator. Inheriting classes 
// are used to create messages for a specific message set.

class BaseMsgCreator
{
public:

   //***********************************************************************
   // Create a new message, based on a message type.

   virtual Ris::ByteContent* createMsg (int aMessageType)=0;

};

//******************************************************************************
}//namespace
#endif

