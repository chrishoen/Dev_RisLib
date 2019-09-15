//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risCmdLineFile.h"

#define  _PARMS_CPP_
#include "Parms.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

Parms::Parms()
{
   reset();
}

void Parms::reset()
{
   BaseClass::reset();
   BaseClass::setFileName_RelativeToBaseDir("files/RisLib/RisParms_Parms.txt");

   mCode1 = 0;
   mCode2 = 0;
   mCode3 = 0;
   mCode4 = 0;

   mMyClassParms1.reset();
   mMyClassParms2.reset();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Calculate expanded member variables. This is called after the entire
// section of the command file has been processed.

void Parms::expand()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show.

void Parms::show()
{
   printf("Parms ***************** BEGIN %s\n", BaseClass::mTargetSection);

   printf("Code1              %10d\n", mCode1);
   printf("Code2              %10d\n", mCode2);
   printf("Code3              %10d\n", mCode3);
   printf("Code4              %10d\n", mCode4);

   mMyClassParms1.show("MyClassParms1");
   mMyClassParms2.show("MyClassParms2");

   printf("Parms ******************* END %s\n", BaseClass::mTargetSection);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Base class override: Execute a command from the command file to set a 
// member variable.  Only process commands for the target section.This is
// called by the associated command file object for each command in the file.

void Parms::execute(Ris::CmdLineCmd* aCmd)
{
   if (!isTargetSection(aCmd)) return;

   if (aCmd->isCmd("Code1"))  mCode1 = aCmd->argInt(1);
   if (aCmd->isCmd("Code2"))  mCode2 = aCmd->argInt(1);
   if (aCmd->isCmd("Code3"))  mCode3 = aCmd->argInt(1);
   if (aCmd->isCmd("Code4"))  mCode4 = aCmd->argInt(1);

   if (aCmd->isCmd("MyClassParms1")) nestedPush(aCmd, &mMyClassParms1);
   if (aCmd->isCmd("MyClassParms2")) readSection(aCmd->argString(1), &mMyClassParms2);
}

