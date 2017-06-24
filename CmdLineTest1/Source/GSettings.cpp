/*==============================================================================

==============================================================================*/
//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "risCmdLineFile.h"
#include "risPortableCalls.h"
#include "prnPrint.h"

#define  _GSETTINGS_CPP_
#include "GSettings.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

Settings_01::Settings_01()
{
   mN1=1;
   mN2=2;
}

//******************************************************************************

void Settings_01::show()
{
   Prn::print(0,"Settings_01 N1        %d",mN1);
   Prn::print(0,"Settings_01 N2        %d",mN2);
}

//******************************************************************************
// Base class overload, called for each line in the file

void Settings_01::execute(Ris::CmdLineCmd* aCmd)
{
   // Read variables
   if(aCmd->isCmd("N1" ))         mN1        = aCmd->argInt  (1);
   if(aCmd->isCmd("N2" ))         mN2        = aCmd->argInt  (1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

Settings_1::Settings_1()
{
   mN1=1;
   mN2=2;
}

//******************************************************************************

void Settings_1::show()
{
   Prn::print(0,"Settings_1 N1         %d",mN1);
   Prn::print(0,"Settings_1 N2         %d",mN2);
   mS_01.show();
}

//******************************************************************************
// Base class overload, called for each line in the file that starts with Begin

Ris::BaseCmdLineExec* Settings_1::executeForBegin(Ris::CmdLineCmd* aCmd)
{
   if(aCmd->isArgString(1,"Settings_01"))            return &mS_01;
   else return 0;
}

//******************************************************************************
// Base class overload, called for each line in the file

void Settings_1::execute(Ris::CmdLineCmd* aCmd)
{
   // Read variables
   if(aCmd->isCmd("N1" ))         mN1        = aCmd->argInt  (1);
   if(aCmd->isCmd("N2" ))         mN2        = aCmd->argInt  (1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

GSettings::GSettings()
{
   mN1=1;
   mN2=2;
}

//******************************************************************************

void GSettings::show()
{
   Prn::print(0,"GSettings N1 %d",mN1);
   Prn::print(0,"GSettings N2 %d",mN2);
   mS_1.show();
}

//******************************************************************************
// Base class overload, called for each line in the file that starts with Begin

Ris::BaseCmdLineExec* GSettings::executeForBegin(Ris::CmdLineCmd* aCmd)
{
   if(aCmd->isArgString(1,"Settings_1"))   return &mS_1;
   else return 0;
}

//******************************************************************************
// Base class overload, called for each line in the file

void GSettings::execute(Ris::CmdLineCmd* aCmd)
{
   // Read variables
   if(aCmd->isCmd("N1"))   mN1 = aCmd->argInt (1);
   if(aCmd->isCmd("N2"))   mN2 = aCmd->argInt (1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void GSettings::initialize()
{ 

   char tFilePath[200];
   Ris::CmdLineFile tCmdLineFile;
   int tFileFound = 0;

   // Try opening settings file from current directory
   strcpy(tFilePath,"GSettings.txt");
   if (tCmdLineFile.open(tFilePath))
   {
      tFileFound = 1;
   }

   // Try opening settings file from current working directory
   if (tFileFound == 0)
   {
      strcpy(tFilePath,Ris::portableGetCurrentDir());
      strcat(tFilePath,"GSettings.txt");
      if (tCmdLineFile.open(tFilePath))
      {
         tFileFound = 2;
      }
   }

   // Try opening settings file from settings directory
   if (tFileFound == 0)
   {
      strcpy(tFilePath,Ris::portableGetSettingsDir());
      strcat(tFilePath,"GSettings.txt");
      if (tCmdLineFile.open(tFilePath))
      {
         tFileFound = 3;
      }
   }

   // Read file if it was found
   if (tFileFound)
   {
      // Read file using this command line executive
      mSectionCmdLineExec=0;
      tCmdLineFile.execute(this);
      tCmdLineFile.close();

	   Prn::print(0,"GSettings::initialize PASS %d",tFileFound);
   }
   else
   {
	   Prn::print(0,"GSettings::initialize FAIL");
   }
}

