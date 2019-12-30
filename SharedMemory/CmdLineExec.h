#pragma once

/*==============================================================================
Program command line executive.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risCmdLineExec.h"
#include "risThreadsSynch.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This class is the program command line executive. It processes user
// command line inputs and executes them. It inherits from the command line
// command executive base class, which provides an interface for executing
// command line commands. It provides an override execute function that is
// called by a console executive when it receives a console command line input.
// The execute function then executes the command.

class CmdLineExec : public Ris::BaseCmdLineExec
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   Ris::Threads::NamedMutex gMutex;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Infrastructure.

   CmdLineExec();
   void reset();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Base class override. Execute a command line command. It calls one of
   // the following specific command execution functions. This is called by
   // the owner of this object to pass command line commands to it. 
   void execute(Ris::CmdLineCmd* aCmd) override;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Execute specific commands.
   void executeGo1(Ris::CmdLineCmd* aCmd);
   void executeGo2(Ris::CmdLineCmd* aCmd);
   void executeGo3(Ris::CmdLineCmd* aCmd);
   void executeGo4(Ris::CmdLineCmd* aCmd);
   void executeGo5(Ris::CmdLineCmd* aCmd);

   void executeMutexTest1(Ris::CmdLineCmd* aCmd);
   void executeMutexTest2(Ris::CmdLineCmd* aCmd);

   void executeMutexCreate(Ris::CmdLineCmd* aCmd);
   void executeMutexOpen(Ris::CmdLineCmd* aCmd);
   void executeMutexLock(Ris::CmdLineCmd* aCmd);
   void executeMutexUnlock(Ris::CmdLineCmd* aCmd);
   void executeMutexClose(Ris::CmdLineCmd* aCmd);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

