#pragma once

#include "risCmdLineExec.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

class CmdLineExec : public Ris::BaseCmdLineExec
{
public:

  CmdLineExec();

  void reset();
  void execute(Ris::CmdLineCmd* aCmd);
  void special(int aSpecial);

  void executeSend      (Ris::CmdLineCmd* aCmd);
  void executeEcho      (Ris::CmdLineCmd* aCmd);
  void executeData      (Ris::CmdLineCmd* aCmd);
  void executeAbort     (Ris::CmdLineCmd* aCmd);
  void executeGo1       (Ris::CmdLineCmd* aCmd);
  void executeGo2       (Ris::CmdLineCmd* aCmd);
  void executeGo3       (Ris::CmdLineCmd* aCmd);
  void executeGo4       (Ris::CmdLineCmd* aCmd);
  void executeParms     (Ris::CmdLineCmd* aCmd);

};

//******************************************************************************
//******************************************************************************
//******************************************************************************

