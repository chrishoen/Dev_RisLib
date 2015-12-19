
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "prnPrint.h"

#include "examcoSettings.h"
#include "examcoDefs.h"
#include "examcoRecord.h"
#include "examcoHelper.h"
#include "examcoNetworkThread.h"

#include "CmdLineExec.h"

using namespace ExampleComm;

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
   if (aCmd->isCmd("SHUTDOWN" ))  executeOnShutdown (aCmd);
   if (aCmd->isCmd("TX"       ))  executeOnTx  (aCmd);
   if (aCmd->isCmd("GO1"      ))  executeOnGo1 (aCmd);
   if (aCmd->isCmd("GO2"      ))  executeOnGo2 (aCmd);
   if (aCmd->isCmd("GO3"      ))  executeOnGo3 (aCmd);
   if (aCmd->isCmd("GO4"      ))  executeOnGo4 (aCmd);
   if (aCmd->isCmd("GO5"      ))  executeOnGo5 (aCmd);
   if (aCmd->isCmd("GO6"      ))  executeOnGo6 (aCmd);
}
//******************************************************************************

void CmdLineExec::executeOnShutdown (Ris::CmdLineCmd* aCmd)
{
   gNetworkThread->shutdownThread();
}

//******************************************************************************

void CmdLineExec::executeOnTx (Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,1);
   int tRecordType= aCmd->argInt(1);

   switch (tRecordType)
   {
      case 1:
      {
         TestRecord* tRecord = new TestRecord;
         Helper::initialize(tRecord);
         gNetworkThread->sendRecord(tRecord);
         break;
      }
      case 5:
      {
         Data1Record* tRecord = new Data1Record;
         Helper::initialize(tRecord);
         gNetworkThread->sendRecord(tRecord);
         break;
      }
   }
}

//******************************************************************************

void CmdLineExec::executeOnGo1 (Ris::CmdLineCmd* aCmd)
{
   gNetworkThread->sendTestRecord();
}

//******************************************************************************

void CmdLineExec::executeOnGo2(Ris::CmdLineCmd* aCmd)
{
   StatusRecord* tRecord = new StatusRecord;
   Helper::initialize(tRecord);
   gNetworkThread->sendRecord(tRecord);
}


//******************************************************************************

void CmdLineExec::executeOnGo3(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************

void CmdLineExec::executeOnGo4(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************

void CmdLineExec::executeOnGo5(Ris::CmdLineCmd* aCmd)
{
}


//******************************************************************************


void CmdLineExec::executeOnGo6(Ris::CmdLineCmd* aCmd)
{
}
