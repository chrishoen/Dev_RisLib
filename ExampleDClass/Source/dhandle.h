#ifndef _DHANDLE_H_
#define _DHANDLE_H_

#include <stdlib.h>
#include <stdio.h>

//******************************************************************************
//******************************************************************************
//******************************************************************************

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

#endif

