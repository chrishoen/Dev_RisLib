#include "stdafx.h"


#include "CmdLineExec.h"

#include "someThread1.h"
#include "someTimerThread.h"
#include "someShare.h"
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
   if(aCmd->isCmd("GO"    ))  executeGo      (aCmd);
   if(aCmd->isCmd("F101"  ))  executeF101    (aCmd);
   if(aCmd->isCmd("F102"  ))  executeF102    (aCmd);
}

//******************************************************************************

void CmdLineExec::executeGo(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,1);
   gShare.mTest = aCmd->argInt(1);
}

//******************************************************************************

void CmdLineExec::executeF101(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************

void CmdLineExec::executeF102(Ris::CmdLineCmd* aCmd)
{
}

