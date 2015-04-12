#ifndef _RISSERIALPORT_H_
#define _RISSERIALPORT_H_

//******************************************************************************
//******************************************************************************
//******************************************************************************
/* -------------------------------------------------------------------------- */
#include "risPortableTypes.h"

namespace Ris
{
namespace Serial
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
   void initialize(int aPortNumber,char* aPortSetup);
   void close();

   // True if open and valid
   bool isValid();

   //---------------------------------------------------------------------------
   // Return codes, if retval >=0 then no error and retval is number of bytes
   // that were transferred. If retval < 0 then use these return codes
   enum {RetCodeError   = -1};
   enum {RetCodeTimeout = -2};

   //---------------------------------------------------------------------------
   // Send data, fixed size
   int  doSend(char const *aData, int aSize);
   // Send data, null terminated
   int  doSend(char const *aData);

   //---------------------------------------------------------------------------
   // Receive data, fixed size
   int  receiveData (char *aData, int aSize, int aTimeout);
   // Receive data, terminated with cr/lf
   int  receiveUntilCrLf (char *aData, int aMaxSize,int aTimeout);
   // Receive one byte
   int  receiveOne(char *aData, int aTimeout);

   //---------------------------------------------------------------------------
   // Support
   void purge();

   //---------------------------------------------------------------------------
   // Members

   // Serial port handle
   Ris::HandleT   mHandle;

   int  mPortNumber;
   char mPortSetup[100];

   bool mValidFlag;
};
//******************************************************************************
}//namespace
}//namespace
#endif
