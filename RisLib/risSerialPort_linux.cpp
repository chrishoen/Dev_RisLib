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
#include "prnPrint.h"

#include "risSerialPort.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Portable specifics.

class SerialPort::Specific
{
public:
   int mPortFd;
   int mEventFd;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

SerialPort::SerialPort()
{
   mSpecific = new Specific;
   mSpecific->mPortFd = 0;
   mSpecific->mEventFd = 0;
   mValidFlag = false;
   mAbortFlag = false;
}

SerialPort::~SerialPort()
{
   //doClose();
   delete mSpecific;
}

void SerialPort::initialize(SerialSettings& aSettings)
{
   mSettings = aSettings;
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

   // Close the serial port, if already open.
   if (mValidFlag)
   {
      doClose();
   }

   mAbortFlag = false;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Open the port.

   // Open the port.
   mSpecific->mPortFd = open(mSettings.mPortDevice, O_RDWR | O_NOCTTY | O_SYNC);

   // Test the return code.
   if (mSpecific->mPortFd < 0)
   {
      printf("serial_open_error_1 %d %s\n", errno, strerror(errno));
      return false;
   }

   // Open the event.
   mSpecific->mEventFd = eventfd(0, EFD_SEMAPHORE);

   // Test the return code.
   if (mSpecific->mPortFd < 0)
   {
      printf("serial_open_error_2 %d\n", errno);
      return false;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Configure the port from the settings.

   // Configure the port for raw data.
   struct termios tOptions;
   tcgetattr(mSpecific->mPortFd, &tOptions);
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
      cfsetispeed(&tOptions, B38400);
      cfsetospeed(&tOptions, B38400);
   }

   if (tcsetattr(mSpecific->mPortFd, TCSANOW, &tOptions) < 0)
   {
      printf("serial_open_error_baud\n", errno);
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
      
      if (ioctl(mSpecific->mPortFd, TIOCSRS485, &t485conf) < 0)
      {
         printf("serial_open_error_485 %d\n", errno);
         return false;
      }
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Flush.

   doFlush();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Done.
 
   printf("SerialPort open PASS  %s\n", mSettings.mPortDevice);
   mValidFlag = true;
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Close the serial port.

void SerialPort::doClose()
{
   // Test if already closed.
   if (!mValidFlag)
   {
      printf("serial_close already closed\n");
      return;
   }

   int tRet = 0;

   // Set invalid.
   mValidFlag = false;

   // Flush the input and output buffers.
   printf("serial_close flush\n");
   doSuspend();
   doFlush();
   //doResume();
   //Ris::Threads::threadSleep(100);

   // Close the event.
   printf("serial_close event\n");
   tRet = close(mSpecific->mEventFd);

   // Test the return code.
   if (tRet != 0)
   {
      printf("serial_close_error_1 %d\n", errno);
   }

   // Close the port.
   printf("serial_close port\n");
   tRet = close(mSpecific->mPortFd);

   // Test the return code.
   if (tRet != 0)
   {
      printf("serial_close_error_2 %d\n", errno);
   }
   printf("serial_close done\n");

   // Done.
   mSpecific->mEventFd = 0;
   mSpecific->mPortFd = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Flush serial port buffers.

void SerialPort::doFlush()
{
   // Guard.
   int tRet = 0;
   if (!mValidFlag) return;

   // Flush the buffers.
   tRet = tcflush(mSpecific->mPortFd, TCIOFLUSH);

   // Test the return code.
   if (tRet != 0)
   {
      printf("serial_flush ERROR 101 %d\n", errno);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Suspend serial port transmits and receives.

void SerialPort::doSuspend()
{
   // Guard.
   int tRet = 0;
   if (!mValidFlag) return;

   // Suspend output.
   tRet = tcflow(mSpecific->mPortFd, TCOOFF);

   // Test the return code.
   if (tRet != 0)
   {
      printf("serial_suspend ERROR 101 %d\n", errno);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Resume serial port transmits and receives.

void SerialPort::doResume()
{
   // Guard.
   int tRet = 0;
   if (!mValidFlag) return;

   // Resume output.
   tRet = tcflow(mSpecific->mPortFd, TCOOFF);

   // Test the return code.
   if (tRet != 0)
   {
      printf("serial_resume ERROR 101 %d\n", errno);
   }
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
   // Set the abort flag.
   mAbortFlag = true;

   // Guard.
   int tRet = 0;
   if (!mValidFlag) return;

   // Write bytes to the event semaphore.
   unsigned long long tCount = 1;
   tRet = write(mSpecific->mEventFd, &tCount, 8);

   // Test the return code.
   if (tRet < 0)
   {
      printf("serial_abort_1 ERROR %d\n", errno);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return the number of bytes that are available to receive.

int SerialPort::doGetAvailableReceiveBytes()
{
   int tBytesAvaiable;
   ioctl(mSpecific->mPortFd, FIONREAD, &tBytesAvaiable);
   return tBytesAvaiable;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a fixed number of bytes. Return the actual number of bytes
// sent or a negative error code.

int SerialPort::doSendBytes(const char* aBytes, int aNumBytes)
{
   // Guard.
   if (!mValidFlag) return cSerialRetError;

   // Local variables.
   int tNumWritten = 0;
   int tRet  = 0;

   // Write bytes to the port.
   tRet = write(mSpecific->mPortFd, aBytes, aNumBytes);

   // Test the return code.
   if (tRet < 0)
   {
      printf("serial_write_error_1 %d\n", errno);
      return cSerialRetError;
   }

   if (tRet != aNumBytes)
   {
      printf("serial_write_error_2 %d\n", tRet);
      return cSerialRetError;
   }

   // Write was successful. Return the number of bytes written.
   //printf("SerialPort::doSendBytes PASS %d\n",aNumBytes);
   return tRet;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive any available bytes. Block until at least one byte has
// been received. Return the number of bytes received or a negative
// error code. Copy the bytes into the pointer argument.

int SerialPort::doReceiveAnyBytes(char* aBytes, int aMaxNumBytes)
{
   // Guard.
   if (!mValidFlag) return cSerialRetError;
   int tRet = 0;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Poll the port for a read or a close.

   // Setup a poll structure for the port and event fds.
   struct pollfd tPollFd[2];
   tPollFd[0].fd = mSpecific->mPortFd;
   tPollFd[0].events = POLLIN;
   tPollFd[0].revents = 0;
   tPollFd[1].fd = mSpecific->mEventFd;
   tPollFd[1].events = POLLIN;
   tPollFd[1].revents = 0;

   // Blocking poll for "there is data to read". This returns when
   // the port has received data or the abort event was posted
   tRet = poll(&tPollFd[0], 2, -1);

   // Test for an abort request.
   if (mAbortFlag)
   {
      //printf("serial_poll_abort\n");
      return cSerialRetAbort;
   }

   // Test the return code for error.
   if (tRet < 0)
   {
      //printf("serial_poll_error_1 %d\n", errno);
      return cSerialRetError;
   }

   // Test the return code for timeout.
   if (tRet == 0)
   {
      //printf("serial_poll_error_2 timeout\n");
      return cSerialRetTimeout;
   }

   if (tPollFd[0].revents & POLLIN)
   {
      //printf("serial_poll_event0 %d %04X\n",tRet, tPollFd[0].revents);
   }

   if (tPollFd[1].revents & POLLIN)
   {
      //printf("serial_poll_event1 %d %04X\n", tRet, tPollFd[1].revents);
   }

   // Test the return code for closed port.
   if (tRet == 2)
   {
      //printf("serial_poll_error_3 close\n");
      return cSerialRetError;
   }

   //printf("serial_poll_pass %d\n",tRet);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read from the serial port.

   // Read.
   tRet = (int)read(mSpecific->mPortFd, aBytes, (size_t)aMaxNumBytes);

   // Test the return code.
   if (tRet < 0)
   {
      printf("serial_read_error_1 %d\n", errno);
      return cSerialRetError;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Done.

   // Read was successful. Return the number of bytes read.
   return tRet;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive a requested number of bytes. Block until all of the bytes
// have been received. Return the number of bytes received or a
// negative error code. Copy the bytes into the pointer argument.

int SerialPort::doReceiveAllBytes(char* aBytes, int aRequestBytes)
{
   // Guard.
   if (!mValidFlag) return cSerialRetError;
   if (aRequestBytes == 0) return 0;
   int tRet = 0;

   // Loop to read all of the requested bytes.
   int tBytesRequired = aRequestBytes;
   int tBytesRemaining = aRequestBytes;
   int tBytesTotal = 0;

   bool tGoing = true;
   while (tGoing)
   {
      // Read any available receive bytes. Block until at least one byte
      // has been received. Return the number of bytes read or a negative
      // error code.
      tRet = doReceiveAnyBytes(&aBytes[tBytesTotal], tBytesRemaining);

      if (tRet > 0)
      {
         tBytesTotal += tRet;
         tBytesRemaining -= tRet;
         if (tBytesTotal == tBytesRequired)
         {
            tGoing = false;
            tRet = tBytesTotal;
         }
      }
      else
      {
         tGoing = false;
      }
   }

   // If success then return the total number of bytes read, which is 
   // equal to the requested number of bytes. If failure then return
   // the negative error code.
   return tRet;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive one byte. Block until the byte has been received. Return
// one or zero or a negative error code. Copy the byte into the
// pointer argument.

int SerialPort::doReceiveOneByte(char* aByte)
{
   return doReceiveAnyBytes(aByte, 1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

#if 0
// Set the number of bytes to read.
struct termios tOptions;
tcgetattr(mSpecific->mPortFd, &tOptions);
tOptions.c_cc[VMIN] = aNumBytes;
tcsetattr(mSpecific->mPortFd, TCSANOW, &tOptions);



if (!mSettings.m485Flag)
{
   printf("SerialPort initialize PASS  $ %s : %16s\n",
      mSettings.mPortDevice,
      mSettings.mPortSetup);
}
else
{
   printf("SerialPort initialize PASS  $ %s : %16s RS485\n",
      mSettings.mPortDevice,
      mSettings.mPortSetup);
}

#endif
