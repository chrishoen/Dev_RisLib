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

int SerialStringPort::doSendString(const char* aString)
{
   // Guard.
   if (!BaseClass::mValidFlag) return false;
   int tRet = 0;

   // Get the string length.
   int tLength = (int)strlen(aString);
   if (tLength > mBufferSize - 1) tLength = mBufferSize - 1;

   // Copy the string to the transmit buffer.
   strncpy(mTxBuffer, aString, (size_t)tLength);

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

   // Metrics.
   mTxCount++;

   // Transmit the buffer.
   return BaseClass::doSendBytes(mTxBuffer, tLength);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive a message from the serial port with a blocking read call into a
// byte buffer and extract a message from the byte buffer. Return the
// message and true if successful. As part of the termination process,
// returning false means that the serial port was closed or that there was
// an error.

int SerialStringPort::doReceiveString (char* aString, int aMaxSize)
{
   // Guard.
   if (!BaseClass::mValidFlag) return false;
   int tRet = 0;

   // Termination character.
   char tTermChar = 0;
   switch (mRxTermMode)
   {
   case cSerialTermMode_Null: tTermChar = 0; break;
   case cSerialTermMode_LF: tTermChar = cLF; break;
   case cSerialTermMode_CR: tTermChar = cCR; break;
   case cSerialTermMode_CRLF: tTermChar = cLF; break;
   default: break;
   }

   // Loop to receive one byte until a termination byte is 
   // encountered or the max size is reached.
   int tIndex = 0;
   while (true)
   {
      // Receive one byte. Block until the byte has been received. Return
      // one or zero or a negative error code. Copy the byte into the
      // pointer argument.
      char tRxChar = 0;
      tRet = BaseClass::doReceiveOneByte(&tRxChar);

      // Test the return code.
      if (tRet != 1) return tRet;

      // Test for termination byte.
      if (tRxChar == tTermChar)
      {
         tIndex++;
         break;
      }
      else
      {
         aString[tIndex++] = tRxChar;
         if (tIndex == aMaxSize - 1)
         {
            break;
         }
      }
   }

   // Null terminate the string.
   aString[tIndex] = 0;

   // If this was a CRLF then trim the CR.
   if (aString[tIndex] == cCR)
   {
      aString[--tIndex] = 0;
   }

   // Metrics.
   mRxCount++;

   // Return the number of bytes received, excluding the termination.
   return tIndex;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

}//namespace

