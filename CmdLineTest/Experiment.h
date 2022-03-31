#pragma once

/*==============================================================================
Experimental code.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a class that contains parameter member variables. The values of

class Experiment
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants.

   static const int cMaxStringSize = 100;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members. 

   // Mode.
   int  mMode;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor,
   Experiment();

   // Test functions.
   void doRun1();
   void doRun2();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global instance.

#ifdef _EXPERIMENT_CPP_
   Experiment gExperiment;
#else
   extern Experiment gExperiment;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************