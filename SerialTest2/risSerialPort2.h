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

// Serial por return codes, if retval >=0 then no error and retval is
// number of bytes that were transferred. If retval < 0 then use these
// return codes.

static const int cSerialRetError = -1;
static const int cSerialRetAbort = -2;
static const int cSerialRetTimeout = -3;

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This class encapsulates a serial port. It provides functions to
// open, close, and purge a serial port. It provides functions to send and
// receive bytes via a serial port. The send and receive functions are
// blocking.

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

   // Open the serial port with the settings.
   bool doOpen();

   // Close the serial port.
   void doClose();

   // Abort pending serial port receive.
   void doAbort();

   // Flush serial port buffers.
   void doFlush();

   // Return the number of bytes that are available to read.
   int doGetAvailableReceiveBytes();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Send a number of bytes, return the actual number of bytes sent.
   int  doSendBytes(const char *aBytes, int aNumBytes);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Read any available receive bytes. Block until at least one byte
   // has been received. Return the number of bytes read or a negative
   // error code. Copy the bytes into the pointer argument.
   int doReadAnyBytes(char *aBytes, int aMaxNumBytes);

   // Read a requested number of receive bytes. Block until all of the
   // bytes have been received and read. Return the number of bytes read
   // or a negative error code. Copy the bytes into the pointer argument.
   int doReadAllBytes(char* aBytes, int aRequestBytes);

   // Read one receive byte. Block until the byte has been received
   // and read. Return one or a negative error code. Copy the byte into
   // the pointer argument.
   int doReadOneByte(char* aByte);

};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
