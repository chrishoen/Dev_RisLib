#pragma once

/*==============================================================================
Serial port class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <windows.h>

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

static const int cSerialRetSuccess   =  1;  
static const int cSerialRetEmpty     =  0;
static const int cSerialRetError     = -1;
static const int cSerialRetTimeout   = -2;
static const int cSerialRetAbort     = -3;
static const int cSerialRetDataError = -4;

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This class encapsulates a serial port. It provides the functionality
// to configure, open, and close serial ports. It provides for the
// transmission and reception of raw bytes. It is used to support USB and
// UART serial communication.

class SerialPort 
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Settings.
   SerialSettings mSettings;

   // True if open.
   bool mOpenFlag;

   // True if open and valid.
   bool mValidFlag;

   // True if a receive abort is requested.
   bool mAbortFlag;

   // Program trace index.
   int mTI;

   // Show counts.
   int mOpenErrorShowCount;
   int mCloseErrorShowCount;

   // Metrics.
   long long mRxByteCount;
   long long mTxByteCount;
   int mPortErrorCount;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members. Windows specific.

   HANDLE mPortHandle;
   HANDLE mReadCompletion;
   HANDLE mWriteCompletion;
   HANDLE mCommCompletion;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   SerialPort();
  ~SerialPort();

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

   // Flush serial port buffers.
   void doFlush();

   // Drain serial port buffers.
   void doDrain();

   // Suspend serial port transmits and receives.
   void doSuspend();

   // Resume serial port transmits and receives.
   void doResume();

   // Return the number of bytes that are available to receive.
   int doGetAvailableReceiveBytes();

   // Abort a pending serial port receive.
   void doAbort();

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
   // been received. Copy the bytes into the pointer argument.
   // Return the number of bytes received or a negative error code. 
   int doReceiveAnyBytes(char* aBytes, int aMaxNumBytes);

   // Receive a requested number of bytes. Block until all of the bytes
   // have been received. Copy the bytes into the pointer argument.
   // Return the number of bytes received or a negative error code.
   int doReceiveAllBytes(char* aBytes, int aRequestBytes);

   // Receive one byte. Block until the byte has been received.
   // Copy the byte into the pointer argument.
   // Return one or a negative error code. 
   int doReceiveOneByte(char* aByte);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Windows only.

   // Receive a requested number of bytes. Block until all of the bytes
   // have been received. Copy the bytes into the pointer argument.
   // Return the number of bytes received or a negative error code.
   //
   // If normal then call the first one.
   // If bluetooth then call the seconds one. 
   int doReceiveAllBytes1(char* aBytes, int aRequestBytes);
   int doReceiveAllBytes2(char* aBytes, int aRequestBytes);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
