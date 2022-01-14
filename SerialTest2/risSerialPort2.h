#pragma once

/*==============================================================================
Serial port class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risSerialSettings.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constants.

// Serial port return codes, if retval >=0 then no error and retval is
// number of bytes that were transferred. If retval < 0 then use these
// return codes.

static const int cSerialRetError = -1;
static const int cSerialRetTimeout = -2;
static const int cSerialRetAbort = -3;

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This class encapsulates a serial port. It provides the functionality
// to configure, open, and close serial ports. It provides for the
// transmission and reception of raw bytes. It is used to support USB and
// UART serial communication.

class SerialPort2 
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Settings.
   SerialSettings mSettings;

   // True if open and valid.
   bool mValidFlag;

   // True if a receive abort is requested.
   bool mAbortFlag;

   // Portable specifics.
   class Specific;
   Specific* mSpecific;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   SerialPort2();
  ~SerialPort2();

   // Initialize with settings.
   virtual void initialize(SerialSettings& aSettings);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Open the serial port and configure with the settings.
   bool doOpen();

   // Close the serial port.
   void doClose();

   // Abort a pending serial port receive.
   void doAbort();

   // Flush serial port buffers.
   void doFlush();

   // Return the number of bytes that are available to read.
   int doGetAvailableReadBytes();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Send a fixed number of bytes. Return the actual number of bytes
   // sent or a negative error code.
   int  doSendBytes(const char *aBytes, int aNumBytes);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Receive any available bytes. Block until at least one byte has
   // been received. Return the number of bytes received or a negative
   // error code. Copy the bytes into the pointer argument.
   int doReceiveAnyBytes(char *aBytes, int aMaxNumBytes);

   // Receive a requested number of bytes. Block until all of the bytes
   // have been received. Return the number of bytes received or a
   // negative error code. Copy the bytes into the pointer argument.
   int doReceiveAllBytes(char* aBytes, int aRequestBytes);

   // Receive one byte. Block until the byte has been received. Return
   // one or zero or a negative error code. Copy the byte into the
   // pointer argument.
   int doReceiveOneByte(char* aByte);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
