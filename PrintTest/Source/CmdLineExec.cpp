
#include "stdafx.h"

#include "CmdLineExec.h"

#include "risNetPortDef.h"
#include "risNetUdpStringSocket.h"

#include "someTimerThread.h"


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
   if(aCmd->isCmd("TP"    ))  executeTP      (aCmd);
   if(aCmd->isCmd("GO1"   ))  executeGo1     (aCmd);
   if(aCmd->isCmd("GO2"   ))  executeGo2     (aCmd);
   if(aCmd->isCmd("GO3"   ))  executeGo3     (aCmd);
   if(aCmd->isCmd("GO4"   ))  executeGo4     (aCmd);
   if(aCmd->isCmd("GO5"   ))  executeGo4     (aCmd);
}

//******************************************************************************

void CmdLineExec::executeTP(Ris::CmdLineCmd* aCmd)
{
   gTimerThread->mTPFlag=aCmd->argBool(1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
   Prn::print(0,"Go1 0");
   Prn::print(Prn::ProcRun1,"Go1 Prn::ProcRun1");
   Prn::print(Prn::ProcRun2,"Go1 Prn::ProcRun2");
   Prn::print(Prn::ProcRun3,"Go1 Prn::ProcRun3");
   Prn::print(Prn::ProcRun4,"Go1 Prn::ProcRun4");
}

//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   Prn::print(0,"Go2");
   Log::write(0,"Go2");
}

//******************************************************************************

void CmdLineExec::executeGo3(Ris::CmdLineCmd* aCmd)
{
   Prn::print(Prn::ViewRun3,"Go2 Prn::ViewRun3");
   Prn::print(Prn::ViewRun4,"Go2 Prn::ViewRun4");
}

//******************************************************************************

void CmdLineExec::executeGo4(Ris::CmdLineCmd* aCmd)
{
   Prn::print(0,"Go3 ZERO");
}

//******************************************************************************

void CmdLineExec::executeGo5(Ris::CmdLineCmd* aCmd)
{
   Ris::Net::UdpTxStringSocket tSocket;
   tSocket.configure(Ris::Net::PortDef::cPrintView);

   tSocket.doSendString("ABCDEFG");
   tSocket.doClose();

}



