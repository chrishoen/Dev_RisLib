/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"


#include "risPortableCalls.h"
#include "risThreadsThreads.h"
#include "my_functions.h"
#include "prnPrint.h"
#include "trcTraceBuffer.h"
#include "risCmdLineReader.h"

#define  _RISCMDLINECONSOLE_CPP_
#include "risCmdLineConsole.h"


namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This enters a loop that reads console input and applies a command line 
// executive to each line that is entered by a user. The loop terminates when
// the user exits.

void CmdLineConsole::execute (BaseCmdLineExec* aExec)
{            
   printf("COMMAND LINE EXECUTIVE\n");
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize console input.

   // Reset the executive.
   aExec->reset();

   // Locals.
   char tCommandLine[200];
   CmdLineCmd  tCmd;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Loop for console input.

   while(true)
   {
      //************************************************************************
      //************************************************************************
      //************************************************************************
      // Read console input.

      if (gCmdLineReader.readLine(tCommandLine, 200) == 0)
      {
         return;
      }

      //************************************************************************
      //************************************************************************
      //************************************************************************
      // Test for specific commands.

      if (strlen(tCommandLine)==0)
      {
         // For an empty command line, which is only an enter, execute the
         //  given executive, callint special command with -1.
         aExec->special(-1);
      }

      //************************************************************************
      //************************************************************************
      //************************************************************************
      // Process command line.

      else
      {
         // Parse command line input string to get command line command.
         tCmd.parseCmdLine(tCommandLine);

         // Test for valid command line command.
         if (tCmd.mValidFlag)
         {
            // Test for exit.
            if(tCmd.isCmd("EXIT") || tCmd.isCmd("E"))
            {
               printf("EXIT\n");
               break;
            }
            // Test for a trace command.
            else if (tCmd.isCmd("TRC"))
            {
               Trc::gTraceBuffer.execute(&tCmd);
            }
            // Test for a cwd.
            else if (tCmd.isCmd("CWD"))
            {
               Prn::print(0, "%s", portableGetCurrentDir());
            }
            else if (tCmd.isCmd("HELP"))
            {
               aExec->executeHelp(&tCmd);
            }
            // Test for special commands, if the first character is a digit.
            else if (tCmd.mArgNum == 0 &&
               strlen(tCmd.mCommandLine) == 1 &&
               isdigit(tCmd.mCommandLine[0]))
            {
               int tSpecial = -1;
               sscanf(tCmd.mCommandLine, "%d", &tSpecial);
               printf("SPECIAL %d\n", tSpecial);
               // Execute the  given executive, pass it the the
               // speical command line command.
               aExec->special(tSpecial);
            }
            // Not exit or special.
            else
            {
               // Execute the  given executive, pass it the the
               // command line command.
               aExec->execute(&tCmd);

               // Test for bad command. This is true if the executive didn't
               // accept the command.
               if(tCmd.isBadCmd()) printf("INVALID COMMAND\n");
            }
         }
         // Invalid command line command.
         else
         {
            printf("INVALID COMMAND\n");
         }
      }
   }

   // Done.
   return;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Nickname.

void executeCmdLineConsole (BaseCmdLineExec* aExec)
{
   gCmdLineConsole.execute(aExec);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Console thread:

class ConsoleThread : public Ris::Threads::BaseThread
{
public:
   BaseCmdLineExec* mExec;

	ConsoleThread(BaseCmdLineExec* aExec)
	{
		mExec=aExec;
	}

	void threadRunFunction()
	{
		gCmdLineConsole.execute(mExec);
	}

	void shutdownThread()
	{
		forceTerminateThread();
	}
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

ConsoleThread* gConsoleThread=0;

//******************************************************************************
//******************************************************************************
//******************************************************************************

void executeCmdLineConsoleThread(BaseCmdLineExec* aExec)
{
	gConsoleThread = new ConsoleThread(aExec);
	gConsoleThread->launchThread();
	gConsoleThread->waitForThreadTerminate();
	delete gConsoleThread;
	gConsoleThread=0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void abortCmdLineConsole ()
{
	if (gConsoleThread)
	{
		gConsoleThread->shutdownThread();
	}
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

