#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "prnPrint.h"
#include "my_functions.h"

#include "risByteBuffer.h"

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
   aCmd->setArgDefault(1,1001);
   int tCount1 = aCmd->argInt(1);
   int tCount2 = 0;

   Ris::ByteBuffer tBuffer(1000);

   tBuffer.setCopyTo();
   tBuffer.copy ( &tCount1 );
   Prn::print(0,"Buffer.Length %d",tBuffer.mWorkingLength);

   tBuffer.rewind();
   tBuffer.setCopyFrom();

   tCount1=0;
   tBuffer.copy ( &tCount1 );
   Prn::print(0,"Count1        %d",tCount1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,false);
   bool tFlag1 = aCmd->argBool(1);

   Ris::ByteBuffer tBuffer(1000);

   tBuffer.setCopyTo();
   tBuffer.copy ( &tFlag1 );
   Prn::print(0,"Flag1          %5s",my_string_from_bool(tFlag1));
   Prn::print(0,"Buffer.Length  %5d",tBuffer.mWorkingLength);

   tBuffer.rewind();
   tBuffer.setCopyFrom();

   tFlag1 = false;
   tBuffer.copy ( &tFlag1 );
   Prn::print(0,"Flag1          %5s",my_string_from_bool(tFlag1));
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

