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
   void start(int aTraceIndex);

   // Stop a trace on a buffer.
   void stop(int aTraceIndex);

   // Resume a stopped trace on a buffer.
   void resume(int aTraceIndex);

   // Write to a trace buffer.
   void write(int aTraceIndex, int aLevel, const char* aFormat, ...);

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

