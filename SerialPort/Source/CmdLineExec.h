#pragma once

#include "risCmdLineExec.h"

#include "risSerialPort.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

class CmdLineExec : public Ris::BaseCmdLineExec
{
public:

  CmdLineExec();

  void reset();
  void execute(Ris::CmdLineCmd* aCmd);

  void executeGo1       (Ris::CmdLineCmd* aCmd);
  void executeGo2       (Ris::CmdLineCmd* aCmd);
  void executeGo3       (Ris::CmdLineCmd* aCmd);
  void executeOpen      (Ris::CmdLineCmd* aCmd);
  void executeClose     (Ris::CmdLineCmd* aCmd);



  Ris::SerialPort mPort;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

