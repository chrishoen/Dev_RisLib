
#include "stdafx.h"
#ifdef WIN32
#include <windows.h>
#endif

#include "procoSerialParms.h"
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
   if (aCmd->isCmd("A"))         executeAbort(aCmd);
   if (aCmd->isCmd("C"))         executeClose(aCmd);
   if (aCmd->isCmd("G"))         executeGo1(aCmd);
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
   char tString[100];
   if (aCmd->numArg() == 0)
   {
      strcpy(tString, "ABCD");
   }
   else
   {
      sprintf(tString, "%s", aCmd->argWhole());
   }
   gProcThread->sendString(new std::string(tString));
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeAbort(Ris::CmdLineCmd* aCmd)
{
   gProcThread->mSerialStringThread->mSerialStringPort.doAbort();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeClose(Ris::CmdLineCmd* aCmd)
{
   gProcThread->mSerialStringThread->mSerialStringPort.doClose();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
#ifdef WIN32
class SerialPortSpecific
{
public:
   HANDLE mPortHandle;
   HANDLE mRxEventHandle;
   HANDLE mTxEventHandle;
};

void CmdLineExec::executeGo1 (Ris::CmdLineCmd* aCmd)
{
   DWORD tModemStat = 0;
   SerialPortSpecific* tSpecific = (SerialPortSpecific*)gProcThread->mSerialStringThread->mSerialStringPort.mSpecific;
   if (!GetCommModemStatus(tSpecific->mPortHandle, &tModemStat))
   {
      Prn::print(0, "GetCommModemStatus FAIL");
      return;
   }
   Prn::print(0, "GetCommModemStatus %x", tModemStat);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   SerialPortSpecific* tSpecific = (SerialPortSpecific*)gProcThread->mSerialStringThread->mSerialStringPort.mSpecific;
   ClearCommError(tSpecific->mPortHandle, 0, 0);
   Prn::print(0, "ClearCommError");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
#else
void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
}
void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
}
#endif

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
   ProtoComm::gSerialParms.show();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

