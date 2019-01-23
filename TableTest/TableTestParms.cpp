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
   strcpy(BaseClass::mDefaultFileName, "TableTestParms.txt");

   mTable1d.initialize(4);
   mTable2d.initialize(4,2);
   mIntTable2d.initialize(4,2);

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
   mTable1d.show("Table1d");

   printf("\n");
   mTable2d.show("Table2d");

   printf("\n");
   mIntTable2d.show("IntTable2d");

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

   if (aCmd->isCmd("Table1d"))   mTable1d.execute(aCmd);
   if (aCmd->isCmd("Table2d"))   mTable2d.execute(aCmd);
   if (aCmd->isCmd("IntTable2d"))   mIntTable2d.execute(aCmd);

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
