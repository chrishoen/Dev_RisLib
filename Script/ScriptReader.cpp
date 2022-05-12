#include "stdafx.h"

#include "ScriptReader.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

ScriptReader::ScriptReader()
{
}

void ScriptReader::reset()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Run a loop that reads command lines from a script file and 
// executes them one by one.

void ScriptReader::doRun()
{

}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute a command line command. It calls one of
// the following specific command execution functions.

void ScriptReader::execute(Ris::CmdLineCmd* aCmd)
{
   if(aCmd->isCmd("RESET"))   reset();
   if (aCmd->isCmd("RED"))    executeRed(aCmd);
   if (aCmd->isCmd("GREEN"))  executeGreen(aCmd);
   if (aCmd->isCmd("BLACK"))  executeBlack(aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void ScriptReader::executeRed(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, 111);
   int tN = aCmd->argInt(1);
   Prn::print(0, "RED     %d", tN);
}

void ScriptReader::executeGreen(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, 111);
   int tN = aCmd->argInt(1);
   Prn::print(0, "GREEN   %d", tN);
}

void ScriptReader::executeBlack(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, 111);
   int tN = aCmd->argInt(1);
   Prn::print(0, "BLACK   %d", tN);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
