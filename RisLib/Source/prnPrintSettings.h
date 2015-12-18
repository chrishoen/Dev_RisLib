#ifndef _PRNPRINTSETTINGS_H_
#define _PRNPRINTSETTINGS_H_
/*==============================================================================

This file contains a set of filters used by prnPrint.h
The filters are initialized from the command line file PrintSettings.txt

==============================================================================*/

//**********************************************************************
//**********************************************************************
//**********************************************************************

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

class PrintSettings : public Ris::BaseCmdLineExec
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

   //--------------------------------------------------------------------------
   //--------------------------------------------------------------------------
   //--------------------------------------------------------------------------
   // Filter table
 
   static const int cFilterTableSize = 200;

   bool mFilterTable  [cFilterTableSize];
   int  mConsoleTable [cFilterTableSize];

   void setFilter(int aFilter, bool aEnablePrint, int aConsole = 0);

   //--------------------------------------------------------------------------
   // Section

   char mSection[200];
   char mSectionMode[200];

   bool isMySection(Ris::CmdLineCmd* aCmd);
   bool mSectionFlag;

   int  mAppNumber;

   void tryFilterStart    ();
   void tryFilterString   (Ris::CmdLineCmd* aCmd,char* aFilterString,int aFilter);
   void tryFilterSet      (Ris::CmdLineCmd* aCmd);

   int mFilterTry;

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

   enum
   {
      FilterZero,
      PrintInit1,
      PrintInit2,
      PrintInit3,
      PrintInit4,
      PrintRun1,
      PrintRun2,
      PrintRun3,
      PrintRun4,
      SocketInit1,
      SocketInit2,
      SocketInit3,
      SocketInit4,
      SocketRun1,
      SocketRun2,
      SocketRun3,
      SocketRun4,
      SerialInit1,
      SerialInit2,
      SerialInit3,
      SerialInit4,
      SerialRun1,
      SerialRun2,
      SerialRun3,
      SerialRun4,
      FileInit1,
      FileInit2,
      FileInit3,
      FileInit4,
      FileRun1,
      FileRun2,
      FileRun3,
      FileRun4,
      ThreadInit1,
      ThreadInit2,
      ThreadInit3,
      ThreadInit4,
      ThreadRun1,
      ThreadRun2,
      ThreadRun3,
      ThreadRun4,
      ProcInit1,
      ProcInit2,
      ProcInit3,
      ProcInit4,
      ProcRun1,
      ProcRun2,
      ProcRun3,
      ProcRun4,
      ExampleInit1,
      ExampleInit2,
      ExampleInit3,
      ExampleInit4,
      ExampleRun1,
      ExampleRun2,
      ExampleRun3,
      ExampleRun4,
      QCallInit1,
      QCallInit2,
      QCallInit3,
      QCallInit4,
      QCallRun1,
      QCallRun2,
      QCallRun3,
      QCallRun4,
      CommInit1,
      CommInit2,
      CommInit3,
      CommInit4,
      CommRun1,
      CommRun2,
      CommRun3,
      CommRun4,
      ViewInit1,
      ViewInit2,
      ViewInit3,
      ViewInit4,
      ViewRun1,
      ViewRun2,
      ViewRun3,
      ViewRun4,
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

