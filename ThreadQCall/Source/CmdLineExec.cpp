
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "prnPrint.h"

#include "CmdLineExec.h"

#include "risPortableCalls.h"

#include "someShare.h"
#include "someQCallThread1.h"
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
   if(aCmd->isCmd("GO1"   ))  executeGo1     (aCmd);
   if(aCmd->isCmd("GO2"   ))  executeGo2     (aCmd);
   if(aCmd->isCmd("GO3"   ))  executeGo3     (aCmd);
   if(aCmd->isCmd("F101"  ))  executeF101    (aCmd);
   if(aCmd->isCmd("F102"  ))  executeF102    (aCmd);
}

//******************************************************************************

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
   Prn::print(0,"%d", sizeof(gQCallThread1->mC102QCall));
}

//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   gQCallThread1->mC102QCall(1001);
}

//******************************************************************************

void CmdLineExec::executeGo3(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************

void CmdLineExec::executeF101(Ris::CmdLineCmd* aCmd)
{
   gQCallThread1->mF100QCall();
   gQCallThread1->mF101QCall(101);
   gQCallThread1->mF102QCall(102,202.2);
   gQCallThread1->mF103QCall(103,203.3,303.3f);
   gQCallThread1->mF104QCall(104,204.4,304.4f,0x44FFFFFF01234567);
   gQCallThread1->mF105QCall(105,205.5,305.5f,0x45FFFFFF01234567,505.5);
}

//******************************************************************************

void CmdLineExec::executeF102(Ris::CmdLineCmd* aCmd)
{
   int           tX1 = 105;
   double        tX2 = 205.5;
   float         tX3 = 305.5;
   long long int tX4 = 0x45FFFFFF01234567;
   double        tX5 = 505.5;

   gQCallThread1->mF105QCall(tX1,tX2,tX3,tX4,tX5);
}

