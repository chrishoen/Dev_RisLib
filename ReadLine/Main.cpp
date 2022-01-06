#include "stdafx.h"

#include <readline/readline.h>
#include <readline/history.h>

#include "risCmdLineReader.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

int main(int argc,char** argv)
{
   char tCommandLine[200];
   while (Ris::gCmdLineReader.readLine(tCommandLine, 200))
   {
      if (strcmp(tCommandLine, "e") == 0)
      {
         return 0;
      }
   }
   return 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
#if 0

using_history();

while (char* tLine = readline(""))
{
   if (strcmp(tLine, "e") == 0)
   {
      free(tLine);
      return 0;
   }
   add_history(tLine);
   free(tLine);
}

return 0;

#endif