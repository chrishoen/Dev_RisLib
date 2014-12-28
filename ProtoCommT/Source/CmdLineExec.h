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

  void executeOnShutdown  (Ris::CmdLineCmd* aCmd);
  void executeOnTx        (Ris::CmdLineCmd* aCmd);
  void executeOnGo1       (Ris::CmdLineCmd* aCmd);
  void executeOnTest1     (Ris::CmdLineCmd* aCmd);
  void executeOnPeriodic  (Ris::CmdLineCmd* aCmd);
};
//******************************************************************************
#endif

