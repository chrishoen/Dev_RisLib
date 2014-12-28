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

  void seqShutdown  (Ris::CmdLineCmd* aCmd);
  void seqTx        (Ris::CmdLineCmd* aCmd);
  void seqGo1       (Ris::CmdLineCmd* aCmd);
  void seqTest1     (Ris::CmdLineCmd* aCmd);
  void seqPeriodic  (Ris::CmdLineCmd* aCmd);

  enum{SERVER_E=1,CLIENT_E=2};
  int mRole;

};
//******************************************************************************
#endif

