#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <random>

#include "prnPrint.h"
#include "CmdLineExec.h"
#include "Parms.h"
#include "someMyClass.h"

using namespace Some;

//******************************************************************************
CmdLineExec::CmdLineExec()
{
}
//******************************************************************************
void CmdLineExec::reset()
{
}

//******************************************************************************
void CmdLineExec::execute(Ris::CmdLineCmd* aCmd)
{
   if (aCmd->isCmd("RESET")) reset();
   if (aCmd->isCmd("GO1"))   executeGo1(aCmd);
   if (aCmd->isCmd("GO2"))   executeGo2(aCmd);
   if (aCmd->isCmd("Parms")) executeParms(aCmd);

   if (aCmd->isCmd("RUN1"))  executeRun1(aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeRun1(Ris::CmdLineCmd* aCmd)
{
   gParms.reset();
   gParms.readSection("default");
   gParms.readSection("run1");

   MyClass tMyClass(gParms.mMyClassParms);
   tMyClass.doRun1();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   gParms.reset();
   gParms.readSection("default");
   gParms.readSection("run1");
   gParms.show();
}

//******************************************************************************

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeParms(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,"Run1");
   gParms.reset();
   gParms.readSection("default");
   gParms.readSection(aCmd->argString(1));
   gParms.show();
}


