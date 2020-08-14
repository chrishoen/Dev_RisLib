//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risPortableCalls.h"
#include "risCmdLineFile.h"
#include "risAlphaDir.h"
#include "risNetPortDef.h"


#define  _PRNPRINTSETTINGS_CPP_
#include "prnPrintSettings.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Prn
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

PrintSettings::PrintSettings()
{
   strcpy(mPrintViewIPAddress, "none");
   mPrintViewIPPort = 58000;
   mCode1 = 0;
}

void PrintSettings::reset()
{
   BaseClass::reset();

   if (Ris::portableIsWindows())
   {
      BaseClass::setFilePath("C:/Alpha/Settings/PrnPrint_Settings.txt");
   }
   else
   {
      BaseClass::setFilePath("/opt/alpha/settings/PrnPrint_Settings.txt");
   }

   strcpy(mPrintViewIPAddress, "127.0.0.1");
   mPrintViewIPPort = Ris::Net::PortDef::cPrintView;

   mCode1 = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show.

void PrintSettings::show()
{
   printf("\n");
   printf("PrintSettings************************************************ %s\n", mTargetSection);

   printf("PrintViewIPAddress      %16s\n", mPrintViewIPAddress);
   printf("PrintViewIPPort         %16d\n", mPrintViewIPPort);
   printf("Code1                   %16d\n", mCode1);

   printf("PrintSettings************************************************\n");
   printf("\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Base class override: Execute a command from the command file to set a 
// member variable.  Only process commands for the target section.This is
// called by the associated command file object for each command in the file.

void PrintSettings::execute(Ris::CmdLineCmd* aCmd)
{
   if (!isTargetSection(aCmd)) return;

   if (aCmd->isCmd("PrintViewIP"))
   {
      aCmd->copyArgString(1, mPrintViewIPAddress, cMaxStringSize);
      mPrintViewIPPort = aCmd->argInt(2);
   }

   if (aCmd->isCmd("Code1")) mCode1 = aCmd->argInt(1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Calculate expanded member variables. This is called after the entire
// section of the command file has been processed.

void PrintSettings::expand()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace