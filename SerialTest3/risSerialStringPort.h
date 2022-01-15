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
// Serial string port. This class encapsulates a serial port that
// communicates strings.
//
// It exchanges strings (send and receive) via a serial port.
// It manages string terminations (NULL, CR, LF, CRLF)
// It manages transmit and receive buffers.
//
// It inherits from SerialPort for serial functionality and
// provides methods that can be used to send and receive strings.

class SerialStringPort : public SerialPort
{
public:
   typedef SerialPort BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants.

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Transmit and receive buffers. Allocated at initialization.
   char* mTxBuffer;
   char* mRxBuffer;

   // Buffer size.
   int mBufferSize;

   // String termination modes. (NULL, CR, LF, CRLF).
   int mTxTermMode;
   int mRxTermMode;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Metrics.
   int mTxCount;
   int mRxCount;

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

   // Copy a string into the transmit buffer and append termination
   // characters. Send the transmit buffer to the serial port with
   // a blocking write call. Return true if successful.
   bool doSendString(const char* aString);

   // Receive a message from the serial port with a blocking read call into a
   // byte buffer and extract a message from the byte buffer. Return the
   // message and true if successful. As part of the termination process,
   // returning false means that the serial port was closed or that there was
   // an error.
   bool doReceiveString (char* aString);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

