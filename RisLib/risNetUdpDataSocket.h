#pragma once

/*==============================================================================
UDP data socket.
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
// Udp data socket. This class encapsulates a udp socket that
// sends and receives data.

class UdpDataSocket : public Sockets::BaseUdpSocket
{
private:
   typedef Sockets::BaseUdpSocket BaseClass;
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants.

   // String size.
   static const int cBufferSize = 1472;

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

   // Metrics.
   int mTxMsgCount;
   int mRxMsgCount;
   long long mTxByteCount;
   long long mRxByteCount;
    
   // Program trace index.
   int mTI;

   // If true then disable status prints.
   bool mPrintDisable;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   UdpDataSocket();
   ~UdpDataSocket();

   // Initialize variables.
   void initialize(Settings* aSettings);

   // Configure the socket. This does socket and bind calls.
   void configure();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Receive a data into the allocated receive buffer.
   // It returns true if successful.
   // The recvfrom address is stored in mFromAddress.
   bool doRecvData (char* aData, int* aSize);

   // Send a data over the socket via a blocking send call.
   // It returns true if successful.
   // It is protected by the transmit mutex.
   bool doSendData(const char* aData, int aSize);
};
//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

