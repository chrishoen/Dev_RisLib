#pragma once

/*==============================================================================

Tcp message server hub socket.

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsQCallThread.h"
#include "risNetSettings.h"
#include "risNetTcpMsgSocket.h"

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
// Tcp server hub socket, it is used to manage client connections.
// It does listen and accept socket calls into tcp stream sockets in response
// to client connect calls. 

class TcpServerHubSocket : public Sockets::BaseTcpServerHubSocket
{
private:
   typedef Sockets::BaseTcpServerHubSocket BaseClass;
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Settings.
   Settings mSettings;

   // General purpose valid flag.
   bool mValidFlag;

   // Program trace index.
   int mTI;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Constructor.
   TcpServerHubSocket();

   // Initialize variables.
   void initialize(Settings& aSettings);

   // Configure the socket. This does socket and bind calls.
   void configure(bool aShowFlag);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

