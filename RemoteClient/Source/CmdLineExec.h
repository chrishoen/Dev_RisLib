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

  void executeShutdown(Ris::CmdLineCmd* aCmd);
  void executeAbort   (Ris::CmdLineCmd* aCmd);
  void executeTP      (Ris::CmdLineCmd* aCmd);
  void executeF101    (Ris::CmdLineCmd* aCmd);
  void executeF102    (Ris::CmdLineCmd* aCmd);
  void executeC101    (Ris::CmdLineCmd* aCmd);
  void executeC102    (Ris::CmdLineCmd* aCmd);

  void executeTest1   (Ris::CmdLineCmd* aCmd);
  void executeTest2   (Ris::CmdLineCmd* aCmd);
  void executeTest3   (Ris::CmdLineCmd* aCmd);
  void executeTest4   (Ris::CmdLineCmd* aCmd);
  void executeTest5   (Ris::CmdLineCmd* aCmd);
  void executeMarker1 (Ris::CmdLineCmd* aCmd);
  void executeMarker2 (Ris::CmdLineCmd* aCmd);

};
//******************************************************************************
#endif

