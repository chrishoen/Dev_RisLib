#pragma once

/*==============================================================================
Program command line executive.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risCmdLineScript.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This class reads a command line script file and executes command line
// one by one. 

class ScriptRunner
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Command line script file.
   Ris::CmdLineScript mScript;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Infrastructure.

   ScriptRunner();
   void reset();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Run a loop that reads command lines from a script file and
   // executes them one by one.
   void doRun();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Execute a command line command. It calls one of
   // the following specific command execution functions.
   void execute(Ris::CmdLineCmd* aCmd);

   // Execute specific commands.
   void executeRed(Ris::CmdLineCmd* aCmd);
   void executeGreen(Ris::CmdLineCmd* aCmd);
   void executeBlue(Ris::CmdLineCmd* aCmd);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

