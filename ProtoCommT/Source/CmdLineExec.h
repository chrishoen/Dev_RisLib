#ifndef _CMDLINEEXEC_H_
#define _CMDLINEEXEC_H_

#include "risCmdLineExec.h"

//******************************************************************************
// specific command line executive
//******************************************************************************
class CmdLineExec : public Ris::BaseCmdLineExec
{
public:

  CmdLineExec();

  void reset();
  void execute      (Ris::CmdLineCmd* aCmd);

  void executeShutdown  (Ris::CmdLineCmd* aCmd);
  void executeTx        (Ris::CmdLineCmd* aCmd);
  void executeGo1       (Ris::CmdLineCmd* aCmd);
  void executeGo2       (Ris::CmdLineCmd* aCmd);
  void executeTest1     (Ris::CmdLineCmd* aCmd);
  void executePeriodic  (Ris::CmdLineCmd* aCmd);
};
//******************************************************************************
#endif

