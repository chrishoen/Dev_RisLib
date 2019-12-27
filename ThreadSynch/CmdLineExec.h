#pragma once

#include "risCmdLineExec.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

class CmdLineExec : public Ris::BaseCmdLineExec
{
public:

  CmdLineExec();

  void reset();
  void execute(Ris::CmdLineCmd* aCmd);

  void executeGo      (Ris::CmdLineCmd* aCmd);
  void executeF101    (Ris::CmdLineCmd* aCmd);
  void executeF102    (Ris::CmdLineCmd* aCmd);
  void executeNMutex  (Ris::CmdLineCmd* aCmd);

};

//******************************************************************************
//******************************************************************************
//******************************************************************************

