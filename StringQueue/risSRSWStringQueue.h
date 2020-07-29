#pragma once

/*==============================================================================

Single Reader Writer String Queue Class Template. 

It is single writer single reader thread safe.
It is uses no thread synchronization.

It is shared memory safe. This means no constructors, no pointers.

It is thread safe for separate single writer and single reader threads.

==============================================================================*/
//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risSRSWObjectQueue.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

template <int StringSize,int QueueSize>
class SRSWStringQueue : public SRSWObjectQueue<char[StringSize],QueueSize>
{
public:
   typedef SRSWObjectQueue<char[StringSize], QueueSize> BaseClass;

   // Initialize, there's no constructor. This is called by the process wh
   // first creates the shared memory.
   void intialize()
   {
      BaseClass::initialize();
      for (int i = 0; i < QueueSize; i++)
      {
         BaseClass::mElement[i][0] = 0;
      }
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

