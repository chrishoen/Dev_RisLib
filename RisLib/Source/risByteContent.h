#ifndef _RISBYTECONTENT_H_
#define _RISBYTECONTENT_H_
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
// This is an abstract base class for classes that can be copied to/from
// byte buffers. Each inheriting class supplies a copyToFrom member function
// that can be called by ByteBuffer put and get functions to copy it.

class ByteContent
{
public:
   // This is supplied by each particular inheriting class to copy itself
   // to/from a byte buffer.

   virtual void copyToFrom(Ris::ByteBuffer* aBuffer)=0;
};

//******************************************************************************
}//namespace
#endif

