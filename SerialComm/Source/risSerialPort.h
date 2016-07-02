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
class SerialPort 
{
public:
   //---------------------------------------------------------------------------
   // Constructor creates object, doesn't open serial port
   // SerialPort* example = new SerialPort(1,"9600,N,8,1");

   SerialPort();
  ~SerialPort(void);

   // Open/Close serial port
   void doOpen(int aPortNumber,char* aPortSetup);
   void doClose();

   // True if open and valid
   bool isValid();

   //---------------------------------------------------------------------------
   // Return codes, if retval >=0 then no error and retval is number of bytes
   // that were transferred. If retval < 0 then use these return codes
   enum {RetCodeError   = -1};
   enum {RetCodeTimeout = -2};

   //---------------------------------------------------------------------------
   // Send

   // Send data, fixed size
   int  doSend(char *aData, int aSize);

   // Send data, null terminated
   int  doSend(char *aData);

   // Send data, one byte
   int  doSendOne(char aData);

   //---------------------------------------------------------------------------
   // Receive data, fixed size
   int  doReceiveData (char *aData, int aSize, int aTimeout);
   // Receive data, terminated with cr/lf
   int  doReceiveUntilCRLF (char *aData, int aMaxSize,int aTimeout);
   // Receive data, terminated with cr
   int  doReceiveUntilCR (char *aData, int aMaxSize,int aTimeout);
   // Receive one byte
   int  doReceiveOne(char *aData, int aTimeout);

   //---------------------------------------------------------------------------
   // Support
   void doPurge();

   //---------------------------------------------------------------------------
   // Members

   // Serial port handle
   HANDLE   mHandle;

   int  mPortNumber;
   char mPortSetup[100];

   bool mValidFlag;
};
//******************************************************************************
}//namespace
#endif
