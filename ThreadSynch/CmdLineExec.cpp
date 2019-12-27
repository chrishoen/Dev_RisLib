#include "stdafx.h"



#include "risThreadsSynch.h"
#include "someThread1.h"
#include "someTimerThread.h"
#include "someShare.h"
#include "CmdLineExec.h"
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
   if (aCmd->isCmd("GO"))    executeGo(aCmd);
   if (aCmd->isCmd("F101"))  executeF101(aCmd);
   if (aCmd->isCmd("F102"))  executeF102(aCmd);
   if (aCmd->isCmd("NM"))    executeNMutex(aCmd);
}

//******************************************************************************

void CmdLineExec::executeGo(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,1);
   gShare.mTest = aCmd->argInt(1);
}

//******************************************************************************

void CmdLineExec::executeF101(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************

void CmdLineExec::executeF102(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************

void CmdLineExec::executeNMutex(Ris::CmdLineCmd* aCmd)
{
   Ris::Threads::NamedMutex tMutex("/mymutex");
   Prn::print(0, "mutext lock begin");
   tMutex.lock();
   Prn::print(0, "mutext lock end");
   Ris::Threads::threadSleep(10000);
   Prn::print(0, "mutext unlock begin");
   tMutex.unlock();
   Prn::print(0, "mutext unlock end");

}

