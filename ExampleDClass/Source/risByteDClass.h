#ifndef _RISBYTEDCLASS_H_
#define _RISBYTEDCLASS_H_
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
#include <stdlib.h>
#include <stdio.h>

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{

   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   // This is a handle for DClasses, IT contains a type and a pointer

   class DHandle
   {
   public:

      // Members

      int   mType;
      void* mPtr;

      // Constructor

      DHandle()
      {
         mType = 0;
         mPtr  = 0;
      }

      DHandle(int aType,void* aPtr)
      {
         mType = aType;
         mPtr  = aPtr;
      }

      void reset()
      {
         mType = 0;
         mPtr  = 0;
      }

      void set(int aType,void* aPtr)
      {
         mType = aType;
         mPtr  = aPtr;
      }

      void deallocate()
      {
         if (mPtr != 0)
         {
            free (mPtr);
         }
      }
   };

//******************************************************************************
}//namespace
#endif

