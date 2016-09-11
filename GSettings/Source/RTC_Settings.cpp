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

#include "RTC_Settings.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

RTC_Settings::RTC_Settings()
{
   mCode1           = 0;
   mCode2           = 0;
}

void RTC_Settings::show()
{
   printf("\n");
   printf("RTC_Settings****************************************************************\n");
   printf("\n");

   printf("Code1               %d\n", mCode1);
   printf("Code2               %d\n", mCode2);
}

//******************************************************************************
// Base class overload, called for each line in the file

void RTC_Settings::execute(Ris::CmdLineCmd* aCmd)
{
   if (aCmd->isCmd("END"))        nestedPop(aCmd);

   // Read variables
   if (aCmd->isCmd("Code1"))            mCode1      = aCmd->argInt(1);
   if (aCmd->isCmd("Code2"))            mCode2      = aCmd->argInt(1);
}

