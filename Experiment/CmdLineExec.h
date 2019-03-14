#pragma once

#include "risCmdLineExec.h"

#include "TestTable.h"
//******************************************************************************
//******************************************************************************
//******************************************************************************

class CmdLineExec : public Ris::BaseCmdLineExec
{
public:

   TestTable<int, 10> mIntTable;

   CmdLineExec();

   void reset();
   void execute(Ris::CmdLineCmd* aCmd);

   void executeGo1(Ris::CmdLineCmd* aCmd);
   void executeGo2(Ris::CmdLineCmd* aCmd);
   void executeGo3(Ris::CmdLineCmd* aCmd);
   void executeGo4(Ris::CmdLineCmd* aCmd);
   void executeGo5(Ris::CmdLineCmd* aCmd);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
