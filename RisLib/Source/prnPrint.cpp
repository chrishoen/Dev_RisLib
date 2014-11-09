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
#include "risRemoteClientThread.h"

#include "prnPrint.h"

namespace Prn
{

// constants

enum {MAX_PRINT_STRING_SIZE    = 400};

//****************************************************************************
// regional variables

PrintMode rPrintMode=PrintModeLocalThreaded;
FILE* rPrintFileFile=0;
bool  rInitialized = false;

//****************************************************************************

void suppressPrint()
{
   gSettings.mSuppressFlag=true;
}

void unsuppressPrint()
{
   gSettings.mSuppressFlag=false;
}

void toggleSuppressPrint()
{
   gSettings.mSuppressFlag = !gSettings.mSuppressFlag;
}

//****************************************************************************

void initializePrint(PrintMode aPrintMode)
{
   //-----------------------------------------------------
   // Guard

	if (gSettings.mInitFlag) return;
	gSettings.mInitFlag=true;

   //-----------------------------------------------------
   // Initialize variables
   
   rPrintMode = aPrintMode;
   gSettings.mSuppressFlag = false;

   //-----------------------------------------------------
   // Launch print thread

   switch (rPrintMode)
   {
      case PrintModeLocal : 
            break;
      case PrintModeLocalThreaded : 
            gPrintThread.configureThread();
            gPrintThread.launchThread();
            break;
      case PrintModeRemote : 
         break;
      default : 
         break;
   }   
}

//****************************************************************************
// This initializes, settings are already initialized

void initializePrintFromSettings(
   char* aSettingsFileName,
   char* aSection,
   PrintMode aPrintMode)
{
   //-----------------------------------------------------
   // Guard

	if (gSettings.mInitFlag) return;
	gSettings.mInitFlag=true;

   //-----------------------------------------------------
   // Initialize variables
   
   rPrintMode = aPrintMode;
   gSettings.mSuppressFlag = false;

   //-----------------------------------------------------
   // initialize settings

   gSettings.initialize(aSettingsFileName,aSection);

   //-----------------------------------------------------
   // Launch print thread

   switch (rPrintMode)
   {
      case PrintModeLocal : 
            break;
      case PrintModeLocalThreaded : 
            gPrintThread.configureThread();
            gPrintThread.launchThread();
            break;
      case PrintModeRemote : 
         break;
      default : 
         break;
   }   

   //-----------------------------------------------------
   // Open log files

   if (gSettings.mLogFileEnable)
   {
      char tPrintFileName[200];
      strcpy(tPrintFileName,gSettings.mLogFileName);
      rPrintFileFile=fopen(tPrintFileName,"w");
      if(rPrintFileFile<=0)
      {
         gSettings.mLogFileEnable=false;
         printf("BAD PRINT LOG FILE NAME %s\n",gSettings.mLogFileName);
      }
   }

   //-----------------------------------------------------
   // Test print

   print(Prn::PrintRun,Prn::Run1,"PrintRun,Run1");
   print(Prn::PrintRun,Prn::Run2,"PrintRun,Run2");
}

//****************************************************************************
// This initializes, settings are already initialized

void initializePrintFromSettings(PrintMode aPrintMode)
{
   char tFilePath[200];
   strcpy(tFilePath,Ris::portableGetSettingsDir());
   strcat(tFilePath,"prnPrintSettings.txt");

   initializePrintFromSettings(tFilePath,"Default",aPrintMode);
}

//****************************************************************************

void closePrint()
{
   //-----------------------------------------------------
   // Shutdown print thread

   switch (rPrintMode)
   {
      case PrintModeLocal : 
         break;
      case PrintModeLocalThreaded : 
         gPrintThread.shutdownThread();
         break;
      case PrintModeRemote : 
         break;
      default : 
         break;
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
   if (gSettings.mSuppressFlag)
   {
      if (aTopic!=0) return;
   }

   //-----------------------------------------------------
   // filter

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
   Ris::RemoteMsg::PrintStrMsg* tPrintStrMsg = 0;

   switch (rPrintMode)
   {
      case PrintModeLocal : 
      {
         // Local print
         tPrintStr = &tPrintBuffer[0];
      }
      break;
      case PrintModeLocalThreaded :
      {
         //Local print via print thread
         tPrintBlock = new PrintBlock;
         tPrintStr = &tPrintBlock->mString[0];
      }
      break;
      case PrintModeRemote : 
      {
         //Remote print via remote client thread
         tPrintStrMsg = new Ris::RemoteMsg::PrintStrMsg;
         tPrintStr = &tPrintStrMsg->mPrintStr[0];
      }
      break;
      default : 
      {
         // Local print
         tPrintStr = &tPrintBuffer[0];
      }
      break;
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

   switch (rPrintMode)
   {
      case PrintModeLocal : 
      {
         // Local print
         puts(tPrintStr);
      }
      break;
      case PrintModeLocalThreaded :
      {
         //Local print via print thread
         gPrintThread.mPrintBlockQCall(tPrintBlock);
      }
      break;
      case PrintModeRemote : 
      {
         //Remote print via remote client thread
         Ris::Remote::gClientThread->sendMsgToRemote(tPrintStrMsg);
      }
      break;
      default : 
         break;
   }   
}

} //namespace

