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

void BaseCmdLineExec::nestedPush(BaseCmdLineExec* aNextExec)
{
   // Guard
   if (mAnchor == 0) return;

   // Push the executive onto the nested executive stack
   mAnchor->nestedPush(aNextExec);
}

//******************************************************************************
// Pop the executive from the nested executive stack

void BaseCmdLineExec::nestedPop()
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
   mChangeFlag = false;
   mExec = 0;
}

//******************************************************************************
// Initialize 
void CmdLineExecNestedAnchor::initialize(BaseCmdLineExec* aInitialExec)
{
   mChangeFlag = false;
   mExec = aInitialExec;
}
//******************************************************************************
// Push the executive onto the nested executive stack

void CmdLineExecNestedAnchor::nestedPush(BaseCmdLineExec* aNextExec)
{
   // Push current executive onto the stack
   mExecStack.push(mExec);

   // Set the current executive
   mExec = aNextExec;

   // Change
   mChangeFlag = true;
}

//******************************************************************************
// Pop the executive from the nested executive stack

void CmdLineExecNestedAnchor::nestedPop  ()
{
   // Pop from the stack to set the current executive 
   mExecStack.pop(mExec);

   // Change
   mChangeFlag = true;
}

//******************************************************************************
}//namespace

