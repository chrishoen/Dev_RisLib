#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "my_functions.h"
#include "prnPrint.h"

#include "dhandle.h"
#include "exampleDClass.h"

#include "CmdLineExec.h"

using namespace Example;

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
   if(aCmd->isCmd("RESET"  ))  reset();
   if(aCmd->isCmd("GO1"    ))  executeGo1(aCmd);
   if(aCmd->isCmd("GO2"    ))  executeGo2(aCmd);
   if(aCmd->isCmd("GO3"    ))  executeGo3(aCmd);
   if(aCmd->isCmd("GO4"    ))  executeGo4(aCmd);
   if(aCmd->isCmd("GO5"    ))  executeGo5(aCmd);
}

//******************************************************************************

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************

void CmdLineExec::executeGo3(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************

void CmdLineExec::executeGo4(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************

void myFunction1(DHandle aClass)
{
}

void CmdLineExec::executeGo5(Ris::CmdLineCmd* aCmd)
{
   Test* tTest1 = new Test;
   myFunction1 (DHMake::handle(tTest1));
   myFunction1(DHandle(TypeIdT::cTest,tTest1));

   DHandle tHandle1(TypeIdT::cTest,tTest1);
   DHandle tHandle2(DHMake::handle(tTest1));


   Test* tTest2 = (Test*)tHandle1.mPtr;
   Test* tTest3 = DHMake::ptr_Test(tHandle1);
}

