#pragma once
/*==============================================================================

Table class templates that can be used by the command line file classes.
One or two dimensional table classes of int,double,string.

==============================================================================*/

#include "stdlib.h"
#include "stdio.h"
#include <array>

#include "my_functions.h"
#include "risCmdLineExec.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Fixed length string.

// Fixed length string used in 1d string table.
static const int cTableStringSize = 200;
typedef char TableString[cTableStringSize];

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// One dimesnional table.

template<typename ElementType,size_t MaxRows>
class CmdLineTable1D : public std::array<ElementType, MaxRows>, public Ris::BaseCmdLineExec
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Last row number that was read from the file.
   int  mRows;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   CmdLineTable1D()
   {
      reset();
   }

   ElementType& e(int aRow)
   {
      return this->operator[](aRow);
   }

   // Reset variables.
   void reset()
   {
      mRows = 0;
      for (int i = 0; i < MaxRows; i++)
      {
         // Specialize.
         resetElement<ElementType>(e(i));
      }
   }

   // Specialize.
   template <typename T>
   void resetElement(ElementType& aX)
   {
   }

   // Specialize.
   template <>
   void resetElement<int>(ElementType& aX)
   {
      aX = 0;
   }

   // Specialize.
   template <>
   void resetElement<double>(ElementType& aX)
   {
      aX = 0;
   }

};


//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace




