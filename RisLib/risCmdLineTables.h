#pragma once
/*==============================================================================

Table classes that can be used by the command line file classes.
One or two dimensional tables classes of int,double,string.
One dimensional talble of string.

==============================================================================*/

#include "stdlib.h"
#include "stdio.h"
#include <type_traits>
#include <array>

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
// Table, 1 dimensional, indexed by 0..R-1

template<typename ElementType,size_t MaxRows>
class CmdLineTable1D : public std::array<ElementType, MaxRows>, public Ris::BaseCmdLineExec
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Vector components.
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
      for (int i = 0; i < MaxRows; i++) e(i) = 0;
   }

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
         showRow<ElementType>(aLabel, i);
      }
      printf("\n");
   }

   template <typename T>
   void showRow(char* aLabel, int aRow)
   {
   }

   template <>
   void showRow<int>(char* aLabel, int aRow)
   {
      printf("%-20s %10d\n", aLabel, e(aRow));
   }

   template <>
   void showRow<double>(char* aLabel, int aRow)
   {
      printf("%-20s %10.4f\n", aLabel, e(aRow));
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
         readRow<ElementType>(aCmd);
      }
   }
   
   template <typename T>
   void readRow(Ris::CmdLineCmd* aCmd)
   {
   }

   template <>
   void readRow<int>(Ris::CmdLineCmd* aCmd)
   {
      e(mRows++) = aCmd->argInt(0);
   }

   template <>
   void readRow<double>(Ris::CmdLineCmd* aCmd)
   {      
      e(mRows++) = aCmd->argDouble(0);
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Table, 1 dimensional, indexed by 0..R-1

template<typename ElementType, size_t MaxRows, size_t MaxCols>
class CmdLineTable2D : public std::array<ElementType[MaxCols], MaxRows>, public Ris::BaseCmdLineExec
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Vector components.
   int  mRows;

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
            e(i, j) = 0;
         }
      }
   }

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
         printf("%-20s ", aLabel);
         for (int j = 0; j < MaxCols; j++)
         {
            showElement<ElementType>(i, j);
         }
         printf("\n");
      }
      printf("\n");
   }

   template <typename T>
   void showElement(int aRow, int aCol)
   {
   }

   template <>
   void showElement<int>(int aRow, int aCol)
   {
      printf("%10d ",e(aRow,aCol));
   }

   template <>
   void showElement<double>(int aRow, int aCol)
   {
      printf("%10.4f ", e(aRow, aCol));
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
         readRow<ElementType>(aCmd);
      }
   }

   template <typename T>
   void readRow(Ris::CmdLineCmd* aCmd)
   {
   }

   template <>
   void readRow<int>(Ris::CmdLineCmd* aCmd)
   {
      for (int j = 0; j < aCmd->numArg() + 1; j++)
      {
         e(mRows, j) = aCmd->argInt(j);
      }
      mRows++;
   }

   template <>
   void readRow<double>(Ris::CmdLineCmd* aCmd)
   {
      for (int j = 0; j < aCmd->numArg() + 1; j++)
      {
         e(mRows, j) = aCmd->argDouble(j);
      }
      mRows++;
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

}//namespace




