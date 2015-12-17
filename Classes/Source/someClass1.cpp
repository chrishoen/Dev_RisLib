/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "prnPrint.h"

#define  _SOMEClass1_CPP_
#include "someClass1.h"

namespace Some
{

Class1::Class1() :
   mMyCallPointer(this,&Class1::myCall)
{
   // CallPointers
   mMyCallPointer.bind  (this,&Class1::myCall);
}

//******************************************************************************

void Class1::myCall(int aX)
{
   Prn::print(0,"Class1::myCall %d",aX);
}

//******************************************************************************

void Class1::configure(MyCallPointer tMyCallPointer)
{
   mMyCallPointer = tMyCallPointer;
}

}//namespace