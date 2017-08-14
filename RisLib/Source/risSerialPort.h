#pragma once

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <Windows.h>

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This class encapsulates a serial port.
// SerialPort* example = new SerialPort();
// example->doOpen(1,"9600,N,8,1",2000);

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

   // Serial port handle.
   HANDLE mPortHandle;

   // Serial port event handles.
   HANDLE mRxEventHandle;
   HANDLE mTxEventHandle;

   // COM1 = 1
   int  mPortNumber;

   // "9600,N,8,1"
   char mPortSetup[16];

   // Validity.
   bool mValidFlag;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Infastrcture.

   // Constructor.
   SerialPort();
  ~SerialPort(void);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.
   // Open/Close serial port
   // PortNumber: COM1==1
   // PortSetup:  "9600,N,8,1"
   // RxTimeout:  Timeout for all receives, milliseconds, 0==no timeout
   void doOpen(int aPortNumber,char* aPortSetup,int aRxTimeout);
   void doClose();
   void doPurge();

   // True if open and valid
   bool isValid();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Send data, fixed number of bytes.
   int  doSendBytes(char *aData, int aNumBytes);

   // Send data, null terminated.
   int  doSendZString(char *aData);

   // Send data, one byte.
   int  doSendOne(char aData);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Receive data, fixed number of bytes.
   int  doReceiveBytes(char *aData, int aNumBytes);
   int  doReceiveBytes22(char *aData, int aNumBytes);

   // Receive data, terminated with CR/LF.
   int  doReceiveUntilCRLF (char *aData, int aMaxNumBytes);

   // Receive data, terminated with CR.
   int  doReceiveUntilCR (char *aData, int aMaxNumBytes);

   // Receive one byte.
   int  doReceiveOne(char *aData);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
