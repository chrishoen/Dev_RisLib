
#include "stdafx.h"

#include "CmdLineExec.h"

#include "risNetPortDef.h"
#include "risNetUdpStringSocket.h"
#include "risNetSettings.h"

#include "someTimerThread.h"

#include "prnPrint.h"
#include "prnPrintSettings.h"

using namespace Some;

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

void CmdLineExec::execute(Ris::CmdLineCmd* aCmd)
{
   if(aCmd->isCmd("TP"    ))  gTimerThread->mTPFlag = aCmd->argBool(1);
   if(aCmd->isCmd("GO1"   ))  executeGo1     (aCmd);
   if(aCmd->isCmd("GO2"   ))  executeGo2     (aCmd);
   if(aCmd->isCmd("GO3"   ))  executeGo3     (aCmd);
   if(aCmd->isCmd("GO4"   ))  executeGo4     (aCmd);
   if(aCmd->isCmd("GO5"   ))  executeGo5     (aCmd);
   if(aCmd->isCmd("PARMS" ))  executeParms   (aCmd);
   if (aCmd->isCmd("T"))         Prn::toggleSuppressPrint();
   if (aCmd->isCmd("S"))         Prn::suppressPrint();
   if (aCmd->isCmd("U"))         Prn::unsuppressPrint();

}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
   Prn::print(0,"Go1 0");
   Prn::print(Prn::View01, "Go1 Prn::View01");
   Prn::print(Prn::View11, "Go1 Prn::View11");
   Prn::print(Prn::View21, "Go1 Prn::View21");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   Prn::print(0,"Go2");
   Log::write(0,"Go2");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo3(Ris::CmdLineCmd* aCmd)
{
   Prn::print(Prn::ViewRun3,"Go3 Prn::ViewRun3");
   Prn::print(Prn::ViewRun4,"Go3 Prn::ViewRun4");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo4(Ris::CmdLineCmd* aCmd)
{
   Prn::print(0,"Go4 ZERO");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo5(Ris::CmdLineCmd* aCmd)
{
   Prn::print(Prn::View11, "Go5 Print1");
   Prn::print(Prn::View11, "");
   Prn::print(Prn::View11, "Go5 Print2");
   Prn::print(Prn::View11, "");
   Prn::print(Prn::View11, "");
   Prn::print(Prn::View11, "Go5 Print3");
   Prn::print(Prn::View11, "");
   Prn::print(Prn::View11, "");
   Prn::print(Prn::View11, "");
   Prn::print(Prn::View11, "Go5 Print4");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeParms(Ris::CmdLineCmd* aCmd)
{
   Prn::gPrintSettings.reset();
   Prn::gPrintSettings.readSection("default");
   Prn::gPrintSettings.show();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************


