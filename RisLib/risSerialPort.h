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
// This class encapsulates a serial port. It provides functions to
// open, close, and purge a serial port. It provides functions to send and
// receive bytes via a serial port. The send and receive functions are
// blocking.

class SerialPort 
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants.

   // Return codes, if retval >=0 then no error and retval is number of bytes
   // that were transferred. If retval < 0 then use these return codes.
   static const int cRetCodeError   = -1;
   static const int cRetCodeTimeout = -2;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Settings.
   SerialSettings mSettings;

   // Validity.
   bool mValidFlag;

   // If true then a terminations is requested.
   bool mTerminateFlag;

   // Portable specifics.
   class Specific;
   Specific* mSpecific;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   SerialPort();
  ~SerialPort(void);

   // Initialize with settings.
   virtual void initialize(SerialSettings& aSettings);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Open/Close serial port.
   bool doOpen();
   void doClose();
   void doPurge();

   // True if open and valid.
   bool isValid();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Send data, fixed number of bytes.
   int  doSendBytes(const char *aData, int aNumBytes);

   // Send a null terminated string, append an end of line LF (\n,10) or
   // CRLF (\r\n,13,10)
   int  doSendLine(const char *aData);

   // Send data, one byte.
   int  doSendOne(char aData);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Receive data, fixed number of bytes.
   int doReceiveBytes(char *aData, int aNumBytes);

   // Receive a string, terminated with end of line LF (\n,10) or 
   // CRLF (\r\n,13,10). Trims the  terminator and returns a null terminated
   // string. Termination mode is determined by settings.
   int doReceiveLine(char *aData, int aMaxNumBytes);

   // Receive a string, terminated with end of line LF (\n,10). 
   int doReceiveLineLF(char* aData, int aMaxNumBytes);

   // Receive a string, terminated with end of line CRLF (\r\n,13,10). 
   int doReceiveLineCRLF(char* aData, int aMaxNumBytes);

   // Receive one byte.
   int doReceiveOne(char *aData);

   // Return the number of bytes that are available to receive.
   int getAvailableReceiveBytes();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
