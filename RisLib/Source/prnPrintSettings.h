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
// command lines from a command line file. Each application reads its
// own print settings from a common settings file. 

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

   static const int  FilterZero        =   0;
   static const int  PrintInit1        =   1;
   static const int  PrintInit2        =   2;
   static const int  PrintInit3        =   3;
   static const int  PrintInit4        =   4;
   static const int  PrintRun1         =   5;
   static const int  PrintRun2         =   6;
   static const int  PrintRun3         =   7;
   static const int  PrintRun4         =   8;
   static const int  SocketInit1       =   9;
   static const int  SocketInit2       =  10;
   static const int  SocketInit3       =  11;
   static const int  SocketInit4       =  12;
   static const int  SocketRun1        =  13;
   static const int  SocketRun2        =  14;
   static const int  SocketRun3        =  15;
   static const int  SocketRun4        =  16;
   static const int  SerialInit1       =  17;
   static const int  SerialInit2       =  18;
   static const int  SerialInit3       =  19;
   static const int  SerialInit4       =  20;
   static const int  SerialRun1        =  21;
   static const int  SerialRun2        =  22;
   static const int  SerialRun3        =  23;
   static const int  SerialRun4        =  24;
   static const int  FileInit1         =  25;
   static const int  FileInit2         =  26;
   static const int  FileInit3         =  27;
   static const int  FileInit4         =  28;
   static const int  FileRun1          =  29;
   static const int  FileRun2          =  30;
   static const int  FileRun3          =  31;
   static const int  FileRun4          =  32;
   static const int  FileError1        =  33;
   static const int  FileError2        =  34;
   static const int  FileError3        =  35;
   static const int  FileError4        =  36;
   static const int  ThreadInit1       =  37;
   static const int  ThreadInit2       =  38;
   static const int  ThreadInit3       =  39;
   static const int  ThreadInit4       =  40;
   static const int  ThreadRun1        =  41;
   static const int  ThreadRun2        =  42;
   static const int  ThreadRun3        =  43;
   static const int  ThreadRun4        =  44;
   static const int  ProcInit1         =  45;
   static const int  ProcInit2         =  46;
   static const int  ProcInit3         =  47;
   static const int  ProcInit4         =  48;
   static const int  ProcRun1          =  49;
   static const int  ProcRun2          =  50;
   static const int  ProcRun3          =  51;
   static const int  ProcRun4          =  52;
   static const int  PointInit1        =  53;
   static const int  PointInit2        =  54;
   static const int  PointInit3        =  55;
   static const int  PointInit4        =  56;
   static const int  PointRun1         =  57;
   static const int  PointRun2         =  58;
   static const int  PointRun3         =  59;
   static const int  PointRun4         =  60;
   static const int  QCallInit1        =  61;
   static const int  QCallInit2        =  62;
   static const int  QCallInit3        =  63;
   static const int  QCallInit4        =  64;
   static const int  QCallRun1         =  65;
   static const int  QCallRun2         =  66;
   static const int  QCallRun3         =  67;
   static const int  QCallRun4         =  68;
   static const int  CalcInit1         =  69;
   static const int  CalcInit2         =  70;
   static const int  CalcInit3         =  71;
   static const int  CalcInit4         =  72;
   static const int  CalcRun1          =  73;
   static const int  CalcRun2          =  74;
   static const int  CalcRun3          =  75;
   static const int  CalcRun4          =  76;
   static const int  CalcError1        =  77;
   static const int  CalcError2        =  78;
   static const int  CalcError3        =  79;
   static const int  CalcError4        =  80;
   static const int  MsgRx1            =  81;
   static const int  MsgRx2            =  82;
   static const int  MsgRx3            =  83;
   static const int  MsgRx4            =  84;
   static const int  ViewInit1         =  85; 
   static const int  ViewInit2         =  86;
   static const int  ViewInit3         =  87;
   static const int  ViewInit4         =  88;
   static const int  ViewRun1          =  89;
   static const int  ViewRun2          =  90;
   static const int  ViewRun3          =  91;
   static const int  ViewRun4          =  92;
   static const int  CamInit1          =  93;
   static const int  CamInit2          =  94;
   static const int  CamInit3          =  95;
   static const int  CamInit4          =  96;
   static const int  CamRun1           =  97;
   static const int  CamRun2           =  98;
   static const int  CamRun3           =  99;
   static const int  CamRun4           = 100;
   static const int  CamError1         = 101;
   static const int  CamError2         = 102;
   static const int  CamError3         = 103;
   static const int  CamError4         = 104;
   static const int  SensorInit1       = 105;
   static const int  SensorInit2       = 106;
   static const int  SensorInit3       = 107;
   static const int  SensorInit4       = 108;
   static const int  SensorRun1        = 109;
   static const int  SensorRun2        = 110;
   static const int  SensorRun3        = 111;
   static const int  SensorRun4        = 112;
   static const int  SensorError1      = 113;
   static const int  SensorError2      = 114;
   static const int  SensorError3      = 115;
   static const int  SensorError4      = 116;
   static const int  DspInit1          = 117;
   static const int  DspInit2          = 118;
   static const int  DspInit3          = 119;
   static const int  DspInit4          = 120;
   static const int  DspRun1           = 121;
   static const int  DspRun2           = 122;
   static const int  DspRun3           = 123;
   static const int  DspRun4           = 124;
   static const int  DspError1         = 125;
   static const int  DspError2         = 126;
   static const int  DspError3         = 127;
   static const int  DspError4         = 128;

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

