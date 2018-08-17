
#include "stdafx.h"

#include "CmdLineExec.h"

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

   if(aCmd->isCmd("Open"  ))  executeOpen     (aCmd);
   if(aCmd->isCmd("Close" ))  executeClose    (aCmd);
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

void CmdLineExec::executeOpen(Ris::CmdLineCmd* aCmd)
{
   mPort.doOpen(7,"38400,N,8,1",2000);
}

//******************************************************************************

void CmdLineExec::executeClose(Ris::CmdLineCmd* aCmd)
{
   mPort.doClose();
}

