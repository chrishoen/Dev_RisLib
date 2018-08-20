#include "stdafx.h"

#include <stdlib.h>
#include <stdio.h>

#include "risPortableCalls.h"
#include "prnPrint.h"

#include "someTimerThread.h"

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
   if(aCmd->isCmd("RESET"  ))  reset();
   if(aCmd->isCmd("T"      ))  Some::gTimerThread->mTPFlag = !Some::gTimerThread->mTPFlag;
   if(aCmd->isCmd("GO1"    ))  executeGo1(aCmd);
   if(aCmd->isCmd("GO2"    ))  executeGo2(aCmd);
   if(aCmd->isCmd("GO3"    ))  executeGo3(aCmd);
   if(aCmd->isCmd("GO4"    ))  executeGo4(aCmd);
   if(aCmd->isCmd("GO5"    ))  executeGo5(aCmd);
   if(aCmd->isCmd("GO6"    ))  executeGo6(aCmd);
   if(aCmd->isCmd("GO7"    ))  executeGo7(aCmd);
   if(aCmd->isCmd("ABORT"  ))  printf("ABORT*********************************\n");
}

//******************************************************************************

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,101);
   aCmd->setArgDefault(2,202.2);


   int    tI = aCmd->argInt(1);
   double tD = aCmd->argDouble(2);

   printf("ArgNum %d\n",aCmd->mArgNum);
   printf("Args   %d   %f\n",tI,tD);
}

//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   Prn::print(0,"GO2GO2GO2GO2");
   return;

   int    tCount = 0;
   while(!Ris::portableKbhit())
   {
      Prn::print(0,"count   %d",tCount++);
      Ris::portableSleep(500);
   }
}

//******************************************************************************

void CmdLineExec::executeGo3(Ris::CmdLineCmd* aCmd)
{
   Prn::print(0,"PRINT1");
   Prn::print(0,"PRINT2");

   Prn::print(Prn::ProcRun1,"Prn::ProcRun1");
   Prn::print(Prn::ProcRun2,"Prn::ProcRun2");
   Prn::print(Prn::ProcRun3,"Prn::ProcRun3");
   Prn::print(Prn::ProcRun4,"Prn::ProcRun4");
}

//******************************************************************************

void CmdLineExec::executeGo4(Ris::CmdLineCmd* aCmd)
{
   printf("GetCurrentWorkingDir  %s\n",Ris::portableGetCurrentDir());
}

//******************************************************************************

void CmdLineExec::executeGo5(Ris::CmdLineCmd* aCmd)
{
}

void CmdLineExec::executeGo6(Ris::CmdLineCmd* aCmd)
{
}

void CmdLineExec::executeGo7(Ris::CmdLineCmd* aCmd)
{
}



