
#include "stdafx.h"

#include "procoUdpParms.h"
#include "procoProcThread.h"
#include "procoMonitorThread.h"

#include "CmdLineExec.h"

using namespace ProtoComm;

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
   if (aCmd->isCmd("TP"))        ProtoComm::gProcThread->mTPFlag = aCmd->argBool(1);
   if (aCmd->isCmd("SEND"))      executeSend(aCmd);
   if (aCmd->isCmd("GO1"))       executeGo1(aCmd);
   if (aCmd->isCmd("GO2"))       executeGo2(aCmd);
   if (aCmd->isCmd("GO3"))       executeGo3(aCmd);
   if (aCmd->isCmd("GO4"))       executeGo4(aCmd);
   if (aCmd->isCmd("Parms"))     executeParms(aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::special(int aSpecial)
{
   ProtoComm::gMonitorThread->mShowCode = aSpecial;

   if (aSpecial == 0)
   {
      Prn::setFilter(Prn::Show1, false);
      Prn::setFilter(Prn::Show2, false);
   }
   else if (aSpecial == 1)
   {
      Prn::setFilter(Prn::Show1, true);
      Prn::setFilter(Prn::Show2, false);
   }
   else if (aSpecial == 2)
   {
      Prn::setFilter(Prn::Show1, false);
      Prn::setFilter(Prn::Show2, true);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeSend (Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, 1472);
   int tSize = aCmd->argInt(1);
   char* tData = new char[tSize];
   memset(tData, 2, tSize);
   tData[0] = 1;
   tData[tSize - 1] = 3;

   gProcThread->sendData(tData, tSize);
   Prn::print(0, "SEND %d", tSize);
   delete tData;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo1 (Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo3(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo4(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeParms(Ris::CmdLineCmd* aCmd)
{
   ProtoComm::gUdpParms.show();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeHelp(Ris::CmdLineCmd* aCmd)
{
    Prn::print(0, "help ***********************************");
    Prn::print(0, "");
    Prn::print(0, "special commands 0..9");
    Prn::print(0, "0          -- don't show monitor or receive prints");
    Prn::print(0, "1          -- show receive prints");
    Prn::print(0, "2          -- show monitor prints");
    Prn::print(0, "SEND       -- send a udp datagram");
    Prn::print(0, "TP 1/0     -- enable/disable sending periodic udp datagrams");
    Prn::print(0, "");
    Prn::print(0, "trc help   -- show trace help");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

