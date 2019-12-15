#pragma once
#include <string>
#include <vector>

/*==============================================================================
These are some functions that do system calls.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

// Execute a system command. return zero if successful.
int doSystemCommand(const char* aCommand);

// Execute a system command. return zero if successful.
// Suppress print.
int doSystemCommandSuppress(const char* aCommand);

// Execute a system command. return zero if successful.
// Also return the output of the command into a list of strings.
int doSystemCommand(const char* aCommand,std::vector<std::string>& aResponse);

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace


