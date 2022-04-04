#pragma once

/*==============================================================================
Program trace buffer facility interface.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
// These are aliase functioins for the trace buffer code. Refer there
// for an explanation.

namespace Trc
{
   // Reset the trace facility.
   void reset();

   // Start a trace on a buffer.
   void start(int aBufNum);

   // Stop a trace on a buffer.
   void stop(int aBufNum);

   // Resume a stopped trace on a buffer.
   void resume(int aBufNum);

   // Write to a trace buffer.
   void write(int aBufNum, const char* aFormat, ...);

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

