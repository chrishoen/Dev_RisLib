#include "stdafx.h"

#include "CmdLineExec.h"
#include "Parms.h"
#include "someMyClass.h"
#include "risAlphaDir.h"
#include "risBaseDir.h"

using namespace Some;

//******************************************************************************
//******************************************************************************
//******************************************************************************

CmdLineExec::CmdLineExec()
{
}
void CmdLineExec::reset()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::execute(Ris::CmdLineCmd* aCmd)
{
   if (aCmd->isCmd("RESET"))  reset();
   if (aCmd->isCmd("GO1"))    executeGo1(aCmd);
   if (aCmd->isCmd("GO2"))    executeGo2(aCmd);
   if (aCmd->isCmd("GO3"))    executeGo3(aCmd);
   if (aCmd->isCmd("GO4"))    executeGo4(aCmd);
   if (aCmd->isCmd("GO5"))    executeGo5(aCmd);
   if (aCmd->isCmd("GO6"))    executeGo6(aCmd);
   if (aCmd->isCmd("Parms"))  executeParms(aCmd);
   if (aCmd->isCmd("Parms2")) executeParms2(aCmd);

   if (aCmd->isCmd("RUN1"))  executeRun1(aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
   char tBuffer[400];

   Prn::print(0, "AlphaDir            %s", Ris::getAlphaDirectory());
   Prn::print(0, "Alpha Files         %s", Ris::getAlphaFilePath_Files(tBuffer));
   Prn::print(0, "Alpha Filse         %s", Ris::getAlphaFilePath_Files(tBuffer, "BackEnd.cs"));
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   Prn::print(0, "portableGetCurrentDir         %s", Ris::portableGetCurrentDir());
   Prn::print(0, "portableGetBinDir         %s", Ris::portableGetBinDir());
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo3(Ris::CmdLineCmd* aCmd)
{
   char tBuffer[400];

   Prn::print(0, "BaseDir             %s", Ris::getBaseDirectory());
   Prn::print(0, "BackEnd.cs          %s", Ris::getBaseFullFilePath(tBuffer, "files/RisLib/BackEnd.cs"));
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo4(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo5(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void my_local_function()
{
   printf("my_local_function\n");
}

void CmdLineExec::executeGo6(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeRun1(Ris::CmdLineCmd* aCmd)
{
   gParms.reset();
   gParms.readSection("default");
   gParms.readSection("run1");

   MyClass tMyClass(gParms.mMyClassParms1);
   tMyClass.doRun1();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeParms(Ris::CmdLineCmd* aCmd)
{
   gParms.reset();
   gParms.readSection("default");
   gParms.show();
}


//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeParms2(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, "Run1");
   gParms.reset();
   Prn::print(0, "gParms.mFilePath         %s", gParms.mFilePath);
   gParms.readSection("default");
   gParms.readSection(aCmd->argString(1));
   gParms.show();
}


