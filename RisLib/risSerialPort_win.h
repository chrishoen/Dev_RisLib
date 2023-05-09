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
// Constants. Windows specific.

// Comm event mask. Passed to SetCommMask;
static const DWORD cCommEvtMask = EV_DSR;

// Comm modem status mask. Passed to GetCommModemStatus.
static const DWORD cCommModemMask = MS_DSR_ON;

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

   // Handles.
   HANDLE mPortHandle;
   HANDLE mReadCompletion;
   HANDLE mWriteCompletion;
   HANDLE mCommCompletion;

   // Overlapped, persistent between read calls. 
   OVERLAPPED mReadOverlapped;
   OVERLAPPED mWriteOverlapped;
   OVERLAPPED mCommOverlapped;

   // If true then this is the first read after a successful open.
   bool mFirstReadFlag;

   // If true then an overlapped io is pending.
   bool mReadPending;
   bool mWritePending;
   bool mCommPending;

   // If true then receive calls use comm events and modem status.
   bool mUseModemStatus;

   // If true then the modem is valid.
   bool mModemValid;

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

   // Wait for the modem to be valid. Return true if successful.
   bool doWaitForModemValid();

   // Close the serial port.
   void doClose();

   // Flush serial port buffers.
   void doFlush();

   // Abort a pending serial port receive.
   void doAbort();

   // Receive a requested number of bytes. Block until all of the bytes
   // have been received. Copy the bytes into the pointer argument.
   // Return the number of bytes received or a negative error code.
   int doReceiveBytes(char* aBytes, int aNumBytes);

   // Send a requested number of bytes. Return the actual number of bytes
   // sent or a negative error code.
   int doSendBytes(const char *aBytes, int aNumBytes);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Windows specific.

   // Receive a requested number of bytes. Block until all of the bytes
   // have been received. Copy the bytes into the pointer argument.
   // Return the number of bytes received or a negative error code.
   //
   // If normal then call the first one.
   // If bluetooth then call the seconds one. 
   int doReceiveBytes1(char* aBytes, int aNumBytes);
   int doReceiveBytes2(char* aBytes, int aNumBytes);

   // Get the comm modem status. Return true if the modem is valid.
   bool doGetModemStatus();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
