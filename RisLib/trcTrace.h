#pragma once

/*==============================================================================
Program trace buffer facility interface.
==============================================================================*/

#include <string>
#include <memory>

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
   void create_buffer(int aTraceIndex, int aWriteLevel, const char* aLabel);

   // Create a log file for a trace. Set the initial log level
   // for the trace.
   void create_log(
      int aTraceIndex, int aLogLevel, const char* aFilePath, bool aAppend = false);

   // Create a log file for a trace. Set the initial log level
   // for the trace.
   void create_log(
      int aTraceIndex, int aLogLevel, std::unique_ptr<std::string> aFilePath, bool aAppend = false);

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

   // Set default show trace index.
   void set_default_trace_index(int aTraceIndex);

   // Use absolute or program time for timetag.
   void use_absolute_timetag();
   void use_program_timetag();

   //******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

