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

#include "SCP_Settings.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

SCP_Settings::SCP_Settings()
{
   mCode1           = 0;
   mCode2           = 0;

   mEnable_StripChart1 = false;
   mEnable_StripChart2 = false;

   mMount_disk1.setName("Mount_disk1");
   mMount_disk2.setName("Mount_disk2");
   mMount_disk3.setName("Mount_disk3");
   mMount_disk4.setName("Mount_disk4");
}

void SCP_Settings::show()
{
   printf("\n");
   printf("SCP_Settings****************************************************************\n");
   printf("\n");

   printf("Code1               %d\n", mCode1);
   printf("Code2               %d\n", mCode2);
   printf("\n");

   printf("Enable_StripChart1 %16s\n", my_string_from_bool(mEnable_StripChart1));
   printf("Enable_StripChart2 %16s\n", my_string_from_bool(mEnable_StripChart2));
   printf("\n");

   mMount_disk1.show();
   mMount_disk2.show();
   mMount_disk3.show();
   mMount_disk4.show();
}

//******************************************************************************
// Base class overload, called for each line in the file

void SCP_Settings::execute(Ris::CmdLineCmd* aCmd)
{
   if (aCmd->isCmd("END"))              nestedPop(aCmd);

   if (aCmd->isCmd("Begin"))            executeOnBegin(aCmd);

   // Read variables
   if (aCmd->isCmd("Code1"))            mCode1      = aCmd->argInt(1);
   if (aCmd->isCmd("Code2"))            mCode2      = aCmd->argInt(1);

   if(aCmd->isCmd("Enable_StripChart1" ))   mEnable_StripChart1 = aCmd->argBool(1);
   if(aCmd->isCmd("Enable_StripChart2" ))   mEnable_StripChart2 = aCmd->argBool(1);
}

//******************************************************************************
// Execute specific command

void SCP_Settings::executeOnBegin(Ris::CmdLineCmd* aCmd)
{
   //------------------------------------------------------
   if      (aCmd->isArgString(1, "Mount_disk1"))
   {
      nestedPush(aCmd,&mMount_disk1);
   }
   else if (aCmd->isArgString(1, "Mount_disk2"))
   {
      nestedPush(aCmd,&mMount_disk2);
   }
   else if (aCmd->isArgString(1, "Mount_disk3"))
   {
      nestedPush(aCmd,&mMount_disk3);
   }
   else if (aCmd->isArgString(1, "Mount_disk4"))
   {
      nestedPush(aCmd,&mMount_disk4);
   }
}

