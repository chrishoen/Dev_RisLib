//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

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
   strcpy(mPrintViewRemoteIPAddress, "none");
   strcpy(mPrintViewLocalIPAddress, "none");
   mPrintViewIPPort = 99;
   mCode1 = 0;
}

void PrintSettings::reset()
{
   BaseClass::reset();
   BaseClass::setFileName_RelAlphaSettings("PrnPrint_Settings.txt");

   strcpy(mPrintViewLocalIPAddress, "127.0.0.1");
   strcpy(mPrintViewRemoteIPAddress, "127.0.0.1");
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

   printf("PrintViewLocal          %16s\n", mPrintViewLocalIPAddress);
   printf("PrintViewRemote         %16s\n", mPrintViewRemoteIPAddress);
   printf("PrintViewPort           %5d\n",  mPrintViewIPPort);
   printf("Code1                   %5d\n",  mCode1);

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

   if (aCmd->isCmd("PrintViewLocalIPAddress"))  aCmd->copyArgString(1, mPrintViewLocalIPAddress, cMaxStringSize);
   if (aCmd->isCmd("PrintViewRemoteIPAddress")) aCmd->copyArgString(1, mPrintViewRemoteIPAddress, cMaxStringSize);
   if (aCmd->isCmd("PrintViewIPPort")) mPrintViewIPPort = aCmd->argInt(1);

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