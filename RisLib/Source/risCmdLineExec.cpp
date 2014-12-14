/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
//#include <ctype.h>
#include <string.h>

#include "risCmdLineExec.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor

BaseCmdLineExec::BaseCmdLineExec()
{
   mExitFlag = false;
   mAnchor   = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Exit

void BaseCmdLineExec::forceExit()
{
   // This can be used to cause and "EXIT" command for the executer
   mExitFlag=true;;
}

//******************************************************************************
// Set the nested anchor

void BaseCmdLineExec::setAnchor(CmdLineExecNestedAnchor* aAnchor)
{
   mAnchor = aAnchor;
}

//******************************************************************************
// Push the executive onto the nested executive stack

void BaseCmdLineExec::nestedPush (BaseCmdLineExec* aExec)
{
   // Guard
   if (mAnchor == 0) return;

   // Push the executive onto the nested executive stack
   mAnchor->nestedPush(aExec);
}

//******************************************************************************
// Pop the executive from the nested executive stack

void BaseCmdLineExec::nestedPop  ()
{
   // Guard
   if (mAnchor == 0) return;

   // Pop the executive from the nested executive stack
   mAnchor->nestedPop();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor

CmdLineExecNestedAnchor::CmdLineExecNestedAnchor()
{
   mPushFlag = false;
   mExec = 0;
}

//******************************************************************************
// Push the executive onto the nested executive stack

void CmdLineExecNestedAnchor::nestedPush (BaseCmdLineExec* aExec)
{
   mPushFlag = true;
   mExec     = aExec;
   mExecStack.push(mExec);
}

//******************************************************************************
// Pop the executive from the nested executive stack

void CmdLineExecNestedAnchor::nestedPop  ()
{
   mExecStack.pop(mExec);
}

//******************************************************************************
}//namespace

