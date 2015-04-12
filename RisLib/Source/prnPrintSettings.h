#ifndef _PRNPRINTSETTINGS_H_
#define _PRNPRINTSETTINGS_H_
/*==============================================================================

This file contains a set of filters used by prnPrint.h
The filters are initialized from the command line file PrintSettings.txt

==============================================================================*/

//**********************************************************************
//**********************************************************************
//**********************************************************************

#include "risPortableTypes.h"
#include "risCmdLineFile.h"

namespace Prn
{

//**********************************************************************
//**********************************************************************
//**********************************************************************
// PrintSettings class, inherits from BaseCmdLineExec to process
// command lines from a command line file.
// Each application reads its own print settings from a common settings
// file. 

class  PrintSettings : public Ris::BaseCmdLineExec
{
public:

   PrintSettings();

   void reset();

   // Baseclass override, executes for each line in the settings
   // command line file

   void execute(Ris::CmdLineCmd* aCmd);

   // Initialize.
   // aSettingsFileName is the settings filename. aSection is the settings
   // file section that each application extracts its settings from. 
   // aSectionMode specifies the section mode. Each section begins with a 
   // command line, such as "Begin CmdFile Default", where "Begin" is the 
   // command ,"CmdFile" is  first argument and denotes the section and
   // "Default" is the second argument and denotes the section mode.

   bool initialize(
      char* aSettingsFileName,
      char* aSection);

   // Shows the filter settings
   void show();

   //--------------------------------------------------------------------------
   //--------------------------------------------------------------------------
   //--------------------------------------------------------------------------
   // Filter table
 
   enum {FILTER_TABLE_SIZE     = 100};

   unsigned int mFilterTable[FILTER_TABLE_SIZE];

   void setFilter(int aTopic, int aSubTopic, bool aEnablePrint);

   //--------------------------------------------------------------------------
   // Section

   char mSection[200];
   char mSectionMode[200];

   bool isMySection(Ris::CmdLineCmd* aCmd);
   bool mSectionFlag;

   int  mAppNumber;

   void tryFilterStart    ();
   void tryFilterTopic    (Ris::CmdLineCmd* aCmd,char* aTopicStr,int aTopic);
   void tryFilterSubTopic (Ris::CmdLineCmd* aCmd,char* aSubTopicStr,int aSubTopic);
   void tryFilterSet      (Ris::CmdLineCmd* aCmd);

   int mFilterTopic;
   int mFilterSubTopic;

   //--------------------------------------------------------------------------
   // Print control

   // If this flag is true then initialize has been called
   bool  mInitFlag;

   // If this flag is true then any print that does not have a
   // filter,subfilter of 0,0 will not be printed
   bool  mSuppressFlag;

   // If this flag is false then any special print immediately returns.
   bool  mSpecialFlag;

   // Log file for print.txt
   bool  mLogFileEnable;
   char  mLogFileName[200];
};

//**********************************************************************
//**********************************************************************
//**********************************************************************
// Filter constants
//**********************************************************************
//**********************************************************************
//**********************************************************************
// Topics

   enum
   {
      PrintInit,
      PrintRun,
      PrintPer,
      SocketInit,
      SocketRun,
      SocketPer,
      SerialInit,
      SerialRun,
      FileInit,
      FileRun,
      FilePer,
      ThreadInit,
      ThreadRun,
      ThreadPer,
      ProcInit,
      ProcRun,
      ProcPer,
      Example1Init,
      Example1Run,
      Example1Per,
      QCallThreadInit,
      CommInit,
      CommRun,
      Msg,
   };

//**********************************************************************
// SubTopics

   enum
   { 
      Any    =    1,
      Init1  = 1<<1,
      Init2  = 1<<2,
      Init3  = 1<<3,
      Init4  = 1<<4,
      Run1   = 1<<5,
      Run2   = 1<<6,
      Run3   = 1<<7,
      Run4   = 1<<8,
      Per1   = 1<<9,
      Per2   = 1<<10,
      Per3   = 1<<11,
      Per4   = 1<<12,
      Tx1    = 1<<13,
      Tx2    = 1<<14,
      Tx3    = 1<<15,
      Tx4    = 1<<16,
      Rx1    = 1<<17,
      Rx2    = 1<<18,
      Rx3    = 1<<19,
      Rx4    = 1<<20,
      Show1  = 1<<21,
      Show2  = 1<<22,
      Show3  = 1<<23,
      Show4  = 1<<24,
      Spare1 = 1<<25,
      Spare2 = 1<<26,
      Spare3 = 1<<27,
      Spare4 = 1<<28,
   };

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global instance

#ifdef _PRNPRINTSETTINGS_CPP_
        PrintSettings gSettings;
#else
        extern PrintSettings gSettings;
#endif

}//namespace
#endif

