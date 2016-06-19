#ifndef _RISBYTETMESSAGE_H_
#define _RISBYTETMESSAGE_H_
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

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a base class for classes that can be copied to/from ByteBuffers.

class ByteTMessage
{
public:
   // General purpose identifier
   int mMessageType;

   // Constructor
   ByteTMessage()
   {
      mMessageType = 0;
   }
};

//******************************************************************************
}//namespace
#endif

