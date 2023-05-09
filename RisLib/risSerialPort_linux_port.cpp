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

SerialPort::SerialPort()
{
   mPortFd = 0;
   mEventFd = 0;
   mValidFlag = false;
   mOpenFlag = false;
   mAbortFlag = false;
   mOpenErrorShowCount = 0;
   mCloseErrorShowCount = 0;
   mTI = 0;
   mRxByteCount = 0;
   mTxByteCount = 0;
   mPortErrorCount = 0;
}

SerialPort::~SerialPort()
{
}

void SerialPort::initialize(SerialSettings& aSettings)
{
   mSettings = aSettings;
   mTI = aSettings.mTraceIndex;
   mOpenErrorShowCount = 0;
   mCloseErrorShowCount = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Open the serial port with the settings.
//
// Configure the port for raw, set the baud rate, and optionally 
// select RS485.

bool SerialPort::doOpen()
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Do this first.

   Trc::write(mTI, 0, "SerialPort::doOpen");

   // Guard.
   if (mOpenFlag)
   {
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doOpen ERROR already open");
      return false;
   }

   mValidFlag = false;
   mAbortFlag = false;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Open the port.

   // Open the port.
   mPortFd = open(mSettings.mPortDevice, O_RDWR | O_NOCTTY | O_SYNC);

   // Test the return code.
   if (mPortFd < 0)
   {
      if (mOpenErrorShowCount == 0)
      {
         printf("serial_open_error_1 %s %d %s\n", mSettings.mPortDevice, errno, strerror(errno));
      }
      mOpenErrorShowCount++;
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doOpen ERROR 1");
      return false;
   }

   // Open the event.
   mEventFd = eventfd(0, EFD_SEMAPHORE);

   // Test the return code.
   if (mPortFd < 0)
   {
      if (mOpenErrorShowCount == 0)
      {
         printf("serial_open_error_2 %s %d %s\n", mSettings.mPortDevice, errno, strerror(errno));
      }
      mOpenErrorShowCount++;
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doOpen ERROR 2");
      return false;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Configure the port from the settings.

   // Configure the port for raw data.
   struct termios tOptions;
   tcgetattr(mPortFd, &tOptions);
   cfmakeraw(&tOptions);

   // Configure the baud rate.
   if (strcmp(mSettings.mPortSetup, "9600") == 0)
   {
      cfsetispeed(&tOptions, B9600);
      cfsetospeed(&tOptions, B9600);
   }
   else if (strcmp(mSettings.mPortSetup, "19200") == 0)
   {
      cfsetispeed(&tOptions, B19200);
      cfsetospeed(&tOptions, B19200);
   }
   else if (strcmp(mSettings.mPortSetup, "38400") == 0)
   {
      cfsetispeed(&tOptions, B38400);
      cfsetospeed(&tOptions, B38400);
   }
   else
   {
      cfsetispeed(&tOptions, B115200);
      cfsetospeed(&tOptions, B115200);
   }

   if (tcsetattr(mPortFd, TCSANOW, &tOptions) < 0)
   {
      printf("serial_open_error_baud\n");
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doOpen ERROR baud");
      return false;
   }

   // Configure the port for rs485.
   if (mSettings.m485Flag)
   {
      struct serial_rs485 t485conf;
      memset(&t485conf, 0, sizeof(struct serial_rs485));
      t485conf.flags |= SER_RS485_ENABLED;
      t485conf.flags |= SER_RS485_RTS_ON_SEND;
      t485conf.flags &= ~(SER_RS485_RTS_AFTER_SEND);
      
      if (ioctl(mPortFd, TIOCSRS485, &t485conf) < 0)
      {
         printf("serial_open_error_485 %d\n", errno);
         Trc::write(mTI, 0, "SerialPort::doOpen ERROR RS485");
         mPortErrorCount++;
         return false;
      }
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Flush.

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Done.

   // Set flags.
   printf("SerialPort open    PASS          %s\n", mSettings.mPortDevice);
   mOpenFlag = true;
   mValidFlag = true;
   mOpenErrorShowCount = 0;
   mCloseErrorShowCount = 0;

   // Flush.
   doFlush();

   // Done.
   Trc::write(mTI, 0, "SerialPort::doOpen done");
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Wait for the modem to be valid. Return true if successful.

bool SerialPort::doWaitForModemValid()
{
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Close the serial port.

void SerialPort::doClose()
{
   Trc::write(mTI, 0, "SerialPort::doClose");

   // Test if already closed.
   if (!mOpenFlag)
   {
      Trc::write(mTI, 0, "SerialPort::doClose ERROR not open");
      mPortErrorCount++;
      return;
   }

   int tRet = 0;

   // Set invalid.
   mValidFlag = false;

   // Flush the input and output buffers.
   doFlush();

   // Close the event.
   Trc::write(mTI, 1, "SerialPort::doClose close event");
   tRet = close(mEventFd);
   mEventFd = 0;

   // Test the return code.
   if (tRet != 0)
   {
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doClose ERROR 1");
      return;
   }
   Trc::write(mTI, 1, "SerialPort::doClose close event done");

   // Close the port.
   Trc::write(mTI, 1, "SerialPort::doClose close port");
   tRet = close(mPortFd);
   mPortFd = 0;

   // Test the return code.
   if (tRet != 0)
   {
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doClose ERROR 2");
      return;
   }
   Trc::write(mTI, 1, "SerialPort::doClose close port done");

   // Set closed.
   mOpenFlag = false;

   // Done.
   mCloseErrorShowCount = 0;
   Trc::write(mTI, 0, "SerialPort::doClose done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Abort pending serial port receive.
// 
// Set the abort flag and write bytes to the event semaphore. This will
// cause a pending receive call poll to return and the receive call will
// abort because the abort flag is set.

void SerialPort::doAbort()
{
   Trc::write(mTI, 0, "SerialPort::doAbort");

   // Set the abort flag.
   mAbortFlag = true;

   // Guard.
   int tRet = 0;
   if (!mOpenFlag)
   {
      Trc::write(mTI, 0, "SerialPort::doAbort not open");
      return;
   }

   // Write bytes to the event semaphore.
   Trc::write(mTI, 0, "SerialPort::doAbort post event");
   unsigned long long tCount = 1;
   tRet = write(mEventFd, &tCount, 8);
   Ris::Threads::threadSleep(10);
   Trc::write(mTI, 0, "SerialPort::doAbort post event done");

   // Test the return code.
   if (tRet < 0)
   {
      Trc::write(mTI, 0, "SerialPort::doAbort ERROR");
      printf("serial_abort_1 ERROR %d\n", errno);
      mPortErrorCount++;
      return;
   }
   Trc::write(mTI, 0, "SerialPort::doAbort done");
   printf("serial_abort done\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Flush serial port buffers.

void SerialPort::doFlush()
{
   Trc::write(mTI, 0, "SerialPort::doFlush");
   int tRet = 0;

   // Guard.
   if (!mOpenFlag)
   {
      Trc::write(mTI, 0, "SerialPort::doFlush ERROR not open");
      mPortErrorCount++;
      return;
   }

   // Flush the buffers.
   tRet = tcflush(mPortFd, TCIOFLUSH);

   // Test the return code.
   if (tRet != 0)
   {
      printf("serial_flush ERROR 101 %d\n", errno);
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doFlush ERROR");
      return;
   }
   Trc::write(mTI, 0, "SerialPort::doFlush done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Drain serial port buffers.

void SerialPort::doDrain()
{
   Trc::write(mTI, 0, "SerialPort::doDrain");
   int tRet = 0;

   // Guard.
   if (!mOpenFlag)
   {
      Trc::write(mTI, 0, "SerialPort::doDrain ERROR not open");
      mPortErrorCount++;
      return;
   }

   // Drain the buffers.
   tRet = tcdrain(mPortFd);

   // Test the return code.
   if (tRet != 0)
   {
      printf("serial_flush ERROR 101 %d\n", errno);
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doDrain ERROR");
      return;
   }
   Trc::write(mTI, 0, "SerialPort::doDrain done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Suspend serial port transmits and receives.

void SerialPort::doSuspend()
{
   Trc::write(mTI, 0, "SerialPort::doSuspend");

   // Guard.
   int tRet = 0;
   if (!mOpenFlag)
   {
      Trc::write(mTI, 0, "SerialPort::doSuspend ERROR not open");
      mPortErrorCount++;
      return;
   }

   // Suspend output.
   tRet = tcflow(mPortFd, TCOOFF);

   // Test the return code.
   if (tRet != 0)
   {
      printf("serial_suspend ERROR 1 %d\n", errno);
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doSuspend ERROR");
      return;
   }
   Trc::write(mTI, 0, "SerialPort::doSuspend done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Resume serial port transmits and receives.

void SerialPort::doResume()
{
   Trc::write(mTI, 0, "SerialPort::doResume");
   // Guard.
   int tRet = 0;
   if (!mOpenFlag)
   {
      Trc::write(mTI, 0, "SerialPort::doResume ERROR not open");
      mPortErrorCount++;
      return;
   }

   // Resume output.
   tRet = tcflow(mPortFd, TCOOFF);

   // Test the return code.
   if (tRet != 0)
   {
      printf("serial_resume ERROR 101 %d\n", errno);
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doResume ERROR");
      return;
   }
   Trc::write(mTI, 0, "SerialPort::doResume done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

