#pragma once

/*==============================================================================
Program command line executive.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "MySettings1.h"
#include "MySettings2.h"
#include "risCmdLineExec.h"

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

   MySettings1 mSettings1;
   MySettings2 mSettings2;
   int mLockFile;

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

   void executeRead1(Ris::CmdLineCmd* aCmd);
   void executeWrite1(Ris::CmdLineCmd* aCmd);

   void executeBegin2(Ris::CmdLineCmd* aCmd);
   void executeEnd2(Ris::CmdLineCmd* aCmd);
   void executeRead2(Ris::CmdLineCmd* aCmd);

   void executeLock(Ris::CmdLineCmd* aCmd);
   void executeUnlock(Ris::CmdLineCmd* aCmd);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

