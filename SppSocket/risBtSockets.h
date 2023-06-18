#pragma once
/*==============================================================================
Description:

Platform Sockets support classes

This is a set of classes that are wrappers for socket calls. They
hide the details of the parameters that are passed to the calls and
of the error codes that are returned. The layout of the classes is
based on the different types of sockets (udp, tcp server, tcp
client). Each socket class has methods that are aliases for
those socket calls that pertain to that type of socket. The socket
calls are not documented here, look in a sockets book or on the web.

Here are the classes:

BtSocketAddress is an ip address + a port number.
They are stored in host order.

BaseSocket is the socket base class. It contains a local and a host
address. Local is the machine that the socket is running on and Host
is the remote host machine that the socket is connected to.

UdpSocket is a udp datagram socket.

TcpServerHubSocket manages tcp socket session connections. Clients
connect to Hubs and Hubs spawn Nodes to handle the actual data
transfers.

TcpServerNodeSocket does the tcp send/recv transfers with a client.

TcpClientSocket connects to a Hub and send/recv with a node.
   

==============================================================================*/

#include <winsock2.h>
#include <ws2bth.h>


//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{
namespace BtSockets
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

class BtSocketAddress
{
public:

   BtSocketAddress();
   void reset();

   bool mValid;
   SOCKADDR_BTH mBtAddress;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

class BaseSocket
{
public:

   BaseSocket();
   virtual ~BaseSocket();
   virtual void reset();
 
   virtual bool doSocket() = 0;
   virtual bool doClose();

   BtSocketAddress mLocal;
   BtSocketAddress mRemote;
   SOCKET mDesc;

   bool updateError(int aStatus);
   void setError(int aError);
   void showError(const char* aLabel);

   int mStatus;
   int mError;

   bool ioctlBlocking(bool aBlocking);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

class BaseTcpStreamSocket : public BaseSocket
{
public:

   bool doSocket  ();
   bool doConnect ();
   bool doSend    (const char* aPayload,int aLength);
   bool doRecv    (char* aPayload,int aLength,int& aStatus);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

class BaseTcpClientSocket : public BaseTcpStreamSocket
{
public:
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace


