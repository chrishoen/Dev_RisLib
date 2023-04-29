#pragma once

/*==============================================================================
serial string port class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsSynch.h"
#include "risSerialSettings.h"
#include "risSerialPort.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Serial string port. This class encapsulates a serial port that operates
// on strings that use termination bytes (NULL, CR, LF, CRLF).
// 
// It inherits from SerialPort for serial port raw byte functionality and
// provides methods that can be used to send and receive strings.
//
// SEE END OF FILE NOTES FOR TERMINATION BYTE FORMATS.

class SerialStringPort : public SerialPort
{
private:
   typedef SerialPort BaseClass;
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants.

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Buffer size.
   int mBufferSize;

   // String termination modes. (NULL, CR \r 13, LF \n 10, CRLF \r\n).
   int mTxTermMode;
   int mRxTermMode;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Metrics.
   int mTxStringCount;
   int mRxStringCount;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   SerialStringPort(); 
  ~SerialStringPort(); 

   // Initialize and open the serial port.
   void initialize(SerialSettings& aSettings) override;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Copy a null terminated string into the transmit buffer and append
   // termination bytes. Send the transmit buffer via the serial port
   // base class. Return the total number of bytes transmitted, including
   // any termination bytes or a negative error code.
   int doSendString(const char* aString);

   // Enter a loop that receives one byte at a time via the serial port
   // base class and tests for a termination byte. Copy the received bytes
   // into the argument pointer. Terminate the loop when a termination byte
   // is received. Return the number of bytes received, excluding termination
   // bytes or return zero or a negative error code.
   int doReceiveString (char* aString, int aMaxSize);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

#if 0

ON THE TRANSMIT SIDE doSendString ***************************************

   input null terminated string "ABCD"
   input strlen      4
   input memory size 5
   input memory bytes 'A', 'B', 'C', 'D', 0

   For termination NULL

      transmit size 5
      transmit bytes 'A', 'B', 'C', 'D', 0

   For termination CR

      transmit size 5
      transmit bytes 'A', 'B', 'C', 'D', CR

   For termination LF

      transmit size 5
      transmit bytes 'A', 'B', 'C', 'D', LF

   For termination CRLF

      transmit size 6
      transmit bytes 'A', 'B', 'C', 'D', CR, LF

ON THE RECEIVE SIDE doReceiveString***************************************

   output null terminated string "ABCD"
   output strlen      4
   output memory size 5
   output memory bytes 'A', 'B', 'C', 'D', 0

   returns     4

#endif
