/*==============================================================================

==============================================================================*/
//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "my_functions.h"
#include "risCmdLineFile.h"
#include "risPortableCalls.h"

#include "System_Settings.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

System_Settings::System_Settings()
{
   mCode1 = 0;
   mCode2 = 0;
   mIsKepler = true;
}

void System_Settings::show()
{
   printf("\n");
   printf("System_Settings**********************************************************\n");
   printf("\n");

   printf("Code1               %d\n", mCode1);
   printf("Code2               %d\n", mCode2);
   printf("\n");
   printf("IsKepler                     %8s\n",     my_string_from_bool(mIsKepler));
   printf("\n");
   printf("\n");
}

//******************************************************************************
// Base class overload, called for each line in the file

void System_Settings::execute(Ris::CmdLineCmd* aCmd)
{
   if (aCmd->isCmd("END"))        nestedPop(aCmd);

   // Read variables
   if (aCmd->isCmd("Code1"))             mCode1 = aCmd->argInt(1);
   if (aCmd->isCmd("Code2"))             mCode2 = aCmd->argInt(1);

   if (aCmd->isCmd("IsKepler"))          mIsKepler = aCmd->argBool(1);
}

