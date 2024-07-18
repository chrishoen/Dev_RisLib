//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risCmdLineFile.h"
#include "risPortableCalls.h"


#define  _PROCOTCPPARMS_CPP_
#include "procoTcpParms.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace ProtoComm
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

TcpParms::TcpParms()
{
   reset();
}

void TcpParms::reset()
{
   BaseClass::reset();
   BaseClass::setFilePath("files/ProtoComm_TcpParms.txt");

   mMyAppNumber = 0;

   strcpy(mTcpServerAddress, "0.0.0.0");
   mTcpServerPort = 0;
   mTcpMaxSessions = 0;

   mThreadTimerPeriod = 0;
   mNumWords = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show.

void TcpParms::show()
{
   printf("\n");
   printf("TcpParms************************************************ %s\n", mTargetSection);

   printf("MyAppNumber             %16d\n",       mMyAppNumber);

   printf("TcpServerAddress        %16s\n",       mTcpServerAddress);
   printf("TcpServerPort           %16d\n",       mTcpServerPort);
   printf("TcpMaxSessions          %16d\n",       mTcpMaxSessions);

   printf("\n");
   printf("ThreadTimerPeriod       %16d\n", mThreadTimerPeriod);
   printf("NumWords                %16d\n", mNumWords);

   printf("TcpParms************************************************\n");
   printf("\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Base class override: Execute a command from the command file to set a 
// member variable.  Only process commands for the target section.This is
// called by the associated command file object for each command in the file.

void TcpParms::execute(Ris::CmdLineCmd* aCmd)
{
   if (!isTargetSection(aCmd)) return;

   if (aCmd->isCmd("MyAppNumber"))   mMyAppNumber = aCmd->argInt(1);

   if (aCmd->isCmd("TcpServerAddress"))    aCmd->copyArgString(1, mTcpServerAddress,cMaxStringSize);
   if (aCmd->isCmd("TcpServerPort"))       mTcpServerPort = aCmd->argInt(1);
   if (aCmd->isCmd("TcpMaxSessions"))      mTcpMaxSessions = aCmd->argInt(1);

   if (aCmd->isCmd("ThreadTimerPeriod"))   mThreadTimerPeriod = aCmd->argInt(1);
   if (aCmd->isCmd("NumWords"))            mNumWords = aCmd->argInt(1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Calculate expanded member variables. This is called after the entire
// section of the command file has been processed.

void TcpParms::expand()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace