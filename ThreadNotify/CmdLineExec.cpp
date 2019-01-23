
#include "stdafx.h"

#include "someTestQCallThread.h"
#include "someThreadParms.h"

#include "risCmdLineConsole.h"
#include "CmdLineExec.h"

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
// This class is the program command line executive. It processes user
// command line inputs and executes them. It inherits from the command line
// command executive base class, which provides an interface for executing
// command line commands. It provides an override execute function that is
// called by a console executive when it receives a console command line input.
// The execute function then executes the command.

void CmdLineExec::execute(Ris::CmdLineCmd* aCmd)
{
   if (aCmd->isCmd("TEST1"))     executeTest1(aCmd);
   if (aCmd->isCmd("TEST2"))     executeTest2(aCmd);
   if (aCmd->isCmd("TEST3"))     executeTest3(aCmd);
   if (aCmd->isCmd("TEST4"))     executeTest4(aCmd);

   if (aCmd->isCmd("N"))         executeNotify(aCmd);
   if (aCmd->isCmd("Error"))     executeError(aCmd);
   if (aCmd->isCmd("A"))         executeAbort(aCmd);

   if (aCmd->isCmd("GO1"))       executeGo1(aCmd);
   if (aCmd->isCmd("GO2"))       executeGo2(aCmd);
   if (aCmd->isCmd("GO3"))       executeGo3(aCmd);
   if (aCmd->isCmd("GO4"))       executeGo4(aCmd);
   if (aCmd->isCmd("GO5"))       executeGo5(aCmd);
   if (aCmd->isCmd("GO5"))       executeGo5(aCmd);
   if (aCmd->isCmd("Parms"))     executeParms(aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeTest1(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, -1);

   Some::gTestQCallThread->mTest1QCall(aCmd->argInt(1));
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeTest2(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, -1);

   Some::gTestQCallThread->mTest2QCall(aCmd->argInt(1));
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeTest3(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, -1);

   Some::gTestQCallThread->mTest3QCall(aCmd->argInt(1));
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeTest4(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, -1);

   Some::gTestQCallThread->mTest4QCall(aCmd->argInt(1));
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeNotify(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, 1);
   Some::gTestQCallThread->mNotify.notify(aCmd->argInt(1));
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeError(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, 1);
   aCmd->setArgDefault(2, 101);
   Some::gTestQCallThread->mNotify.notifyError(aCmd->argInt(1), aCmd->argInt(2));
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeAbort(Ris::CmdLineCmd* aCmd)
{
   Some::gTestQCallThread->mNotify.abort();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, 1);
   Some::gTestQCallThread->mNotify.notify(aCmd->argInt(1));
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   Some::gTestQCallThread->mNotify.notify(1);
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

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeParms(Ris::CmdLineCmd* aCmd)
{
   Some::gThreadParms.reset();
   Some::gThreadParms.readSection("default");
   Some::gThreadParms.show();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

