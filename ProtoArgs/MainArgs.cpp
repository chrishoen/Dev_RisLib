//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#define  _MAINARGS_CPP_
#include "MainArgs.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

MainArgs::MainArgs()
{
   reset();
}

void MainArgs::reset()
{
   mCLIEnable = false;
}
   
void MainArgs::show()
{
   printf("MainArgs*************************\n");
   printf("CLIEnable       %s\n", my_string_from_bool(mCLIEnable));
}

void MainArgs::help()
{
   printf("MainArgs\n");
   printf("-cli   enable command line interface\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Parse the main program arguments into member variables.

void MainArgs::initialize(int argc, char** argv)
{
   reset();

   if (argc >= 2)
   {
      if (strcmp(argv[1], "-help") == 0) help();
      if (strcmp(argv[1], "-cli") == 0) mCLIEnable = true;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
