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
   mBufferSize = 0;
   mTxTermMode = 0;
   mRxTermMode = 0;
   mTxStringCount = 0;
   mRxStringCount = 0;
}

SerialStringPort::~SerialStringPort()
{
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

   // Initialize member variables.
   mTxStringCount = 0;
   mRxStringCount = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Enter a loop that receives one byte at a time via the serial port
// base class and tests for a termination byte. Copy the received bytes
// into the argument pointer. Terminate the loop when a termination byte
// is received. Return the number of bytes received, excluding termination
// bytes or return zero or a negative error code.

int SerialStringPort::doReceiveString (char* aString, int aMaxSize)
{
   // Guard.
   if (!BaseClass::mValidFlag) return false;
   int tRet = 0;

   // Termination character.
   char tTermChar = 0;
   switch (mRxTermMode)
   {
   case cSerialTermMode_NULL: tTermChar = 0; break;
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
      tRet = BaseClass::doReceiveBytes(&tRxChar, 1);

      // Test the return code.
      if (tRet != 1) return tRet;

      // Test for termination byte.
      if (tRxChar == tTermChar)
      {
         // The termination byte will not be stored.
         break;
      }
      else
      {
         // Store the byte.
         aString[tIndex++] = tRxChar;

         // Test for overrun.
         if (tIndex == aMaxSize - 1)
         {
            break;
         }
      }
   }

   // The index points to the byte after the last byte in the string.
   // Null terminate the string by replacing the byte after the last 
   // byt with zero. Then the index is also the strlen of the string 
   // returned to the caller. 
   aString[tIndex] = 0;

   // If this was a CRLF then trim the CR, which is the last byte
   // in the string.
   if (aString[tIndex - 1] == cCR)
   {
      aString[--tIndex] = 0;
   }

   // Metrics.
   mRxStringCount++;

   // Return the strlen of the string returned to the caller, 
   // The returned string does not contain termination bytes.
   return tIndex;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Copy a null terminated string into the transmit buffer and append
// termination bytes. Send the transmit buffer via the serial port
// base class. Return the total number of bytes transmitted, including
// any termination bytes or a negative error code.

int SerialStringPort::doSendString(const char* aString)
{
   // Guard.
   if (!BaseClass::mValidFlag) return false;
   int tRet = 0;

   // Get the string length.
   int tLength = (int)strlen(aString);
   if (tLength > mBufferSize - 1) tLength = mBufferSize - 1;

   // Allocate a transmit buffer. 
   char* tTxBuffer = (char*)malloc(mBufferSize);
   if (tTxBuffer == 0) return 0;

   // Copy the string to the transmit buffer.
   strcpy(tTxBuffer, aString);

   // Append termination characters.
   switch (mTxTermMode)
   {
   case cSerialTermMode_NULL:
      // Transmit the string + null.
      tLength++;
      break;
   case cSerialTermMode_LF:
      // Transmit the string + LF.
      tTxBuffer[tLength++] = cLF;
      break;
   case cSerialTermMode_CR:
      // Transmit the string + CR.
      tTxBuffer[tLength++] = cCR;
      break;
   case cSerialTermMode_CRLF:
      // Transmit the string + CRLF.
      tTxBuffer[tLength++] = cCR;
      tTxBuffer[tLength++] = cLF;
      break;
   default:
      break;
   }

   // Metrics.
   mTxStringCount++;

   // Transmit the buffer.
   tRet = BaseClass::doSendBytes(tTxBuffer, tLength);

   // Deallocate the buffer.
   free(tTxBuffer);

   // Done.
   return tRet;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

}//namespace

