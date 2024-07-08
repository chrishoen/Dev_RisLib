/*==============================================================================
Print utility
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <stdarg.h>

#include "risAlphaDir.h"
#include "risPortableCalls.h"
#include "risNetPortDef.h"
#include "risNetUdpStringSocket.h"
#include "risNetSettings.h"

#include "prnPrintSettings.h"
#include "prnPrintFilterTable.h"
#include "prnPrint.h"

namespace Prn
{

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Constants.

static const int cMaxPrintStringSize = 1000;
static const int cMaxConsoles = PrintFilterTable::cMaxConsoles;

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Regional variables.

// Suppress any prints that do not have a filter of zero.
bool rSuppressFlag;

// If true then a print view console is used.
bool rConsoleFlag [cMaxConsoles];

// The socket for the print view console.
Ris::Net::UdpStringSocket rConsoleSocket[cMaxConsoles];

// The port number for the print view console.
int rConsolePort[cMaxConsoles];

// Print filter table defaults to the global.
PrintFilterTable* rPrintFilterTable = &gPrintFilterTable;

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Use an application specific filter table. This is used by programs
// that use shared memory.

void usePrintFilterTable(PrintFilterTable* aTable)
{
   rPrintFilterTable = aTable;
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Reset variables.

void resetVariables()
{
   rSuppressFlag = true;

   for (int i = 0; i < cMaxConsoles; i++)
   {
      rConsoleFlag[i] = false;
      rConsolePort[i] = gPrintSettings.mPrintViewIPPort + i;
   }
   // rConsoleFlag[0] = true;
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Reset print facility.

void resetPrint()
{
   // Reset the filter table.
   // DON'T DO THIS FOR SHARED MEMORY
   // THE PRINT GLOBAL CONSTRUCTOR ISN'T WORKING FOR STATIC LIB 
   gPrintFilterTable.initialize();

   // Reset the settings.
   gPrintSettings.reset();

   // If the settings file exists then read from it.
   if (gPrintSettings.fileExists())
   {
      gPrintSettings.readSection("default");
   }

   // Reset variables.
   resetVariables();
}

void resetPrint(const char* aPrintViewIPAddress, int aPrintViewIPPort)
{
   // Reset the filter table.
   // DON'T DO THIS FOR SHARED MEMORY
   // THE PRINT GLOBAL CONSTRUCTOR ISN'T WORKING FOR STATIC LIB 
   gPrintFilterTable.initialize();

   // Manually set the settings.
   gPrintSettings.reset();
   strncpy(gPrintSettings.mPrintViewIPAddress, aPrintViewIPAddress, 29);
   gPrintSettings.mPrintViewIPPort = aPrintViewIPPort;

   // Reset variables.
   resetVariables();
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Set console flag. A print view console for the index will be created and
// used.

void useConsole(int aConsole)
{
   if (aConsole <= 0) return;
   if (aConsole > cMaxConsoles-1) return;
   rConsoleFlag[aConsole] = true;
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Initialize print facility.

void initializePrint()
{
   rSuppressFlag = false;

   // For each print view console, create a PrintView process
   // and initialize and configure a socket to send the print to.
   for (int i = 1; i < cMaxConsoles; i++)
   {
      if (rConsoleFlag[i])
      {
         // Create a socket to send to the PrintView console.
         Ris::Net::Settings tSettings;
         tSettings.mTestForLocal = true;
         tSettings.setRemoteAddress(gPrintSettings.mPrintViewIPAddress, rConsolePort[i]);
         rConsoleSocket[i].initialize(&tSettings);
         rConsoleSocket[i].mPrintDisable = true;
         rConsoleSocket[i].configure();
         if (!rConsoleSocket[i].mValidFlag)
         {
            rConsoleSocket[i].showError("initializePrint");
            rConsoleFlag[i] = false;
         }
      }
   }
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Finalize the print facility.

void finalizePrint()
{
   // Terminate PrintView processes that were created. 
   for (int i = 1; i < cMaxConsoles; i++)
   {
      rConsoleSocket[i].doClose();
   }
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Set a filter in the filter table.
//
// aFilter is the index of the filter.
// aEnable is the value stored in the table at the filter index.
// aConsole is the console index stored in the table at the filter index.

void setFilter(int aFilter, bool aEnable, int aConsole)
{
   rPrintFilterTable->setFilter(aFilter,aEnable, aConsole);
}   	

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Enable a filter in the filter table.
//
// aFilter is the index of the filter.
// aEnable is the value stored in the table at the filter index.

void enableFilter(int aFilter, bool aEnable)
{
   rPrintFilterTable->enableFilter(aFilter, aEnable);
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Get a filter in the filter table.
//
// aFilter is the index of the filter.
// Return true if the filter is enabled.

bool getFilter(int aFilter)
{
   return rPrintFilterTable->getFilter(aFilter);
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Filtered print, if the corresponding entry in the filter table is true
// then the print is executed.

void print(int aFilter, const char* aFormat, ...)
{
   //*************************************************************************
   //*************************************************************************
   //*************************************************************************
   // Exit if print not enabled.

   // Exit if filter is less than zero.
   if (aFilter < 0) return;

   // Exit if suppressed and the filter is not zero.
   if (rSuppressFlag && aFilter != 0) return;

   // Exit if the filter table entry is disabled.
   if (rPrintFilterTable->mEnable[aFilter] == false) return;

   //*************************************************************************
   //*************************************************************************
   //*************************************************************************
   // Instantiate a print string buffer and a pointer.

   char* tPrintStr = 0;
   char  tPrintBuffer[cMaxPrintStringSize];
   int   tPrintStrSize;
   tPrintStr = &tPrintBuffer[0];

   //*************************************************************************
   //*************************************************************************
   //*************************************************************************
   // Do a vsprintf with variable arg list into print string pointer.

   va_list  ArgPtr;
   va_start(ArgPtr, aFormat);
   tPrintStrSize = vsnprintf(tPrintStr, cMaxPrintStringSize, aFormat, ArgPtr);
   va_end(ArgPtr);

   // If empty string then add one space.
   if (tPrintStrSize > 0)
   {
      tPrintStr[tPrintStrSize] = 0;
   }
   else
   {
      strcpy(tPrintStr, " ");
   }

   //*************************************************************************
   //*************************************************************************
   //*************************************************************************
   // Print the string.

   // Get the console index assigned to the filter.
   int tConsole = rPrintFilterTable->mConsole[aFilter];

   if (tConsole == 0 && !rConsoleFlag[0])
   {
      // Print to stdout.
      puts(tPrintStr);
   }
   else
   {
      // Print to the PrintView console.
      rConsoleSocket[tConsole].doSendString(tPrintStr);
   }
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Suppress any prints that do not have a filter of zero.

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
// Initialize the regional variables when program is loaded.

class PrintResetClass
{
public:
   PrintResetClass()
   {
      resetVariables();
   }
};

PrintResetClass gPrintResetClass;

//******************************************************************************
//******************************************************************************
//******************************************************************************
} //namespace

