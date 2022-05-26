
#include "stdafx.h"

#include "procoUdpSettings.h"
#include "risNetUdpMsgThread.h"
#include "risSleep.h"
#include "procoMsg.h"

#include "MainTest.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Test.

void main_test()
{
   Ris::Net::UdpMsgThread* mMsgThread;
   ProtoComm::MsgMonkey* mMsgMonkey;

   mMsgMonkey = new ProtoComm::MsgMonkey;

   // Instance of network socket settings.
   Ris::Net::Settings tSettings;

   tSettings.setLocalPort(ProtoComm::gUdpSettings.mMyUdpPort);
   tSettings.setRemoteAddress(ProtoComm::gUdpSettings.mOtherUdpIPAddress, ProtoComm::gUdpSettings.mOtherUdpPort);
   tSettings.setUdpWrapFlag(ProtoComm::gUdpSettings.mUdpWrapFlag);
   tSettings.mMsgMonkey = mMsgMonkey;
   //tSettings.mRxMsgQCall = mRxMsgQCall;
   tSettings.mTraceIndex = 11;

   // Create the child thread with the settings.
   mMsgThread = new Ris::Net::UdpMsgThread(tSettings);

   // Launch the child thread.
   mMsgThread->launchThread();

   Ris::sleepMs(1000);
   // Launch the child thread.
   mMsgThread->shutdownThread();
   delete mMsgThread;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
