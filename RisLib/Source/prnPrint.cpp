/*==============================================================================
Print utility
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "risPortableCalls.h"
#include "risNetPortDef.h"
#include "risNetUdpStringSocket.h"

#include "prnPrint.h"

namespace Prn
{

HANDLE rCreatePrintView(int aConsole);

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Regional variables

   static const int cMaxPrintStringSize = 400;
   static const int cMaxNameSize = 200;
   static const int cMaxConsoles = 5;

   bool    rUseSettingsFile;
   char    rSettingsFilePath    [cMaxNameSize];
   char    rSettingsFileSection [cMaxNameSize];
   int     rNumOfConsoles;
   bool    rSuppressFlag;

   Ris::Net::UdpTxStringSocket rConsoleSocket [cMaxConsoles];
   int                         rConsolePort   [cMaxConsoles];
   HANDLE                      rConsoleHandle [cMaxConsoles];


//****************************************************************************
//****************************************************************************
//****************************************************************************
// Initialize settings to defaults

void resetPrint()
{
   rUseSettingsFile = false;
   rSettingsFilePath[0]=0;
   strcpy(rSettingsFileSection, "DEFAULT");
   rNumOfConsoles=1;
   rSuppressFlag=true;

   strncpy(rSettingsFilePath,Ris::portableGetSettingsDir(),cMaxNameSize);
   strncat(rSettingsFilePath,"prnPrintSettings.txt",cMaxNameSize);

   for (int i=0;i<cMaxConsoles;i++)
   {
      rConsolePort   [i] = Ris::Net::PortDef::cPrintView + i;
      rConsoleHandle [i] = 0;
   }
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Override defaults

void useSettingsFileDefault()
{
   strncpy(rSettingsFilePath,Ris::portableGetSettingsDir(),cMaxNameSize);
   strncat(rSettingsFilePath,"prnPrintSettings.txt",cMaxNameSize);
   rUseSettingsFile=true;
}

void useSettingsFileName(char* aSettingsFileName)
{
   strncpy(rSettingsFilePath,Ris::portableGetSettingsDir(),cMaxNameSize);
   strncat(rSettingsFilePath,aSettingsFileName,cMaxNameSize);
   rUseSettingsFile=true;
}

void useSettingsFilePath(char* aSettingsFilePath)
{
   strncpy(rSettingsFilePath,aSettingsFilePath,cMaxNameSize);
   rUseSettingsFile=true;
}

void useSettingsFileSection(char*aSettingsFileSection)
{
   strncpy(rSettingsFileSection, aSettingsFileSection, cMaxNameSize);
}

void useConsoles(int aNumOfConsoles)
{
   rNumOfConsoles = aNumOfConsoles;
   if (rNumOfConsoles > cMaxConsoles) rNumOfConsoles = cMaxConsoles;
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Initialize

void initializePrint()
{
   //-----------------------------------------------------
   // Settings

   if (rUseSettingsFile)
   {
      gSettings.initialize(rSettingsFilePath, rSettingsFileSection);
   }

   //-----------------------------------------------------
   // Regionals

   rSuppressFlag = false;

   //-----------------------------------------------------
   // Console sockets

   for (int i = 1; i < rNumOfConsoles; i++)
   {
      rConsoleSocket[i].configure(rConsolePort[i]);
      rConsoleHandle[i] = rCreatePrintView(i);
   }
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Finalize
void finalizePrint()
{
   //-----------------------------------------------------
   // Terminate PrintView processes that were created 

   for (int i = 1; i < rNumOfConsoles; i++)
   {
      if (rConsoleHandle[i] != 0)
      {
         TerminateProcess(rConsoleHandle[i], 0);
      }
      rConsoleSocket[i].doClose();
   }
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Set filter table entry

void setFilter(int aFilter, bool aEnablePrint, int aConsole)
{
   gSettings.setFilter(aFilter,aEnablePrint, aConsole);
}   	

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Print

void print(int aFilter, const char* aFormat, ...)
{
   //-----------------------------------------------------
   // Guard for print not enabled

   // Test filter table entry
   if (gSettings.mFilterTable[aFilter] == false) return;

   // Get the console index assigned to the filter
   int tConsole = gSettings.mConsoleTable[aFilter];

   // If suppressed and the filter is not zero and
   // the console is zero then exit
   if (rSuppressFlag && aFilter != 0 && tConsole == 0) return;

   //-----------------------------------------------------
   // Print string pointer

   char* tPrintStr = 0;
   char  tPrintBuffer[cMaxPrintStringSize];
   int   tPrintStrSize;
   tPrintStr = &tPrintBuffer[0];

   //-----------------------------------------------------
   // Do a vsprintf with variable arg list into print string pointer

   va_list  ArgPtr;
   va_start(ArgPtr, aFormat);
   tPrintStrSize = vsnprintf(tPrintStr, cMaxPrintStringSize, aFormat, ArgPtr);
   va_end(ArgPtr);

   tPrintStr[tPrintStrSize++] = 0;

   //-----------------------------------------------------
   // Print the string

   if (tConsole == 0)
   {
      puts(tPrintStr);
   }
   else
   {
      rConsoleSocket[tConsole].doSendString(tPrintStr);
   }
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Suppress

void suppressPrint()
{
   rSuppressFlag=true;
}

void unsuppressPrint()
{
   rSuppressFlag=false;
}

void toggleSuppressPrint()
{
   rSuppressFlag = !rSuppressFlag;
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Launch a new process for a PrintView console application

HANDLE rCreatePrintView(int aConsole)
{
   STARTUPINFO si;
   PROCESS_INFORMATION pi;

   ZeroMemory( &si, sizeof(si) );
   si.cb = sizeof(si);
   ZeroMemory( &pi, sizeof(pi) );

   char tCommandLine[200];
   sprintf(tCommandLine,"C:\\MyLib\\Bin\\PrintView.exe  %d",rConsolePort[aConsole]);

   char tConsoleTitle[50];
   sprintf(tConsoleTitle,"PRINTVIEW%d",aConsole);
   si.lpTitle = tConsoleTitle;

   // Start the child process. 
   BOOL status = CreateProcess(
      NULL,           // Module name  
      tCommandLine,   // Command Line
      NULL,           // Process handle not inheritable
      NULL,           // Thread handle not inheritable
      FALSE,          // Set handle inheritance to FALSE
      CREATE_NEW_CONSOLE, // Creation flags
      NULL,           // Use parent's environment block
      NULL,           // Use parent's starting directory 
      &si,            // Pointer to STARTUPINFO structure
      &pi);           // Pointer to PROCESS_INFORMATION structure

   if (status)
   {
      Sleep(1000);
   }
   else
   {
      printf( "CreateProcess failed (%d).\n", GetLastError() );
      return 0;
   }

   return pi.hProcess;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize the regional variables when program is loaded

class PrintResetClass
{
public:
   PrintResetClass()
   {
      resetPrint();
   }
};

PrintResetClass gPrintResetClass;

} //namespace

