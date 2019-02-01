/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "my_functions.h"

#include "risCmdLineTables.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

CmdLineIntTable1D::CmdLineIntTable1D()
{
   mValues = 0;
   reset();
}

void CmdLineIntTable1D::reset()
{
   if (mValues) delete mValues;
   mValues = 0;

   mRows   = 0;
   mInitialized = false;
   mFirstCmd = true;
   mAlloc = 0;
   mIndex = 0;
   mValidFlag = false;
}

CmdLineIntTable1D::~CmdLineIntTable1D()
{
   if (mValues) delete mValues;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineIntTable1D::initialize(int aRows)
{
   reset();

   mRows  = aRows;
   mAlloc = mRows;

   mValues = new int[mAlloc];
   for (int i=0;i<mAlloc;i++) mValues[i]=0;

   mInitialized = true;
   mIndex = 0;
   mValidFlag = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

int& CmdLineIntTable1D::e (int aRow)
{
   return mValues[aRow];
}

int& CmdLineIntTable1D::operator() (int aRow)
{
   return mValues[aRow];
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineIntTable1D::show(char* aLabel)
{
   if (!mValidFlag)
   {
      printf("Table1D %s NOT VALID\n", aLabel);
      return;
   }

   char tLabel[30];

   if (aLabel==0)
   {
      strcpy(tLabel,"CmdLineIntTable1D");
   }
   else
   {
      strncpy(tLabel,aLabel,30);
   }

   for (int i=0;i<mRows;i++) printf("%s  %d  %10d\n", tLabel,i, e(i));
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


void CmdLineIntTable1D::execute(Ris::CmdLineCmd* aCmd)
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
      mValues[mIndex++] = aCmd->argInt(tArgOffset + i);
      // If values array if full then done
      if (mIndex == mAlloc)
      {
         // Set valid
         mValidFlag = true;
         // Pop this executive off of the nested executive stack
         nestedPop(aCmd);
         return;
      }
   }
}

void CmdLineIntTable1D::reexecute(Ris::CmdLineCmd* aCmd)
{
   mFirstCmd = true;
   mIndex = 0;
   mValidFlag = false;

   execute(aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

