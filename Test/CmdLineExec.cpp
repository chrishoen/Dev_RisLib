#include "stdafx.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include "string.h"

#include "risProgramTime.h"
#include "risBitUtils.h"
#include "my_functions.h"
#include "CmdLineExec.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

CmdLineExec::CmdLineExec()
{
}

void CmdLineExec::reset()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This class is the program command line executive. It processes user
// command line inputs and executes them. It inherits from the command line
// command executive base class, which provides an interface for executing
// command line commands. It provides an override execute function that is
// called by a console executive when it receives a console command line input.
// The execute function then executes the command.

void CmdLineExec::execute(Ris::CmdLineCmd* aCmd)
{
   if(aCmd->isCmd("RESET"))   reset();
   if (aCmd->isCmd("GO1"))    executeGo1(aCmd);
   if (aCmd->isCmd("GO2"))    executeGo2(aCmd);
   if (aCmd->isCmd("GO3"))    executeGo3(aCmd);
   if (aCmd->isCmd("GO4"))    executeGo4(aCmd);
   if (aCmd->isCmd("GO5"))    executeGo5(aCmd);
   if (aCmd->isCmd("GO6"))    executeGo6(aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************


typedef union MacAddrUnionT
{
    typedef struct MacWordStructT
    {
        unsigned long mLsw;
        unsigned short mMsw;
    } MacWordStruct;
    MacWordStruct mWordStruct;
    unsigned char mBytes[6];
} MacAddrUnion;

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
    MacAddrUnion tUnion;
    memset(&tUnion, 0, sizeof(tUnion));
    tUnion.mBytes[0] = 0x00;
    tUnion.mBytes[1] = 0x11;
    tUnion.mBytes[2] = 0x22;
    tUnion.mBytes[3] = 0x33;
    tUnion.mBytes[4] = 0x44;
    tUnion.mBytes[5] = 0x55;

    unsigned int tLsw = tUnion.mWordStruct.mLsw;
    unsigned int tMsw = tUnion.mWordStruct.mMsw;

    Prn::print(0, "tLsw %08x", tLsw);
    Prn::print(0, "tMsw %08x", tMsw);
}


//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo3(Ris::CmdLineCmd* aCmd)
{
   char tTemp[40];
   Prn::print(0, "%s", my_timestamp2(tTemp));
   Prn::print(0, "%s", my_timestamp4(tTemp));
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo4(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo5(Ris::CmdLineCmd* aCmd)
{
   printf("printf\n");
   Prn::print(0, "Prn::print 0");
   Prn::print(Prn::Show1, "Prn::print Prn::Show1");
   Prn::print(Prn::Show2, "Prn::print Prn::Show");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo6(Ris::CmdLineCmd* aCmd)
{
   Prn::print(0, "Ris::portableGetCurrentDir() %s", Ris::portableGetCurrentDir());
   Prn::print(0, "Ris::portableGetBinDir() %s", Ris::portableGetBinDir());
   double tTime = Ris::getProgramTime();
   unsigned int tTimeMS = Ris::getCpuTimeUIntMS();

   Prn::print(0, "Ris::getProgramTime           %10.6f", tTime);
   Prn::print(0, "Ris::getCpuTimeUIntMs         %10d", tTimeMS);
}

