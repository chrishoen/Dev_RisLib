#ifndef _RISSERIALPORT_H_
#define _RISSERIALPORT_H_

//******************************************************************************
//******************************************************************************
//******************************************************************************
/* -------------------------------------------------------------------------- */
#include <Windows.h>

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

   class SerialPort 
{
public:

   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   // Members

   // Serial port handle
   HANDLE mPortHandle;

   // COM1 = 1
   int  mPortNumber;

   // "9600,N,8,1"
   char mPortSetup[16];

   // Validity
   bool mValidFlag;

   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   // Constructor
   // SerialPort* example = new SerialPort();
   // example->doOpen(1,"9600,N,8,1",2000);

   SerialPort();
  ~SerialPort(void);

   // Open/Close serial port
   // PortNumber: COM1==1
   // PortSetup:  "9600,N,8,1"
   // RxTimeout:  Timeout for all receives, milliseconds, 0==no timeout
   void doOpen(int aPortNumber,char* aPortSetup,int aRxTimeout);
   void doClose();
   void doPurge();

   // True if open and valid
   bool isValid();

   //---------------------------------------------------------------------------
   // Return codes, if retval >=0 then no error and retval is number of bytes
   // that were transferred. If retval < 0 then use these return codes

   static const int cRetCodeError   = -1;
   static const int cRetCodeTimeout = -2;

   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   // Send

   // Send data, fixed number of bytes.
   int  doSendBytes(char *aData, int aNumBytes);

   // Send data, null terminated.
   int  doSendZString(char *aData);

   // Send data, one byte.
   int  doSendOne(char aData);

   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   // Receive

   // Receive data, fixed number of bytes.
   int  doReceiveBytes (char *aData, int aNumBytes);

   // Receive data, terminated with CR/LF.
   int  doReceiveUntilCRLF (char *aData, int aMaxNumBytes);

   // Receive data, terminated with CR.
   int  doReceiveUntilCR (char *aData, int aMaxNumBytes);

   // Receive one byte.
   int  doReceiveOne(char *aData);

};
//******************************************************************************
}//namespace
#endif
