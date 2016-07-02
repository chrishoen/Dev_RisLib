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
   // Constructor
   // SerialPort* example = new SerialPort();
   // example->doOpen(1,"9600,N,8,1");

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

   static const int cRetCodeError   = -1;
   static const int cRetCodeTimeout = -2;

   //---------------------------------------------------------------------------
   // Send

   // Send data, fixed number of bytes.
   int  doSend(char *aData, int aNumBytes);

   // Send data, null terminated.
   int  doSend(char *aData);

   // Send data, one byte.
   int  doSendOne(char aData);

   //---------------------------------------------------------------------------
   // Receive

   // Receive data, fixed number of bytes.
   int  doReceiveData (char *aData, int aNumBytes, int aTimeout);

   // Receive data, terminated with cr/lf.
   int  doReceiveUntilCRLF (char *aData, int aMaxNumBytes,int aTimeout);

   // Receive data, terminated with cr.
   int  doReceiveUntilCR (char *aData, int aMaxNumBytes,int aTimeout);

   // Receive one byte.
   int  doReceiveOne(char *aData, int aTimeout);

   //---------------------------------------------------------------------------
   // Support

   void doPurge();

   //---------------------------------------------------------------------------
   // Members

   // Serial port handle
   HANDLE   mPortHandle;

   int  mPortNumber;
   char mPortSetup[16];

   bool mValidFlag;
};
//******************************************************************************
}//namespace
#endif
