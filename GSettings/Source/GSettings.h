#ifndef _GSETTINGS_H_
#define _GSETTINGS_H_

/*==============================================================================

Global settings file global object.

This encapsulates data contained in the global settings file. For each entry
in the file, there is a corresponding variable. The following classes are
defined:

Network_Settings -- Network settings, IP addresses and port numbers

PLT_Settings     -- CSC section settings
DIAG_Settings    -- CSC section settings
SCP_Settings     -- CSC section settings
GGI_Settings     -- CSC section settings
RTC_Settings     -- CSC section settings
GSettings        -- Global settings

The CSC settings classes contain members that are read from the CSC sections
of the settings file. They contain members that are specific to individual 
CSCs.

GSettings contains members that are common to all CSCs and contains members 
that instantiate the CSC classes.

gGSettings is a global object that is an instance of GSettings. It is defined
at the bottom of the file.

Here's a typical use case, where EncoderOffset was read in from the PLT 
section of the settings file:

double encoder_offset = gGSettings.mPLT_Settings.mEncoderOffset;

==============================================================================*/
#include "risCmdLineFile.h"

#include "System_Settings.h"
#include "Network_Settings.h"
#include "Transforms_Settings.h"
#include "Fpga_Settings.h"
#include "PLT_Settings.h"
#include "SCP_Settings.h"
#include "DIAG_Settings.h"
#include "GGI_Settings.h"
#include "RTC_Settings.h"
#include "DevIO_Settings.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

class GSettings : public Ris::BaseCmdLineExec
{
public:

   //---------------------------------------------------------------------------
   // True if initialize and read was successful

   bool mValidFlag;

   //---------------------------------------------------------------------------
   // Common settings members. These are read as normal settings variables from
   // the settings file. They are common to all of the CSCs.

   // Integrity checks
   int   mCode1;
   int   mCode2;

   // Version
   int   mVersion;

   // If true then show is called at initialization
   bool  mShowAtInitialize;

   //---------------------------------------------------------------------------
   // CSC settings members. These are read from the CSC sections of the
   // settings file. For each CSC, there is one corresponding member that 
   // encapsulates the settings variables for the CSC. These are encapsulated 
   // in the classes defined above.

   System_Settings       mSystem;
   Network_Settings      mNetwork;
   Transforms_Settings   mTransforms;
   Fpga_Settings         mFpga;
   PLT_Settings          mPLT;
   SCP_Settings          mSCP;
   DIAG_Settings         mDIAG;
   GGI_Settings          mGGI;
   RTC_Settings          mRTC;
   DevIO_Settings        mDevIO;

   //---------------------------------------------------------------------------
   // Infrastucture. Constructor and such

   typedef Ris::BaseCmdLineExec BaseClass;
   GSettings();
   void show();

   //---------------------------------------------------------------------------
   // Execute. This is called for each line in the settings file that contains
   // a command for a common setting or a command that enters a sub executive 
   // that processes a CSC section.

   void execute(Ris::CmdLineCmd* aCmd);

   // Execute for specific commands. 
   void executeOnBegin(Ris::CmdLineCmd* aCmd);

   //---------------------------------------------------------------------------
   // Read from the settings file using an instance of CmdLineFile and this as
   // the executive.

   // This reads from the default settings directory with the default filename.
   void readFromDefaultFile();
   // This reads from the default settings directory with a given filename.
   void readFromFileName(char* aFileName=0);
   // This reads from the an absolute filepath
   void readFromFilePath(char* aFilePath);

};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global instance

#ifdef _GSETTINGS_CPP_
        GSettings       gGSettings;
#else
extern  GSettings       gGSettings;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Read only alias

inline GSettings const& getGSettings()
{
   GSettings const& constrefGSettings = gGSettings;
   return constrefGSettings;
}

//******************************************************************************

#endif

