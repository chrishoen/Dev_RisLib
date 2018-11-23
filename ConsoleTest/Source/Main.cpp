#include "stdafx.h"

#include "MainInit.h"
#include "risCmdLineConsole.h"
#include "CmdLineExec.h"

#include "conStringReader.h"


//******************************************************************************
//******************************************************************************
//******************************************************************************

int main(int argc,char** argv)
{
   main_initialize(argc,argv);

   Con::gStringReader.doTestLoop1();

   main_finalize();
   return 0;
}
