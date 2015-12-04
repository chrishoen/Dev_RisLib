#ifndef _CMDLINEEXEC_H_
#define _CMDLINEEXEC_H_

#include "risCmdLineExec.h"

//******************************************************************************
// specific command line executeOnuencer
//******************************************************************************
class CmdLineExec : public Ris::BaseCmdLineExec
{
public:

  CmdLineExec();

  void reset();
  void execute      (Ris::CmdLineCmd* aCmd);

  void executeOnShutdown  (Ris::CmdLineCmd* aCmd);
  void executeOnGo1       (Ris::CmdLineCmd* aCmd);
  void executeOnGo2       (Ris::CmdLineCmd* aCmd);
  void executeOnGo3       (Ris::CmdLineCmd* aCmd);
  void executeOnGo4       (Ris::CmdLineCmd* aCmd);

};
//******************************************************************************
#endif

