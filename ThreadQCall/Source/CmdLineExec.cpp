
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "prnPrint.h"

#include "CmdLineExec.h"

#include "risPortableCalls.h"

#include "someShare.h"
#include "someQCallThread1.h"
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
   if(aCmd->isCmd("GO1"   ))  executeGo1     (aCmd);
   if(aCmd->isCmd("GO2"   ))  executeGo2     (aCmd);
   if(aCmd->isCmd("GO3"   ))  executeGo3     (aCmd);
   if(aCmd->isCmd("F101"  ))  executeF101    (aCmd);
   if(aCmd->isCmd("F102"  ))  executeF102    (aCmd);
}

//******************************************************************************

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
   Prn::print(0,"%d", sizeof(gQCallThread1->mC102QCall));
}

//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   gQCallThread1->mC102QCall(1001);
}

//******************************************************************************

void CmdLineExec::executeGo3(Ris::CmdLineCmd* aCmd)
{
   gQCallThread1->mC102QCall.mExecuteCallPointer(1001);
   gQCallThread1->mC102QCall.mX1 = 1002;
   gQCallThread1->mC102QCall.execute();

   gQCallThread1->mC102QCall.mX1 = 1003;
   Ris::Threads::BaseQCall* tQCall = (Ris::Threads::BaseQCall*)&gQCallThread1->mC102QCall;
   tQCall->execute();

}

//******************************************************************************

void CmdLineExec::executeF101(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************

void CmdLineExec::executeF102(Ris::CmdLineCmd* aCmd)
{
}

