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
   mClientIdent = 1;
}
   
void MainArgs::show()
{
   printf("MainArgs*************************\n");
   printf("ClientIdent       %d\n", mClientIdent);
}

void MainArgs::help()
{
   printf("MainArgs\n");
   printf("-client1    client identifier is 1\n");
   printf("-client2    client identifier is 2\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Parse the main program arguments into member variables.

void MainArgs::initialize(int argc, char** argv)
{
   reset();

   for (int i = 1; i < argc; i++)
   {
      //printf("argv[%d] %s\n", i, argv[i]);
      if (strcmp(argv[i], "-help") == 0) help();
      if (strcmp(argv[i], "-client1") == 0) mClientIdent = 1;
      if (strcmp(argv[i], "-client2") == 0) mClientIdent = 2;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
