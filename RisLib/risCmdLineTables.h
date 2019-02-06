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

   // Specialize.
   template <>
   void resetElement<bool>(ElementType& aX)
   {
      aX = false;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Access array elements.

   ElementType& e(int aRow)
   {
      return this->operator[](aRow);
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Show.
   void show(char* aLabel = 0)
   {
      for (int i = 0; i < mRows; i++)
      {
         // Specialize.
         showRow<ElementType>(aLabel, i);
      }
   }

   // Specialize.
   template <typename T>
   void showRow(char* aLabel, int aRow)
   {
   }

   // Specialize.
   template <>
   void showRow<int>(char* aLabel, int aRow)
   {
      printf("%-20s %10d\n", aLabel, e(aRow));
   }

   // Specialize.
   template <>
   void showRow<double>(char* aLabel, int aRow)
   {
      printf("%-20s %10.4f\n", aLabel, e(aRow));
   }

   // Specialize.
   template <>
   void showRow<bool>(char* aLabel, int aRow)
   {
      printf("%-20s %8s\n", aLabel, my_string_from_bool(e(aRow)));
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Execute, read from a command line file. This is called via a nested push.
   void execute(Ris::CmdLineCmd* aCmd) override
   {
      // This should be the first command after the nested push.
      if (aCmd->isCmd("{"))
      {
      }
      // Pop back out at the end.
      else if (aCmd->isCmd("}"))
      {
         nestedPop(aCmd);
      }
      // This should be a numerical value.
      else if (mRows < MaxRows)
      {
         // Specialize.
         readRow<ElementType>(aCmd);
      }
   }
   
   // Specialize.
   template <typename T>
   void readRow(Ris::CmdLineCmd* aCmd)
   {
   }

   // Specialize.
   template <>
   void readRow<int>(Ris::CmdLineCmd* aCmd)
   {
      e(mRows++) = aCmd->argInt(0);
   }

   // Specialize.
   template <>
   void readRow<double>(Ris::CmdLineCmd* aCmd)
   {      
      e(mRows++) = aCmd->argDouble(0);
   }

   // Specialize.
   template <>
   void readRow<bool>(Ris::CmdLineCmd* aCmd)
   {
      e(mRows++) = aCmd->argBool(0);
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Two dimensional table.

template<typename ElementType, size_t MaxRows, size_t MaxCols>
class CmdLineTable2D : public std::array<ElementType[MaxCols], MaxRows>, public Ris::BaseCmdLineExec
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Last row number that was read from the file.
   int  mRows;
   int  mCols;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   CmdLineTable2D()
   {
      reset();
   }

   // Reset variables.
   void reset()
   {
      mRows = 0;
      for (int i = 0; i < MaxRows; i++)
      {
         for (int j = 0; j < MaxCols; j++)
         {
            // Specialize.
            resetElement<ElementType>(e(i,j));
         }
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

   // Specialize.
   template <>
   void resetElement<bool>(ElementType& aX)
   {
      aX = false;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Access array elements.

   ElementType& e(int aRow, int aCol)
   {
      return this->operator[](aRow)[aCol];
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Show.
   void show(char* aLabel = 0)
   {
      for (int i = 0; i < mRows; i++)
      {
         printf("%-24s ", aLabel);
         for (int j = 0; j < mCols; j++)
         {
            // Specialize.
            showElement<ElementType>(i, j);
         }
         printf("\n");
      }
   }

   // Specialize.
   template <typename T>
   void showElement(int aRow, int aCol)
   {
   }

   // Specialize.
   template <>
   void showElement<int>(int aRow, int aCol)
   {
      printf("%10d ",e(aRow,aCol));
   }

   // Specialize.
   template <>
   void showElement<double>(int aRow, int aCol)
   {
      printf("%10.4f ", e(aRow, aCol));
   }

   // Specialize.
   template <>
   void showElement<bool>(int aRow, int aCol)
   {
      printf("%8s ", my_string_from_bool(e(aRow, aCol)));
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Execute, read from a command line file. This is called via a nested push.
   void execute(Ris::CmdLineCmd* aCmd) override
   {
      // This should be the first command after the nested push.
      if (aCmd->isCmd("{"))
      {
      }
      // Pop back out at the end.
      else if (aCmd->isCmd("}"))
      {
         nestedPop(aCmd);
      }
      // This should be a numerical value.
      else if (mRows < MaxRows)
      {
         // Specialize.
         readRow<ElementType>(aCmd);
      }
   }

   // Specialize.
   template <typename T>
   void readRow(Ris::CmdLineCmd* aCmd)
   {
   }

   // Specialize.
   template <>
   void readRow<int>(Ris::CmdLineCmd* aCmd)
   {
      mCols = aCmd->numArg() + 1;
      for (int j = 0; j < mCols; j++)
      {
         e(mRows, j) = aCmd->argInt(j);
      }
      mRows++;
   }

   // Specialize.
   template <>
   void readRow<double>(Ris::CmdLineCmd* aCmd)
   {
      mCols = aCmd->numArg() + 1;
      for (int j = 0; j < mCols; j++)
      {
         e(mRows, j) = aCmd->argDouble(j);
      }
      mRows++;
   }

   // Specialize.
   template <>
   void readRow<bool>(Ris::CmdLineCmd* aCmd)
   {
      mCols = aCmd->numArg() + 1;
      for (int j = 0; j < mCols; j++)
      {
         e(mRows, j) = aCmd->argBool(j);
      }
      mRows++;
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace




