/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "my_functions.h"

#include "risRTable.h"

namespace Ris
{
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

RTable1D::RTable1D()
{
   mRows   = 0;
   mValues = 0;

   mInitialized = false;
   mFirstCmd = true;
   mAlloc = 0;
   mIndex = 0;
}

RTable1D::~RTable1D()
{
   if (mValues) delete mValues;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void RTable1D::initialize(
   int     aRows)
{
   mRows   = aRows;
   int tAlloc = mRows;
   mValues = new double[tAlloc];
   for (int i=0;i<mRows;i++) mValues[i]=0.0;

   mInitialized = true;
   mAlloc = tAlloc;
   mIndex = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

double& RTable1D::e (int aRow)
{
   return mValues[aRow];
}

double& RTable1D::operator() (int aRow)
{
   return mValues[aRow];
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void RTable1D::show(char* aLabel)
{
   char tLabel[30];

   if (aLabel==0)
   {
      strcpy(tLabel,"RTable1D");
   }
   else
   {
      strncpy(tLabel,aLabel,30);
   }

   for (int i=0;i<mRows;i++) printf("%s  %d  %10.8f\n", tLabel,i, e(i));
   printf("\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute, overload used to read from a command line file. This is called
// for each line in the corresponding table section of the file. It parses 
// the file command line to read table values
//
// For each command line, such as:
//
// "Command Arg1 Arg2 Arg3"
// 
// The number of arguments is 3
// Command is at arg[0]
// Arg1    is at arg[1]
// Arg2    is at arg[2]
// Arg3    is at arg[3]


void RTable1D::execute(Ris::CmdLineCmd* aCmd)
{
   // Argument variables
   int tNumUsableArg = 0;
   int tArgOffset    = 0;

   // If this is the first command line for the table
   if (mFirstCmd)
   {
      mFirstCmd = false;
      // If already initialized in parent code,
      // size is already specified
      if (mInitialized)
      {
         // Start using available arguments
         tNumUsableArg = aCmd->numArg();
         tArgOffset    = 1;
      }
      // If not already initialized in parent code,
      // size is specified by first argument
      else
      {
         // Initialize
         int tRows = aCmd->argInt(1);
         initialize(tRows);
         // Start using available arguments
         tNumUsableArg = aCmd->numArg() - 1;
         tArgOffset    = 2;
      }
      // Push this executive onto the nested executive stack
      nestedPush(aCmd, this);
   }
   // If this is not the first command line for the table
   else
   {
      // Use all available arguments, including arg[0],
      // beacuse there will be no command in arg[0] for this
      // command line
      tNumUsableArg = aCmd->numArg() + 1;
      tArgOffset = 0;
   }

   // Read values from arguments into values array
   for (int i = 0; i < tNumUsableArg; i++)
   {
      // Copy argument
      mValues[mIndex++] = aCmd->argDouble(tArgOffset + i);
      // If values array if full then done
      if (mIndex == mAlloc)
      {
         // Pop this executive off of the nested executive stack
         nestedPop(aCmd);
         return;
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

RTable2D::RTable2D()
{
   mRows   = 0;
   mCols   = 0;
   mValues = 0;

   mInitialized = false;
   mFirstCmd = true;
   mAlloc = 0;
   mIndex = 0;
}

RTable2D::~RTable2D()
{
   if (mValues) delete mValues;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void RTable2D::initialize(
   int     aRows,
   int     aCols)
{
   mRows   = aRows;
   mCols   = aCols;
   int tAlloc = mRows*mCols;
   mValues = new double[tAlloc];
   for (int i=0;i<tAlloc;i++) mValues[i]=0.0;

   mInitialized = true;
   mAlloc = tAlloc;
   mIndex = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

double& RTable2D::e (int aRow,int aCol)
{
   int tIndex = (aRow)*mCols + (aCol);
   return mValues[tIndex];
}

double& RTable2D::operator() (int aRow,int aCol)
{
   int tIndex = (aRow)*mCols + (aCol);
   return mValues[tIndex];
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void RTable2D::show(char* aLabel)
{
   char tLabel[30];

   if (aLabel==0)
   {
      strcpy(tLabel,"RTable2D");
   }
   else
   {
      strncpy(tLabel,aLabel,30);
   }

   for (int i=0;i<mRows;i++)
   {
      printf("%s ",tLabel);
      for (int j=0;j<mCols;j++)
      {
         printf("%10.8f ",e(i,j));
      }
      printf("\n");
   }
   printf("\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute, overload used to read from a command line file. This is called
// for each line in the corresponding table section of the file. It parses 
// the file command line to read table values
//
// For each command line, such as:
//
// "Command Arg1 Arg2 Arg3"
// 
// The number of arguments is 3
// Command is at arg[0]
// Arg1    is at arg[1]
// Arg2    is at arg[2]
// Arg3    is at arg[3]


void RTable2D::execute(Ris::CmdLineCmd* aCmd)
{
   // Argument variables
   int tNumUsableArg = 0;
   int tArgOffset    = 0;

   // If this is the first command line for the table
   if (mFirstCmd)
   {
      mFirstCmd = false;
      // If already initialized in parent code,
      // size is already specified
      if (mInitialized)
      {
         // Start using available arguments
         tNumUsableArg = aCmd->numArg();
         tArgOffset    = 1;
      }
      // If not already initialized in parent code,
      // size is specified by first argument
      else
      {
         // Initialize
         int tRows = aCmd->argInt(1);
         int tCols = aCmd->argInt(2);
         initialize(tRows,tCols);
         // Start using available arguments
         tNumUsableArg = aCmd->numArg() - 2;
         tArgOffset    = 3;
      }
      // Push this executive onto the nested executive stack
      nestedPush(aCmd, this);
   }
   // If this is not the first command line for the table
   else
   {
      // Use all available arguments, including arg[0],
      // beacuse there will be no command in arg[0] for this
      // command line
      tNumUsableArg = aCmd->numArg() + 1;
      tArgOffset = 0;
   }

   // Read values from arguments into values array
   for (int i = 0; i < tNumUsableArg; i++)
   {
      // Copy argument
      mValues[mIndex++] = aCmd->argDouble(tArgOffset + i);
      // If values array if full then done
      if (mIndex == mAlloc)
      {
         // Pop this executive off of the nested executive stack
         nestedPop(aCmd);
         return;
      }
   }
}

}//namespace

