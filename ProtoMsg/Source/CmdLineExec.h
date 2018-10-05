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

  void executeOnGo1  (Ris::CmdLineCmd* aCmd);
  void executeOnGo2  (Ris::CmdLineCmd* aCmd);
  void executeOnGo3  (Ris::CmdLineCmd* aCmd);
  void executeOnGo4  (Ris::CmdLineCmd* aCmd);
  void executeOnGo5  (Ris::CmdLineCmd* aCmd);
  void executeOnGo6  (Ris::CmdLineCmd* aCmd);

};

//******************************************************************************
//******************************************************************************
//******************************************************************************

