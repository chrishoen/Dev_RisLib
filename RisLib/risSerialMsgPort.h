#pragma once

/*==============================================================================
Message serial port class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risByteContent.h"
#include "risByteMsgMonkey.h"
#include "risThreadsSynch.h"
#include "risSerialSettings.h"
#include "risSerialHeaderBuffer.h"
#include "risSerialPort.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Serial message port. This class encapsulates a serial port that operates
// on messages that are based on the byte content message encapsulation
// scheme.
// 
// It inherits from SerialPort for serial port raw byte functionality and
// provides methods that can be used to send and receive messages.

class SerialMsgPort : public SerialPort
{
private:
   typedef SerialPort BaseClass;
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants.

   static const int cHeaderReadOne = 1;
   static const int cHeaderReadAll = 2;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Transmit and receive memory. Allocated at initialization.
   char* mRxMemory;

   // Size of allocated memory.
   int mMemorySize;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // This a message monkey that is used to manage the details about messages
   // and message headers while hiding the underlying specific message set code.
   // For received  messages, the message monkey allows the receive method to
   // extract message header details from a byte buffer and it allows it to 
   // then extract full messages from the byte buffer. For transmited messages,
   // the message monkey allows the send method to set header data before the
   // message is sent. A specific message monkey is provided by the parent 
   // thread at initialization.
   BaseMsgMonkey* mMsgMonkey;

   // Metrics.
   int mTxMsgCount;
   int mRxMsgCount;
   int mHeaderAllCount;
   int mHeaderOneCount;
   int mMsgErrorCount;
   int mMsgResyncCount;

   // Program trace index.
   int mTI;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Header read state. This determines if the header is read one byte at 
   // a time and the header buffer is used to detect a header or if it is
   // read all bytes at once. If the header is not synchronized then reads
   // are done one byte at a time until a header is detected. If the header
   // is synchronized then the entire header is read.
   int mHeaderReadState;

   // Message header length.
   int mHeaderLength;

   // Serial header buffer. This is used to detect a header and synchronize
   // to it.
   SerialHeaderBuffer mHeaderBuffer;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   SerialMsgPort(); 
  ~SerialMsgPort(); 

   // Initialize and open the serial port.
   void initialize(SerialSettings& aSettings) override;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Copy a message into a byte buffer and then send the byte buffer to the
   // serial port with a blocking write call. Delete the message when done.
   // Return the total number of bytes transmitted or a negative error code.
   int doSendMsg(ByteContent* aTxMsg);

   // Receive a message from the serial port with blocking read calls into a
   // byte buffer and extract the message from the byte buffer. If the message
   // cannot be extracted because the header is incorrect then enter a mode
   // to resync the header. Return the message and a status code. If the status
   // code is greater than zero then the receive was successful. If the status
   // code is less than or equal to zero then there was an abort or error.
   int doReceiveMsg (ByteContent*& aRxMsg);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

