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

   mIntTable1d.reset();
   mDoubleTable1d.reset();

   mIntTable2d.reset();
   mDoubleTable2d.reset();

   mBoolTable1d.reset();
   mBoolTable2d.reset();

   mStringTable1d.reset();
   mTestTable.reset();

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
   mIntTable1d.show("IntTable1d");

   printf("\n");
   mDoubleTable1d.show("DoubleTable1d");

   printf("\n");
   mIntTable2d.show("IntTable2d");

   printf("\n");
   mDoubleTable2d.show("DoubleTable2d");

   printf("\n");
   mBoolTable1d.show("BoolTable1d");

   printf("\n");
   mBoolTable2d.show("BoolTable2d");

   printf("\n");
   mStringTable1d.show("StringTable1d");

   printf("\n");
   printf("Code1                %10d\n",  mCode1);
   printf("Code2                %10d\n",  mCode2);

   printf("\n");
   printf("TestTableSize            %10d %4d\n", mTestTable.mRows, mTestTable.mCols);
   mTestTable.show("TestTable");

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

   if (aCmd->isCmd("IntTable1d"))     nestedPush(aCmd, &mIntTable1d);
   if (aCmd->isCmd("DoubleTable1d"))  nestedPush(aCmd, &mDoubleTable1d);
   if (aCmd->isCmd("IntTable2d"))     nestedPush(aCmd, &mIntTable2d);
   if (aCmd->isCmd("DoubleTable2d"))  nestedPush(aCmd, &mDoubleTable2d);

   if (aCmd->isCmd("BoolTable1d"))    nestedPush(aCmd, &mBoolTable1d);
   if (aCmd->isCmd("BoolTable2d"))    nestedPush(aCmd, &mBoolTable2d);

   if (aCmd->isCmd("StringTable1d"))  nestedPush(aCmd, &mStringTable1d);

   if (aCmd->isCmd("Code1"))     mCode1 = aCmd->argInt(1);
   if (aCmd->isCmd("Code2"))     mCode2 = aCmd->argInt(1);

   if (aCmd->isCmd("TestTable"))     nestedPush(aCmd, &mTestTable);
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
