#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "my_functions.h"
#include "prnPrint.h"

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
   aCmd->setArgDefaultUnsigned(1,0xffffffff);
   aCmd->setArgDefaultUnsigned(1,4294967295);

   unsigned int tN = aCmd->argUnsigned(1);

   Prn::print(0,0,"%08X",tN);
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
   aCmd->setArgDefault(1,"abcdefg");

   Prn::print(0,0,"%s %s",
      my_string_from_bool(aCmd->isArgString(1, "abcdefg")),
      aCmd->argString(1));
}

//******************************************************************************

void CmdLineExec::executeGo4(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,0);
   aCmd->setArgDefault(2,1);

   int tIndex = aCmd->argInt(1);
   int tDelta = aCmd->argInt(2);
   int tSize  = 5;

   Prn::print(0,0,"");
   Prn::print(0,0,"index %d",tIndex);
   Prn::print(0,0,"delta %d",tDelta);
   Prn::print(0,0,"");

   tIndex = my_index_add(tIndex,tDelta,tSize);

   Prn::print(0,0,"index %d",tIndex);
   Prn::print(0,0,"");
}

//******************************************************************************

void CmdLineExec::executeGo5(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,0);
   aCmd->setArgDefault(2,1);

   int tIndex = aCmd->argInt(1);
   int tDelta = aCmd->argInt(2);
   int tSize  = 5;

   Prn::print(0,0,"");
   Prn::print(0,0,"index %d",tIndex);
   Prn::print(0,0,"delta %d",tDelta);
   Prn::print(0,0,"");

   tIndex = my_index_sub(tIndex,tDelta,tSize);

   Prn::print(0,0,"index %d",tIndex);
   Prn::print(0,0,"");
}

