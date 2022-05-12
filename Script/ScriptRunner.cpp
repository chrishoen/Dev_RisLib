#include "stdafx.h"

#include "ScriptRunner.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

ScriptRunner::ScriptRunner()
{
}

void ScriptRunner::reset()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Run a loop that reads command lines from a script file and 
// executes them one by one.

void ScriptRunner::doRun()
{
   if (!mScript.doOpen("files/script_rgb.txt"))
   {
      printf("ScriptRunner file not found\n");
   }

   Ris::CmdLineCmd* tCmd = new Ris::CmdLineCmd;
   while (true)
   {
      if (mScript.doRead(tCmd))
      {
         if (tCmd->isCmd("EXIT")) break;
         execute(tCmd);
      }
   }

   delete tCmd;
   mScript.doClose();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute a command line command. It calls one of
// the following specific command execution functions.

void ScriptRunner::execute(Ris::CmdLineCmd* aCmd)
{
   if (aCmd->isCmd("RESET"))  reset();
   if (aCmd->isCmd("RED"))    executeRed(aCmd);
   if (aCmd->isCmd("GREEN"))  executeGreen(aCmd);
   if (aCmd->isCmd("BLUE"))   executeBlue(aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void ScriptRunner::executeRed(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, 111);
   int tN = aCmd->argInt(1);
   Prn::print(0, "RED     %d", tN);
}

void ScriptRunner::executeGreen(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, 111);
   int tN = aCmd->argInt(1);
   Prn::print(0, "GREEN   %d", tN);
}

void ScriptRunner::executeBlue(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, 111);
   int tN = aCmd->argInt(1);
   Prn::print(0, "BLUE    %d", tN);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
