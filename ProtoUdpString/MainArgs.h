#pragma once

/*==============================================================================
main program arguments.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This parses main program arguments into some member variables.
// 

class MainArgs
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Peer identifier.
   int mPeerIdent;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   MainArgs();
   void reset();

   // Parse the main program arguments into member variables.
   void initialize(int argc, char** argv);

   // Helpers.
   void show();
   void help();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global instance.

#ifdef _MAINARGS_CPP_
   MainArgs gMainArgs;
#else
   extern MainArgs gMainArgs;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
