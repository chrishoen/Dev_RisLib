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
  void execute(Ris::CmdLineCmd* aCmd);

  void executeGo1     (Ris::CmdLineCmd* aCmd);
  void executeF101    (Ris::CmdLineCmd* aCmd);
  void executeF102    (Ris::CmdLineCmd* aCmd);

};
//******************************************************************************
#endif

