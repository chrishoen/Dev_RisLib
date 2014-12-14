#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "prnPrint.h"
#include "risContainers.h"

#include "risLUT.h"

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
   int tCount=0;
   while(true)
   {
      printf("BEACON %d\n",tCount++);
   }
}

//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,101.5);

   Prn::print(0,0,"%8.3f",aCmd->argDouble(1));
}

//******************************************************************************

void CmdLineExec::executeGo3(Ris::CmdLineCmd* aCmd)
{
   Prn::print(0,0,"ABCDEFGH");
}

//******************************************************************************

void CmdLineExec::executeGo4(Ris::CmdLineCmd* aCmd)
{
	char tBuffer[200];
	tBuffer[0] = 0;

    fgets(tBuffer, 200, stdin);
    //gets(tBuffer);
    int tBufferLength = strlen(tBuffer);
    if (tBuffer[tBufferLength-1] == 10) tBuffer[tBufferLength-1] = 0;
    printf("tBufferLast %d\n", (int)tBuffer[tBufferLength - 1]);
    printf("tBuffer %d $$ %s\n", strlen(tBuffer), tBuffer);

    for (int i = 0; i < (int)strlen(tBuffer); i++)
    {
        printf("tBufferChar %d  %d\n", i, (int)tBuffer[i]);
    }

}

//******************************************************************************

void CmdLineExec::executeGo5(Ris::CmdLineCmd* aCmd)
{
}

