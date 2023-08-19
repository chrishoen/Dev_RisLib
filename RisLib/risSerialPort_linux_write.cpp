//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include <signal.h>
#include <poll.h>
#include <sys/eventfd.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <linux/serial.h>

#include "my_functions.h"
#include "risThreadsThreads.h"
#include "trcTrace.h"

#include "risSerialPort_linux.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a fixed number of bytes. Return the actual number of bytes
// sent or a negative error code.

int SerialPort::doSendBytes(const char* aBytes, int aNumBytes)
{
   Trc::write(mTI, 0, "SerialPort::doSendBytes");
   // Guard.
   if (!mValidFlag)
   {
      Trc::write(mTI, 0, "SerialPort::doSendBytes INVALID");
      return cSerialRetError;
   }

   // Local variables.
   int tNumWritten = 0;
   int tRet = 0;

   // Write bytes to the port.
   tRet = write(mPortFd, aBytes, aNumBytes);

   // Test the return code.
   if (tRet < 0)
   {
      doAbort();
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doSendBytes ERROR 1");
      return cSerialRetError;
   }

   if (tRet != aNumBytes)
   {
      doAbort();
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doSendBytes ERROR 2");
      return cSerialRetError;
   }

   // Write was successful. Return the number of bytes written.
   //printf("SerialPort::doSendBytes PASS %d\n",aNumBytes);
   mTxByteCount += tRet;
   Trc::write(mTI, 0, "SerialPort::doSendBytes done");
   return tRet;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

