#pragma once
/*==============================================================================

Table class templates that can be used by the command line file classes.
One or two dimensional table classes of int,double,string.

==============================================================================*/

#include "stdlib.h"
#include "stdio.h"
#include <type_traits>
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

   // Reset variables.
   void reset()
   {
      mRows = 0;
      for (int i = 0; i < (int)MaxRows; i++)
      {
         // Specialize.
         resetElement(e(i));
      }
   }

   // Specialize.
   template<typename T = ElementType, typename std::enable_if<std::is_same<T, int>::value>::type* = nullptr >
   void resetElement(int& aX)
   {
      aX = 0;
   }

   // Specialize.
   template<typename T = ElementType, typename std::enable_if<std::is_same<T, double>::value>::type* = nullptr >
   void resetElement(double& aX)
   {
      aX = 0;
   }

   // Specialize.
   template<typename T = ElementType, typename std::enable_if<std::is_same<T, bool>::value>::type* = nullptr >
   void resetElement(bool& aX)
   {
      aX = false;
   }

   // Specialize.
   template<typename T = ElementType, typename std::enable_if<std::is_same<T, TableString>::value>::type* = nullptr >
   void resetElement(TableString& aX)
   {
      aX[0]=0;
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
         showRow(aLabel, i);
      }
   }

   // Specialize.
   template<typename T = ElementType, typename std::enable_if<std::is_same<T, int>::value>::type* = nullptr >
   void showRow(char* aLabel, int aRow)
   {
      printf("%-20s %10d\n", aLabel, e(aRow));
   }

   // Specialize.
   template<typename T = ElementType, typename std::enable_if<std::is_same<T, double>::value>::type* = nullptr >
   void showRow(char* aLabel, int aRow)
   {
      printf("%-20s %10.4f\n", aLabel, e(aRow));
   }

   // Specialize.
   template<typename T = ElementType, typename std::enable_if<std::is_same<T, bool>::value>::type* = nullptr >
   void showRow(char* aLabel, int aRow)
   {
      printf("%-20s %8s\n", aLabel, my_string_from_bool(e(aRow)));
   }

   // Specialize.
   template<typename T = ElementType, typename std::enable_if<std::is_same<T, TableString>::value>::type* = nullptr >
   void showRow(char* aLabel, int aRow)
   {
      printf("%-20s %s\n", aLabel, e(aRow));
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
      else if (mRows < (int)MaxRows)
      {
         // Specialize.
         readRow(aCmd);
      }
   }
   
   // Specialize.
   template<typename T = ElementType, typename std::enable_if<std::is_same<T, int>::value>::type* = nullptr >
   void readRow(Ris::CmdLineCmd* aCmd)
   {
      e(mRows++) = aCmd->argInt(0);
   }

   // Specialize.
   template<typename T = ElementType, typename std::enable_if<std::is_same<T, double>::value>::type* = nullptr >
   void readRow(Ris::CmdLineCmd* aCmd)
   {      
      e(mRows++) = aCmd->argDouble(0);
   }

   // Specialize.
   template<typename T = ElementType, typename std::enable_if<std::is_same<T, bool>::value>::type* = nullptr >
   void readRow(Ris::CmdLineCmd* aCmd)
   {
      e(mRows++) = aCmd->argBool(0);
   }

   // Specialize.
   template<typename T = ElementType, typename std::enable_if<std::is_same<T, TableString>::value>::type* = nullptr >
   void readRow(Ris::CmdLineCmd* aCmd)
   {
      aCmd->copyArgString(0, e(mRows++), cTableStringSize);
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
      for (int i = 0; i < (int)MaxRows; i++)
      {
         for (int j = 0; j < (int)MaxCols; j++)
         {
            // Specialize.
            resetElement(e(i,j));
         }
      }
   }

   // Specialize.
   template<typename T = ElementType, typename std::enable_if<std::is_same<T, int>::value>::type* = nullptr >
   void resetElement(int& aX)
   {
      aX = 0;
   }

   // Specialize.
   template<typename T = ElementType, typename std::enable_if<std::is_same<T, double>::value>::type* = nullptr >
   void resetElement(double& aX)
   {
      aX = 0;
   }

   // Specialize.
   template<typename T = ElementType, typename std::enable_if<std::is_same<T, bool>::value>::type* = nullptr >
   void resetElement(bool& aX)
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
            showElement(i, j);
         }
         printf("\n");
      }
   }

   // Specialize.
   template<typename T = ElementType, typename std::enable_if<std::is_same<T, int>::value>::type* = nullptr >
   void showElement(int aRow, int aCol)
   {
      printf("%10d ",e(aRow,aCol));
   }

   // Specialize.
   template<typename T = ElementType, typename std::enable_if<std::is_same<T, double>::value>::type* = nullptr >
   void showElement(int aRow, int aCol)
   {
      printf("%10.4f ", e(aRow, aCol));
   }

   // Specialize.
   template<typename T = ElementType, typename std::enable_if<std::is_same<T, bool>::value>::type* = nullptr >
   void showElement(int aRow, int aCol)
   {
      printf("%1d ", e(aRow, aCol));
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
      else if (mRows < (int)MaxRows)
      {
         // Specialize.
         readRow(aCmd);
      }
   }

   // Specialize.
   template<typename T = ElementType, typename std::enable_if<std::is_same<T, int>::value>::type* = nullptr >
   void readRow(Ris::CmdLineCmd* aCmd)
   {
      mCols = my_imin(aCmd->numArg() + 1,MaxCols);
      for (int j = 0; j < mCols; j++)
      {
         e(mRows, j) = aCmd->argInt(j);
      }
      mRows++;
   }

   // Specialize.
   template<typename T = ElementType, typename std::enable_if<std::is_same<T, double>::value>::type* = nullptr >
   void readRow(Ris::CmdLineCmd* aCmd)
   {
      mCols = my_imin(aCmd->numArg() + 1, MaxCols);
      for (int j = 0; j < mCols; j++)
      {
         e(mRows, j) = aCmd->argDouble(j);
      }
      mRows++;
   }

   // Specialize.
   template<typename T = ElementType, typename std::enable_if<std::is_same<T, bool>::value>::type* = nullptr >
   void readRow(Ris::CmdLineCmd* aCmd)
   {
      mCols = my_imin(aCmd->numArg() + 1, MaxCols);
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




