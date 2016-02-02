
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "prnPrint.h"
#include "my_functions.h"
#include "risStatistics.h"

#include "CmdLineExec.h"

//******************************************************************************
CmdLineExec::CmdLineExec()
{
   mStepFlag=true;
}
//******************************************************************************
void CmdLineExec::reset()
{
}
//******************************************************************************
void CmdLineExec::execute(Ris::CmdLineCmd* aCmd)
{
   if(aCmd->isCmd("RESET"  ))  reset();
   if(aCmd->isCmd("GO1"    ))  executeGo1  (aCmd);
   if(aCmd->isCmd("GO2"    ))  executeGo2  (aCmd);
   if(aCmd->isCmd("GO3"    ))  executeGo3  (aCmd);
   if(aCmd->isCmd("GO4"    ))  executeGo4  (aCmd);
   if(aCmd->isCmd("GO5"    ))  executeGo5  (aCmd);
   if(aCmd->isCmd("GO6"    ))  executeGo6  (aCmd);
   if(aCmd->isCmd("GO7"    ))  executeGo7  (aCmd);
   if(aCmd->isCmd("GO8"    ))  executeGo8  (aCmd);
   if(aCmd->isCmd("GO9"    ))  executeGo9  (aCmd);
   if(aCmd->isCmd("GO10"   ))  executeGo10 (aCmd);
   if(aCmd->isCmd("GO11"   ))  executeGo11 (aCmd);
   if(aCmd->isCmd("GO12"   ))  executeGo12 (aCmd);
   if(aCmd->isCmd("GO13"   ))  executeGo13 (aCmd);

   if(aCmd->isCmd("STEP"   ))  mStepFlag=true;
   if(aCmd->isCmd("RAMP"   ))  mStepFlag=false;
}

//******************************************************************************

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
   Ris::TrialStatistics tF;
   tF.startTrial(50.0);

   for (int i = 0; i<20; i++)
   {
      tF.put(10.0 + 2.0);
   }
   for (int i = 0; i<20; i++)
   {
      tF.put(10.0 - 2.0);
   }

   tF.finishTrial();
   tF.show();
   Prn::print(0,"");
}

//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   Ris::TrialStatistics tF;
   tF.startTrial(50.0);

   for (int i = 0; i<20; i++)
   {
      tF.put(10.0 + 2.0);
   }
   for (int i = 0; i<20; i++)
   {
      tF.put(10.0 - 2.0);
   }
   for (int i = 0; i<20; i++)
   {
      tF.put(60.0);
   }

   tF.finishTrial();
   tF.show();
   Prn::print(0,"");


}

//******************************************************************************

void CmdLineExec::executeGo3(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************

void CmdLineExec::executeGo4(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************

void CmdLineExec::executeGo5(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************

void CmdLineExec::executeGo6(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************

void CmdLineExec::executeGo7(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************

void CmdLineExec::executeGo8(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************

void CmdLineExec::executeGo9(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************

void CmdLineExec::executeGo10(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************

void CmdLineExec::executeGo11(Ris::CmdLineCmd* aCmd)
{
   Ris::PeriodicStatistics tF;
   tF.initialize(40);

   for (int i = 0; i<20; i++)
   {
      tF.put(1.0);
      tF.show();
   }
   Prn::print(0,"");

   for (int i = 0; i<20; i++)
   {
      tF.put(2.0);
      tF.show();
   }
   Prn::print(0,"");

   for (int i = 0; i<20; i++)
   {
      tF.put(1.0);
      tF.show();
   }
   Prn::print(0,"");

   for (int i = 0; i<20; i++)
   {
      tF.put(2.0);
      tF.show();
   }
   Prn::print(0,"");

   for (int i = 0; i<20; i++)
   {
      tF.put(1.0);
      tF.show();
   }
   Prn::print(0,"");

   for (int i = 0; i<20; i++)
   {
      tF.put(2.0);
      tF.show();
   }
   Prn::print(0,"");
}

//******************************************************************************

void CmdLineExec::executeGo12(Ris::CmdLineCmd* aCmd)
{
   Ris::PeriodicStatistics tF;
   tF.initialize(40);

   for (int i = 0; i<20; i++)
   {
      tF.put(1.0);
      tF.show();
   }
   Prn::print(0,"");

   for (int i = 0; i<20; i++)
   {
      tF.put(2.0);
      tF.show();
   }
   Prn::print(0,"");

   for (int i = 0; i<20; i++)
   {
      tF.put(1.0);
      tF.show();
   }
   Prn::print(0,"");

   for (int i = 0; i<20; i++)
   {
      tF.put(2.0);
      tF.show();
   }
   Prn::print(0,"");

   for (int i = 0; i<20; i++)
   {
      tF.put(1.0);
      tF.show();
   }
   Prn::print(0,"");

   for (int i = 0; i<20; i++)
   {
      tF.put(2.0);
      tF.show();
   }
   Prn::print(0,"");
}

//******************************************************************************

void CmdLineExec::executeGo13(Ris::CmdLineCmd* aCmd)
{
   Ris::PeriodicStatistics tF;
   tF.initialize(40);

   for (int i = 0; i<20; i++)
   {
      tF.put(2.0 + 10.0);
      tF.show();
   }
   Prn::print(0,"");

   for (int i = 0; i<20; i++)
   {
      tF.put(-2.0 + 10.0);
      tF.show();
   }
   Prn::print(0,"");


}

