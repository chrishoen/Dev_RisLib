#include "stdafx.h"

#include <windows.h>
#include <time.h>

#include "risContainers.h"
#include "risTimeMarker.h"
#include "risProgramTime.h"

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
   if(aCmd->isCmd("GO1"    ))  executeGo1(aCmd);
   if(aCmd->isCmd("GO2"    ))  executeGo2(aCmd);
   if(aCmd->isCmd("GO3"    ))  executeGo3(aCmd);
   if(aCmd->isCmd("GO4"    ))  executeGo4(aCmd);
   if(aCmd->isCmd("GO5"    ))  executeGo5(aCmd);
}

//******************************************************************************

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
   time_t tTimet;
   struct tm* tTm;

   time(&tTimet);
   tTm = localtime(&tTimet);

   Prn::print(0, "time1 %s", asctime(tTm));
   Prn::print(0, "time1 %lld", tTimet);
}

//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,1000);

   Ris::TrialTimeMarker tMarker;
   tMarker.doStart();
   Sleep(aCmd->argInt(1));
   tMarker.doStop();
   Prn::print(0, "tMarker.mTimeDifferenceUS %8.2f", tMarker.mTimeDifferenceUS);

}

//******************************************************************************

void CmdLineExec::executeGo3(Ris::CmdLineCmd* aCmd)
{
   Ris::TrialTimeMarker tMarker;
   tMarker.doStart();

   time_t tTimet;
   struct tm* tTm;

   time(&tTimet);
   tTm = localtime(&tTimet);

   tMarker.doStop();

   Prn::print(0, "time1 %s", asctime(tTm));
   Prn::print(0, "time1 %lld", tTimet);
   Prn::print(0, "tMarker.mTimeDifferenceUS %8.2f", tMarker.mTimeDifferenceUS);
}

//******************************************************************************

void CmdLineExec::executeGo4(Ris::CmdLineCmd* aCmd)
{
   Prn::print(0, "ProgramTime %8.4f", Ris::getCurrentProgramTime());
   time_t rawtime;
   struct tm * ptm;

   time ( &rawtime );

   ptm = gmtime ( &rawtime );

}

//******************************************************************************

void CmdLineExec::executeGo5(Ris::CmdLineCmd* aCmd)
{
   Ris::ProgramTimeMarker tMarker;
   tMarker.doStart();
   Sleep(1000);
   tMarker.doStop();
   Prn::print(0, "ProgramTime %8.4f", tMarker.mElapsed);
   
}

