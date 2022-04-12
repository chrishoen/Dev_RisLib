#include "stdafx.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include "string.h"

#include "risProgramTime.h"
#include "risBitUtils.h"
#include "risSleep.h"
#include "my_functions.h"
#include "trcTraceBuffer.h"
#include "trcTrace.h"
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
   if(aCmd->isCmd("RESET"))   reset();
   if (aCmd->isCmd("START"))  executeStart(aCmd);
   if (aCmd->isCmd("STOP"))   executeStop(aCmd);
   if (aCmd->isCmd("GO1"))    executeGo1(aCmd);
   if (aCmd->isCmd("GO2"))    executeGo2(aCmd);
   if (aCmd->isCmd("GO3"))    executeGo3(aCmd);
   if (aCmd->isCmd("GO4"))    executeGo4(aCmd);
   if (aCmd->isCmd("GO5"))    executeGo5(aCmd);
   if (aCmd->isCmd("GO6"))    executeGo6(aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeStart(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, 1);
   Trc::start(aCmd->argInt(1));
}

void CmdLineExec::executeStop(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, 1);
   Trc::stop(aCmd->argInt(1));
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, 10);
   int tLoopSize = aCmd->argInt(1);
   for (int i = 0; i < tLoopSize; i++)
   {
      Trc::write(1, 0, "trace1 %d", i);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, 10);
   int tLoopSize = aCmd->argInt(1);
   for (int i = 0; i < tLoopSize; i++)
   {
      Trc::write(1, 0, "trace2 %d", i);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo3(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, 0);
   aCmd->setArgDefault(2, 10);
   int tTraceIndex = aCmd->argInt(1);
   int tLoopSize = aCmd->argInt(2);
   Trc::start(tTraceIndex);
   for (int i = 0; i < tLoopSize; i++)
   {
      Trc::write(tTraceIndex, 0, "trace2 %d", i);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo4(Ris::CmdLineCmd* aCmd)
{
   Prn::print(0, "portableGetBinDir     %s", Ris::portableGetBinDir());
   Prn::print(0, "portableGetCurrentDir %s", Ris::portableGetCurrentDir());
   Ris::portableChdirUpFromBin();
   Prn::print(0, "portableGetBinDir     %s", Ris::portableGetBinDir());
   Prn::print(0, "portableGetCurrentDir %s", Ris::portableGetCurrentDir());
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

void CmdLineExec::executeGo6(Ris::CmdLineCmd* aCmd)
{
}

