#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "prnPrint.h"
#include "my_functions.h"
#include "risCmdLineConsole.h"
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
//******************************************************************************
//******************************************************************************

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
   time_t     now = time(0);
   struct tm  tstruct;
   char       buf[80];
   tstruct = *localtime(&now);
   strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

   printf ( "Current local time and date: %s\n", buf );


   char tBuffer[100];
   printf ( "Current local time and date: %s\n", my_timestamp(tBuffer));
}

//******************************************************************************

void CmdLineExec::executeGo3(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,10);
   int tN = aCmd->argInt(1);

   for (int i = 0; i < tN; i++)
   {
      Prn::print(0, "%d",i);
   }

   Prn::print(0, "");

   for (int i = tN-1; i >= 0; i--)
   {
      Prn::print(0, "%d",i);
   }
}

//******************************************************************************

void CmdLineExec::executeGo4(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************

void CmdLineExec::executeGo5(Ris::CmdLineCmd* aCmd)
{
}

