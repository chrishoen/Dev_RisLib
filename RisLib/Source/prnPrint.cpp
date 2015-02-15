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
#include "prnPrintThread.h"

#include "prnPrint.h"

namespace Prn
{

// constants

enum {MAX_PRINT_STRING_SIZE    = 400};

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Regional variables

enum {MaxNameSize=200};

bool                 rUsePrintThread;
bool                 rUseSettingsFile;
char                 rSettingsFilePath    [MaxNameSize];
char                 rSettingsFileSection [MaxNameSize];
RedirectCallPointer  rRedirectCallPointer;
bool                 rUseRedirectCallPointer;
bool                 rSuppressFlag;

//****************************************************************************
//****************************************************************************
//****************************************************************************
//****************************************************************************
//****************************************************************************
//****************************************************************************

void resetPrint()
{
   rUsePrintThread  = false;
   rUseSettingsFile = false;
   rSettingsFilePath[0]=0;
   strcpy(rSettingsFileSection, "DEFAULT");
   rRedirectCallPointer.clear();
   rUseRedirectCallPointer = false;
   rSuppressFlag=true;

   strncpy(rSettingsFilePath,Ris::portableGetSettingsDir(),MaxNameSize);
   strncat(rSettingsFilePath,"prnPrintSettings.txt",MaxNameSize);
}

//****************************************************************************

void usePrintThread (bool aUsePrintThread)
{
   rUsePrintThread = aUsePrintThread;
}

//****************************************************************************
void useSettingsFileDefault()
{
   strncpy(rSettingsFilePath,Ris::portableGetSettingsDir(),MaxNameSize);
   strncat(rSettingsFilePath,"prnPrintSettings.txt",MaxNameSize);
   rUseSettingsFile=true;
}

void useSettingsFileName(char* aSettingsFileName)
{
   strncpy(rSettingsFilePath,Ris::portableGetSettingsDir(),MaxNameSize);
   strncat(rSettingsFilePath,aSettingsFileName,MaxNameSize);
   rUseSettingsFile=true;
}

void useSettingsFilePath(char* aSettingsFilePath)
{
   strncpy(rSettingsFilePath,aSettingsFilePath,MaxNameSize);
   rUseSettingsFile=true;
}

//****************************************************************************

void useSettingsFileSection(char*aSettingsFileSection)
{
   strncpy(rSettingsFileSection, aSettingsFileSection, MaxNameSize);
}

//****************************************************************************

void useRedirectCallPointer(RedirectCallPointer  aRedirectCallPointer)
{
   rUseRedirectCallPointer = true;
   rRedirectCallPointer = aRedirectCallPointer;
}

//****************************************************************************
//****************************************************************************
//****************************************************************************

void initializePrint()
{
   //-----------------------------------------------------
   // Initialize settings

   if (rUseSettingsFile)
   {
      gSettings.initialize(rSettingsFilePath, rSettingsFileSection);
   }

   //-----------------------------------------------------
   // Launch print thread

   if (rUsePrintThread)
   {
      gPrintThread.configure(rRedirectCallPointer);
      gPrintThread.launchThread();
   }   

   //-----------------------------------------------------
   // Done
   
   rSuppressFlag = false;
}

//****************************************************************************

void finalizePrint()
{
   //-----------------------------------------------------
   // Shutdown print thread

   if (rUsePrintThread)
   {
      gPrintThread.shutdownThread();
   }   
}

//****************************************************************************
void setFilter(int aTopic, int aSubTopic, bool aEnablePrint)
{
   gSettings.setFilter(aTopic,aSubTopic,aEnablePrint);
}   	

//****************************************************************************
bool testForPrint(int aTopic,int aSubTopic)
{
    //-----------------------------------------------------
   // filter
   
   if (aTopic==0 && aSubTopic==0) return true;
  
   // lookup filter table value for any
   if (aSubTopic==0)
   {
      return gSettings.mFilterTable[aTopic]!=0;
   }

   // lookup filter table value
   return (gSettings.mFilterTable[aTopic] & aSubTopic)!=0;
}

//****************************************************************************
void print(int aTopic,int aSubTopic,const char* aFormat, ...)      
{
   // If suppressed and the filter is not zero then exit
   if (rSuppressFlag && aTopic!=0)
   {
      return;
   }

   //-----------------------------------------------------
   // If the print filter is not enabled then exit

   if (!testForPrint(aTopic,aSubTopic))
   {
      return;
   }

   //-----------------------------------------------------
   // Print string pointer

   char* tPrintStr=0;
   char  tPrintBuffer[MAX_PRINT_STRING_SIZE];
   int   tPrintStrSize;
   PrintBlock* tPrintBlock = 0;

   // If non threaded
   if (!rUsePrintThread)
   {
      // Print to local storage
      tPrintStr = &tPrintBuffer[0];
   }
   // If threaded
   else
   {
      // Print to new print block
      tPrintBlock = new PrintBlock;
      tPrintStr = &tPrintBlock->mString[0];
   }

   //-----------------------------------------------------
   // Do a vsprintf with variable arg list into print string pointer
   
   va_list  ArgPtr;
   va_start(ArgPtr,aFormat);
   tPrintStrSize = vsnprintf(tPrintStr,MAX_PRINT_STRING_SIZE,aFormat,ArgPtr);
   va_end (ArgPtr);

   tPrintStr[tPrintStrSize++]=0;

   //-----------------------------------------------------
   // Print the string

   // If non threaded
   if (!rUsePrintThread)
   {
      // If redirection call pointer 
      if (rUseRedirectCallPointer)
      {
         // Send to redirection call pointer 
         rRedirectCallPointer(tPrintStr);
      }
      // If no redirection
      else 
      {
         // Send to standard output
         puts(tPrintStr);
      }
   }
   // If threaded
   else
   {
      // Send to print thread
      gPrintThread.mPrintBlockQCall(tPrintBlock);
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

