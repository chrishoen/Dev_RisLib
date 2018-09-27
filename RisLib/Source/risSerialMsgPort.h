#pragma once

/*==============================================================================
Byte content message serial port class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risByteContent.h"
#include "risByteMsgMonkey.h"
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
// Message serial port, blocking.
//
// It exchanges byte content messages (send and recv) via a serial port.
//
// It inherits from SerialPort for serial functionality and
// provides methods that can be used to transport messages.
//
// Messages are based on the ByteContent message encapsulation scheme.

class SerialMsgPort : public SerialPort
{
public:
   typedef SerialPort BaseClass;

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

   // These are transmit and receive memory. They are allocated when the 
   // message port is opened.
   char* mTxMemory;
   char* mRxMemory;

   // Size of allocated memory.
   int mMemorySize;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // This a  message monkey that is used to get details about  a message from
   // a message header that is contained in a byte buffer. For receive, the 
   // message monkey allows the doRecvMsg method to receive and extract a
   // message from a byte buffer without the having the message code visible
   // to it. For transmit, message monkey allows the doSendMsg method to set
   // header data before the message is sent.
   BaseMsgMonkey* mMonkey;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Header read state. This determines if the header is read one byte at 
   // a time and the header buffer is used to detect a header or if it is
   // read all bytes at once.
   int mHeaderReadState;

   // Message header length.
   int mHeaderLength;

   // Serial header buffer. This is used to detect a header and synchronize
   // to it.
   SerialHeaderBuffer mHeaderBuffer;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Metrics.
   int mTxMsgCount;
   int mRxMsgCount;
   int mHeaderAllCount;
   int mHeaderOneCount;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Infastrcture.

   // Constructor.
   SerialMsgPort(); 
  ~SerialMsgPort(); 

   // Initialize and open the serial port.
   void initialize(SerialSettings& aSettings) override;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Copy a message into a byte buffer and send the byte buffer via the
   // serial port.
   // It returns true if successful.
   // It is protected by the transmit mutex.
   bool doSendMsg(ByteContent*  aTxMsg);

   // Receive data from the serial port into a byte buffer and then extract
   // a message from the byte buffer. Return the message and true if
   // successful. As part of the termination process, returning false means
   // that the serial port was closed or that there was an error.
   bool doReceiveMsg (ByteContent*& aRxMsg);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

