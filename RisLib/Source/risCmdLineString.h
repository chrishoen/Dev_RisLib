#ifndef _RISCMDLINESTRING_H_
#define _RISCMDLINESTRING_H_
/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risCmdLineExec.h"

//******************************************************************************
namespace Ris
{
class  CmdLineString
{
public:

   CmdLineString();

   // This executes a command line executive, passing it a command line string.
   // It returns a response string, if required, else a null.

   char* execute (BaseCmdLineExec* aExec, char* aCmdLineString);

protected:
   // Response string storage
   char mResponseString[CmdLineCmd::ResponseStringSize];
};

//******************************************************************************
}//namespace
#endif

