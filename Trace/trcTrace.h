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

   // Allocate memory for a trace buffer. Set the initial write level.
   void create_buffer(int aTraceIndex, int aWriteLevel);

   // Initialize.
   void initialize();

   // Initialize.
   void finalize();

   // Start a trace on a buffer.
   void start(int aTraceIndex);

   // Stop a trace on a buffer.
   void stop(int aTraceIndex);

   // Suspend a trace on a buffer.
   void suspend(int aTraceIndex);

   // Resume a suspended trace on a buffer.
   void resume(int aTraceIndex);

   // Write to a trace buffer.
   void write(int aTraceIndex, int aLevel, const char* aFormat, ...);

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

