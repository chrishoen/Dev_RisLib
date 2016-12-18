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

  void executeTP            (Ris::CmdLineCmd* aCmd);
  void executeTest1         (Ris::CmdLineCmd* aCmd);
  void executeTest2         (Ris::CmdLineCmd* aCmd);
  void executeSend          (Ris::CmdLineCmd* aCmd);
  void executeResponse      (Ris::CmdLineCmd* aCmd);
  void executeSequence1     (Ris::CmdLineCmd* aCmd);
  void executeSequence2     (Ris::CmdLineCmd* aCmd);
  void executeAbort         (Ris::CmdLineCmd* aCmd);
  void executeWorkRequest   (Ris::CmdLineCmd* aCmd);

};
//******************************************************************************
#endif

