//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risCmdLineFile.h"

#define  _IMAGETESTPARMS_CPP_
#include "TableTestParms.h"


//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

TableTestParms::TableTestParms()
{
   reset();
}

void TableTestParms::reset()
{
   BaseClass::reset();
   BaseClass::setFileName_RelAlphaFiles("RisLib/TableTestParms.txt");

   mTable1dInt.reset();

   mCode1=0;
   mCode2=0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show.

void TableTestParms::show()
{
   printf("\n");
   printf("TableTestParms************************************************ %s\n", mTargetSection);

   printf("\n");
   mTable1dInt.show("Table1dInt");

   printf("\n");
   printf("Code1                  %10d\n",        mCode1);
   printf("Code2                  %10d\n",        mCode2);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Base class override: Execute a command from the command file to set a 
// member variable.  Only process commands for the target section.This is
// called by the associated command file object for each command in the file.

void TableTestParms::execute(Ris::CmdLineCmd* aCmd)
{
   if (!isTargetSection(aCmd)) return;

   if (aCmd->isCmd("Table1dInt"))  nestedPush(aCmd, &mTable1dInt);

   if (aCmd->isCmd("Code1"))     mCode1 = aCmd->argInt(1);
   if (aCmd->isCmd("Code2"))     mCode2 = aCmd->argInt(1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Calculate expanded member variables. This is called after the entire
// section of the command file has been processed.

void TableTestParms::expand()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
