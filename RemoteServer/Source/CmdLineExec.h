#ifndef _CMDLINEEXEC_H_
#define _CMDLINEEXEC_H_

#include "risCmdLineCmd.h"

//******************************************************************************
// specific command line sequencer
//******************************************************************************
class CmdLineExec : public Ris::BaseCmdLineExec
{
public:

  CmdLineExec();

  void reset();
  void execute(Ris::CmdLineCmd* aCmd);

  void executeTest1   (Ris::CmdLineCmd* aCmd);
  void executeCmdLine (Ris::CmdLineCmd* aCmd);
};
//******************************************************************************
#endif

