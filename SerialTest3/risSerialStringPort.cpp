/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "my_functions.h"
#include "prnPrint.h"

#include "risSerialStringPort.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constants.

static const int cLF = 10; // LF   (\n,10)
static const int cCR = 13; // CR   (\r,13)

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

SerialStringPort::SerialStringPort()
{
   mTxBuffer = 0;
   mRxBuffer = 0;
   mBufferSize = 0;
   mTxCount = 0;
   mRxCount = 0;
}

SerialStringPort::~SerialStringPort()
{
   if (mTxBuffer) free(mTxBuffer);
   if (mRxBuffer) free(mRxBuffer);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize the serial port variables.

void SerialStringPort::initialize(SerialSettings& aSettings)
{
   // Initialize the base class.
   BaseClass::initialize(aSettings);

   // Copy settings into member variables.
   mTxTermMode = mSettings.mTxTermMode;
   mRxTermMode = mSettings.mRxTermMode;

   // Allocate memory for byte buffers.
   mBufferSize = 16*1024;
   mTxBuffer = (char*)malloc(mBufferSize);
   mRxBuffer = (char*)malloc(mBufferSize);

   // Initialize member variables.
   mTxCount = 0;
   mRxCount = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Copy a null terminated string into the transmit buffer and append
// termination characters. Send the transmit buffer to the serial port
// with a base class write call. Return true if successful.

bool SerialStringPort::doSendString(const char* aString)
{
   // Guard.
   if (!BaseClass::mValidFlag) return false;
   int tRet = 0;

   // Copy the string to the transmit buffer.
   int tLength = strlen(aString);
   strcpy(mTxBuffer, aString);

   // Append termination characters.
   switch (mTxTermMode)
   {
   case cSerialTermMode_Null:
      // Transmit the string + null.
      tLength++;
      break;
   case cSerialTermMode_LF:
      // Transmit the string + LF.
      mTxBuffer[tLength++] = cLF;
      break;
   case cSerialTermMode_CR:
      // Transmit the string + CR.
      mTxBuffer[tLength++] = cCR;
      break;
   case cSerialTermMode_CRLF:
      // Transmit the string + CRLF.
      mTxBuffer[tLength++] = cCR;
      mTxBuffer[tLength++] = cLF;
      break;
   default:
      break;
   }

   // Transmit the buffer.
   tRet = BaseClass::doSendBytes(mTxBuffer, tLength);

   // Test for errors.
   if (tRet<0)
   {
      printf("ERROR SerialStringPort::doSendString FAIL\n");
      return false;
   }

   // Done.
   mTxCount++;
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive a message from the serial port with a blocking read call into a
// byte buffer and extract a message from the byte buffer. Return the
// message and true if successful. As part of the termination process,
// returning false means that the serial port was closed or that there was
// an error.

bool SerialStringPort::doReceiveString (char* aString)
{
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

}//namespace

