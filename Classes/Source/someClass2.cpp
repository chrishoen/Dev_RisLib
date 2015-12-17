/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "prnPrint.h"

#define  _SOMECLASS2_CPP_
#include "someClass2.h"

namespace Some
{

Class2::Class2() :
   mMyCallPointer(this,&Class2::myCall),
   mMyCallback(*this,&Class2::myCall)
{
   // CallPointers
   mMyCallPointer.bind  (this,&Class2::myCall);

}

//******************************************************************************

void Class2::myCall(int aX)
{
   Prn::print(0,"Class2::myCall %d",aX);
}

//******************************************************************************

void Class2::configure(MyCallPointer tMyCallPointer)
{
   mMyCallPointer = tMyCallPointer;
}

}//namespace