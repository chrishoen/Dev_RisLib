#include "stdafx.h"

#include "risFileFunctions.h"


#include "CmdLineExec.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************


CmdLineExec::CmdLineExec()
{
   mLockFile = 0;
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

   if (aCmd->isCmd("R"))    executeRead(aCmd);
   if (aCmd->isCmd("W"))    executeWrite(aCmd);
   if (aCmd->isCmd("Lock"))    executeLock(aCmd);
   if (aCmd->isCmd("Unlock"))  executeUnlock(aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeRead(Ris::CmdLineCmd* aCmd)
{
   mSettings.doRead();
   mSettings.show();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeWrite(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, 101);
   mSettings.mInput = aCmd->argFloat(1);
   mSettings.doWrite();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeLock(Ris::CmdLineCmd* aCmd)
{
   mLockFile = Ris::doLockFile_OpenAndLockForWrite("mylockfile101");
   Prn::print(0, "done");
}

void CmdLineExec::executeUnlock(Ris::CmdLineCmd* aCmd)
{
   Ris::doLockFile_UnlockAndClose(mLockFile);
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

