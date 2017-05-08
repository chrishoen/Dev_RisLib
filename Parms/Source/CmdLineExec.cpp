#include <windows.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <random>

#include "prnPrint.h"
#include "risPortableCalls.h"
#include "CmdLineExec.h"
#include "Parms.h"
#include "someMyClass.h"

using namespace Some;

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
   if (aCmd->isCmd("RESET")) reset();
   if (aCmd->isCmd("GO1"))   executeGo1(aCmd);
   if (aCmd->isCmd("GO2"))   executeGo2(aCmd);
   if (aCmd->isCmd("Parms")) executeParms(aCmd);

   if (aCmd->isCmd("RUN1"))  executeRun1(aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeRun1(Ris::CmdLineCmd* aCmd)
{
   gParms.reset();
   gParms.readSection("default");
   gParms.readSection("run1");

   MyClass tMyClass(gParms.mMyClassParms);
   tMyClass.doRun1();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
   char tCurrentDir[400];
   GetCurrentDirectory(400,tCurrentDir);

    Prn::print(0,"GetCurrentDir     %s",tCurrentDir);

    Prn::print(0,"RIS GetCurrentDir %s",Ris::portableGetCurrentDir());
    Prn::print(0,"RIS GetProgramDir %s",Ris::portableGetProgramDir());
}

//******************************************************************************

void my_local_function()
{
   printf("my_local_function\n");
}

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   char path[400];
   HMODULE hm = NULL;

   if (!GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | 
            GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            (LPCSTR) &my_local_function, 
            &hm))
   {
         int ret = GetLastError();
         fprintf(stderr, "GetModuleHandle returned %d\n", ret);
   }
   GetModuleFileNameA(hm, path, sizeof(path));

   Prn::print(0,"GetModuleFileNameA %s",path);
}

//******************************************************************************

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeParms(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,"Run1");
   gParms.reset();
   gParms.readSection("default");
   gParms.readSection(aCmd->argString(1));
   gParms.show();
}


