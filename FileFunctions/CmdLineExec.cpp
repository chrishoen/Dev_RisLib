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

   if (aCmd->isCmd("W1"))    executeWrite1(aCmd);
   if (aCmd->isCmd("R1"))    executeRead1(aCmd);

   if (aCmd->isCmd("B2"))    executeBegin2(aCmd);
   if (aCmd->isCmd("E2"))    executeEnd2(aCmd);
   if (aCmd->isCmd("R2"))    executeRead2(aCmd);

   if (aCmd->isCmd("Lock"))    executeLock(aCmd);
   if (aCmd->isCmd("Unlock"))  executeUnlock(aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeWrite1(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, 101);
   mSettings1.mInput = aCmd->argFloat(1);
   mSettings1.doWrite();
}

void CmdLineExec::executeRead1(Ris::CmdLineCmd* aCmd)
{
   mSettings1.doRead();
   mSettings1.show();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeBegin2(Ris::CmdLineCmd* aCmd)
{
   mSettings2.doReadModifyWriteBegin();
   Prn::print(0, "done");
}

void CmdLineExec::executeEnd2(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, 101);
   mSettings2.mInput = aCmd->argFloat(1);
   mSettings2.doReadModifyWriteEnd();
   Prn::print(0, "done");
}

void CmdLineExec::executeRead2(Ris::CmdLineCmd* aCmd)
{
   mSettings2.doRead();
   mSettings2.show();
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

