#include "stdafx.h"

#include "risProgramTime.h"


#include "smShare.h"
#include "CmdLineExec.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************


CmdLineExec::CmdLineExec()
 : mSem("/mynamedsem")
{
}

void CmdLineExec::reset()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This class is the program command line executive. It processes user
// command line inputs and executes them. It inherits from the command line
// command executive base class, which provides an interface for executing
// command line commands. It provides an override execute function that is
// called by a console executive when it receives a console command line input.
// The execute function then executes the command.

void CmdLineExec::execute(Ris::CmdLineCmd* aCmd)
{
   if (aCmd->isCmd("GO1"))  executeGo1(aCmd);
   if (aCmd->isCmd("GO2"))  executeGo2(aCmd);
   if (aCmd->isCmd("GO3"))  executeGo3(aCmd);
   if (aCmd->isCmd("GO4"))  executeGo4(aCmd);
   if (aCmd->isCmd("GO5"))  executeGo5(aCmd);

   if (aCmd->isCmd("Put"))     executePut(aCmd);
   if (aCmd->isCmd("get"))     executeGet(aCmd);

   if (aCmd->isCmd("SW"))      executeShareWrite(aCmd);
   if (aCmd->isCmd("SR"))      executeShareRead(aCmd);
   if (aCmd->isCmd("Show2"))   executeShow2(aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeShareWrite(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, "201");

   SM::gShare->mX1 = aCmd->argInt(1);
}

void CmdLineExec::executeShareRead(Ris::CmdLineCmd* aCmd)
{
   SM::gShare->show(0);
}

void CmdLineExec::executeShow2(Ris::CmdLineCmd* aCmd)
{
   SM::gShare->show2();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executePut(Ris::CmdLineCmd* aCmd)
{
   mSem.put();
   Prn::print(0, "done");
}

void CmdLineExec::executeGet(Ris::CmdLineCmd* aCmd)
{
   mSem.get();
   Prn::print(0, "done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo3(Ris::CmdLineCmd* aCmd)
{
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

