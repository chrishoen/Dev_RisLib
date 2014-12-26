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
// Push the executive onto the nested executive stack

void BaseCmdLineExec::nestedPush(CmdLineCmd* aCmd,BaseCmdLineExec* aNextExec)
{
   // Guard
   if (aCmd->mNestedAnchor == 0) return;

   // Push the executive onto the nested executive stack
   aCmd->mNestedAnchor->nestedPush(aNextExec);
}

//******************************************************************************
// Pop the executive from the nested executive stack

void BaseCmdLineExec::nestedPop(CmdLineCmd* aCmd)
{
   // Guard
   if (aCmd->mNestedAnchor == 0) return;

   // Pop the executive from the nested executive stack
   aCmd->mNestedAnchor->nestedPop();
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

