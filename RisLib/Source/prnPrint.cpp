/*==============================================================================
Print utility
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "risPortableCalls.h"
#include "risNetPortDef.h"
#include "risNetUdpStringSocket.h"

#include "prnPrint.h"

namespace Prn
{

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

   Ris::Net::UdpTxStringSocket rConsoleSocket[cMaxConsoles];


//****************************************************************************
//****************************************************************************
//****************************************************************************
//****************************************************************************
//****************************************************************************
//****************************************************************************

void resetPrint()
{
   rUseSettingsFile = false;
   rSettingsFilePath[0]=0;
   strcpy(rSettingsFileSection, "DEFAULT");
   rNumOfConsoles=1;
   rSuppressFlag=true;

   strncpy(rSettingsFilePath,Ris::portableGetSettingsDir(),cMaxNameSize);
   strncat(rSettingsFilePath,"prnPrintSettings.txt",cMaxNameSize);
}

//****************************************************************************
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
      rConsoleSocket[i].configure(Ris::Net::PortDef::cPrintView + i - 1);
   }
}

//****************************************************************************

void finalizePrint()
{
   //-----------------------------------------------------
   // Console sockets

   for (int i = 1; i < rNumOfConsoles; i++)
   {
      rConsoleSocket[i].doClose();
   }
}

//****************************************************************************
void setFilter(int aFilter, bool aEnablePrint, int aConsole)
{
   gSettings.setFilter(aFilter,aEnablePrint, aConsole);
}   	

//****************************************************************************
bool testForPrint(int aFilter)
{
   // Filter table entry zero is always true
   if (aFilter==0) return true;
  
   // Lookup filter table entry
   return gSettings.mFilterTable[aFilter];
}

//****************************************************************************
void print(int aFilter, const char* aFormat, ...)
{
   // If suppressed and the filter is not zero then exit
   if (rSuppressFlag && aFilter != 0)
   {
      return;
   }

   //-----------------------------------------------------
   // If the print filter is not enabled then exit

   if (!testForPrint(aFilter))
   {
      return;
   }

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

   int tConsole = gSettings.mConsoleTable[aFilter];

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

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Trick that initializes the regional variables

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

