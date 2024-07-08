#pragma once

/*==============================================================================
UDP string socket.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsSynch.h"
#include "risNetSettings.h"
#include "risSockets.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Udp string socket. This class encapsulates a udp socket that
// sends and receives strings.

class UdpStringSocket : public Sockets::BaseUdpSocket
{
private:
   typedef Sockets::BaseUdpSocket BaseClass;
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants.

   // String size.
   static const int cStringSize = 1400;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Settings.
   Settings mSettings;

   // The recvfrom address is stored here.
   Sockets::SocketAddress mFromAddress;

   // True if the socket is valid.
   bool mValidFlag;

   // Received string buffer.
   char mRxString[cStringSize];

   // Metrics.
   int mTxLength;
   int mRxLength;
   int mTxCount;
   int mRxCount;

   // Program trace index.
   int mTI;

   // If true then disable status prints.
   bool mPrintDisable;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   UdpStringSocket();

   // Initialize variables.
   void initialize(Settings* aSettings);

   // Configure the socket. This does socket and bind calls.
   void configure();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Receive a string into the allocated receive buffer.
   // It returns true if successful.
   // The recvfrom address is stored in mFromAddress.
   bool doRecvString ();

   // Send a string over the socket via a blocking send call.
   // It returns true if successful.
   // It is protected by the transmit mutex.
   bool doSendString(const char* aString);
};
//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

