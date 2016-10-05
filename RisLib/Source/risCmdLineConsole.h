#ifndef _RISCMDLINECONSOLE_H_
#define _RISCMDLINECONSOLE_H_
/*==============================================================================

Console command line processor

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risCmdLineExec.h"

//******************************************************************************
namespace Ris
{

//******************************************************************************
// This enters a loop that reads console input and applies a command line 
// executive to each line that is entered by a user. The loop terminates when
// the user exits.

class CmdLineConsole
{
public:
   // Execute command line executive from console input.
   // Returns when user exits.
   void execute (BaseCmdLineExec* aExec);
};

// Helpers:

void  executeCmdLineConsole (BaseCmdLineExec* aExec);
void  executeCmdLineConsoleThread (BaseCmdLineExec* aExec);
void  abortCmdLineConsole ();
void  setConsoleTitle(char* aTitle);

//******************************************************************************
#ifdef _RISCMDLINECONSOLE_CPP_
       CmdLineConsole  gCmdLineConsole;
#else
       extern CmdLineConsole  gCmdLineConsole;
#endif

//******************************************************************************
}//namespace
#endif

