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
  void execute       (Ris::CmdLineCmd* aCmd);

  void executeGo1  (Ris::CmdLineCmd* aCmd);
  void executeGo2  (Ris::CmdLineCmd* aCmd);
  void executeGo3  (Ris::CmdLineCmd* aCmd);
  void executeGo4  (Ris::CmdLineCmd* aCmd);
  void executeGo5  (Ris::CmdLineCmd* aCmd);
  void executeGo6  (Ris::CmdLineCmd* aCmd);
  void executeGo7  (Ris::CmdLineCmd* aCmd);
  void executeGo8  (Ris::CmdLineCmd* aCmd);
  void executeGo9  (Ris::CmdLineCmd* aCmd);

};

//******************************************************************************
//******************************************************************************
//******************************************************************************

