#ifndef _SOMECLASS5_H_
#define _SOMECLASS5_H_

#include <stdio.h>
#include "risThreadsThreads.h"

/*==============================================================================
File: someClass4.h
Description:
QCall test Class class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************


namespace Some
{
//******************************************************************************
//******************************************************************************
//******************************************************************************

template <class MsgTraits>
class Class5 : public Ris::Threads::BaseThreadWithTermFlag
{
public:

   void threadInitFunction() override
   {
      printf("threadInitFunction\n");
   }

   void print1()
   {
      printf("print1\n");
   }
};

//******************************************************************************
}//namespace

#endif

