#ifndef _CMDLINEEXEC_H_
#define _CMDLINEEXEC_H_

#include "risCmdLineExec.h"

//******************************************************************************
// specific command line sequencer
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

  int mRole;

};
//******************************************************************************
#endif

