
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "prnPrint.h"

#include "exampleSettings.h"
#include "exampleTMessage.h"
#include "exampleTMessageHelper.h"
#include "exampleNetworkThread.h"

#include "CmdLineExec.h"

using namespace Example;

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
   int tMsgType= aCmd->argInt(1);

   switch (tMsgType)
   {
      case 1:
      {
         TestMsg* tMsg = new TestMsg;
         TMessageHelper::initialize(tMsg);
         gNetworkThread->sendMessage(tMsg);
         break;
      }
      case 2:
      {
         StatusMsg* tMsg = new StatusMsg;
         TMessageHelper::initialize(tMsg);
         gNetworkThread->sendMessage(tMsg);
         break;
      }
      case 3:
      {
         Data1Msg* tMsg = new Data1Msg;
         TMessageHelper::initialize(tMsg);
         gNetworkThread->sendMessage(tMsg);
         break;
      }
      case 4:
      {
         Data2Msg* tMsg = new Data2Msg;
         TMessageHelper::initialize(tMsg);
         gNetworkThread->sendMessage(tMsg);
         break;
      }
      case 5:
      {
         Data3Msg* tMsg = new Data3Msg;
         TMessageHelper::initialize(tMsg);
         gNetworkThread->sendMessage(tMsg);
         break;
      }
      case 6:
      {
         Data4Msg* tMsg = new Data4Msg;
         TMessageHelper::initialize(tMsg);
         gNetworkThread->sendMessage(tMsg);
         break;
      }
   }
}

//******************************************************************************

void CmdLineExec::executeOnGo1 (Ris::CmdLineCmd* aCmd)
{
   gNetworkThread->sendTestMsg();
}

//******************************************************************************

void CmdLineExec::executeOnGo2(Ris::CmdLineCmd* aCmd)
{
   StatusMsg* tMsg = new StatusMsg;
   TMessageHelper::initialize(tMsg);
   gNetworkThread->sendMessage(tMsg);
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
