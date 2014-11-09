#ifndef _RISCMDLINECONSOLE_H_
#define _RISCMDLINECONSOLE_H_
/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risPortableTypes.h"
#include "risCmdLineCmd.h"

//******************************************************************************
namespace Ris
{

//******************************************************************************
// This executes a command line executive from console input.

class  CmdLineConsole
{
public:
   // Constructor
   CmdLineConsole();
  ~CmdLineConsole();

   // Execute command line executive from console input.
   // Returns when user exits
   void execute   (BaseCmdLineExec* aExec);
};

// Nicknames
void  executeCmdLineConsole (BaseCmdLineExec* aExec);
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

