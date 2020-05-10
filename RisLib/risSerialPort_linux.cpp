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
      mTerminateFlag = false;
   }

   SerialPort::~SerialPort(void)
   {
      doClose();
      delete mSpecific;
   }

   void SerialPort::initialize(SerialSettings& aSettings)
   {
      mSettings = aSettings;
   }

   bool SerialPort::isValid() { return mValidFlag; }

   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   // Open the port.

   bool SerialPort::doOpen()
   {
      mValidFlag = false;

      TS::print(1, "SerialPort::doOpen %s", mSettings.mPortDevice);

      //***************************************************************************
      //***************************************************************************
      //***************************************************************************
      // Open the port.

      mSpecific->mPortFd = open(mSettings.mPortDevice, O_RDWR | O_NOCTTY | O_SYNC);

      if (mSpecific->mPortFd < 0)
      {
         TS::print(0, "serial_open_error_1 %d %s", errno, strerror(errno));
         return false;
      }

      //***************************************************************************
      //***************************************************************************
      //***************************************************************************
      // Open the port.

      mSpecific->mEventFd = eventfd(0, EFD_SEMAPHORE);

      if (mSpecific->mPortFd < 0)
      {
         TS::print(1, "serial_open_error_2 %d", errno);
         return false;
      }

      //***************************************************************************
      //***************************************************************************
      //***************************************************************************
      // Configure the port for raw data.

      struct termios tOptions;
      tcgetattr(mSpecific->mPortFd, &tOptions);
      cfmakeraw(&tOptions);

      if (strcmp(mSettings.mPortSetup, "19200") == 0)
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

      tcsetattr(mSpecific->mPortFd, TCSANOW, &tOptions);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Configure the port for rs485.

   if (mSettings.m485Flag)
   {
      struct serial_rs485 t485conf = { 0 };
      t485conf.flags |= SER_RS485_ENABLED;
      t485conf.flags |= SER_RS485_RTS_ON_SEND;
      t485conf.flags &= ~(SER_RS485_RTS_AFTER_SEND);
      
      if (ioctl(mSpecific->mPortFd, TIOCSRS485, &t485conf) < 0)
      {
         TS::print(1, "serial_open_error_485 %d", errno);
         return false;
      }
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Purge.

   Ris::Threads::threadSleep(100);
   doPurge();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Done.
 
   if (!mSettings.m485Flag)
   {
      TS::print(1, "SerialPort initialize PASS  $ %s : %16s",
         mSettings.mPortDevice,
         mSettings.mPortSetup);
   }
   else
   {
      TS::print(1, "SerialPort initialize PASS  $ %s : %16s RS485",
         mSettings.mPortDevice,
         mSettings.mPortSetup);
   }


   mValidFlag = true;
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Close the port.

void SerialPort::doClose()
{
   int tRet = 0;
   if (!mValidFlag) return;

   TS::print(1, "SerialPort::doClose %s", mSettings.mPortDevice);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Close the port.

   // Set invalid.
   mValidFlag = false;

   // Write bytes to the event semaphore.
   unsigned long long tCount = 1;
   tRet = write(mSpecific->mEventFd, &tCount, 8);

   // Close the port.
   tRet = close(mSpecific->mPortFd);

   // Test the return code.
   if (tRet != 0)
   {
      TS::print(1, "serial_close_error_2 %d", errno);
   }

   // Done.
   mSpecific->mPortFd = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Purge the port.

void SerialPort::doPurge()
{
   return;
   tcflush(mSpecific->mPortFd, TCIFLUSH);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send data, fixed number of bytes.

int SerialPort::doSendBytes(char* aData, int aNumBytes)
{
   // Guard.
   if (!isValid()) return cRetCodeError;

   // Local variables.
   int tNumWritten = 0;
   int tRet  = 0;

   // Write bytes to the port.
   tRet = write(mSpecific->mPortFd, aData, aNumBytes);

   // Test the return code.
   if (tRet < 0)
   {
      TS::print(0, "serial_write_error_1 %d", errno);
      return cRetCodeError;
   }

   if (tRet != aNumBytes)
   {
      TS::print(0, "serial_write_error_2 %d", tRet);
      return cRetCodeError;
   }

   // Write was successful. Return the number of bytes written.
   TS::print(4, "SerialPort::doSendBytes PASS %d",aNumBytes);
   return tRet;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send data, null terminated string, append an end of line LF (\n,10)

int SerialPort::doSendLine(const char *aData)
{
   // Copy the input string to a temp buffer and append a terminator.
   if (mSettings.mTermMode == cSerialTermMode_LF)
   {
      char tBuffer[200];
      strncpy(tBuffer, aData, 196);
      size_t tLength = strlen(tBuffer);
      tBuffer[tLength] = '\n';
      tLength++;
      tBuffer[tLength] = 0;
      return doSendBytes(tBuffer, tLength);
   }
   else
   {
      char tBuffer[200];
      strncpy(tBuffer, aData, 195);
      size_t tLength = strlen(tBuffer);
      tBuffer[tLength] = '\r';
      tLength++;
      tBuffer[tLength] = '\n';
      tLength++;
      tBuffer[tLength] = 0;
      return doSendBytes(tBuffer, tLength);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send data, one byte.

int SerialPort::doSendOne(char aData)
{
   return doSendBytes(&aData,1);   
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive a string, terminated with end of line LF (\n,10) or 
// CRLF (\r\n,13,10). Trims the  terminator and returns a null terminated
// string. Termination mode is determined by settings.

int SerialPort::doReceiveLine(char* aData, int aMaxNumBytes)
{
   if (mSettings.mTermMode == cSerialTermMode_LF)
   {
      return doReceiveLineLF(aData, aMaxNumBytes);
   }
   else
   {
      return doReceiveLineCRLF(aData, aMaxNumBytes);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive a string, terminated with end of line LF (\n,10).

int SerialPort::doReceiveLineLF(char *aData, int aMaxNumBytes)
{
   int  tStatus = -1;
   int  tIndex = 0;
   int  tRxStatus = 0;
   char tRxChar = 0;
   bool tGoing = true;

   aData[0] = 0;

   // Loop to read single bytes, store them, and exit
   // when termination cr/lf is detected
   while (isValid() && tGoing)
   {
      // Read one byte
      tRxStatus = doReceiveOne(&tRxChar);
      if (tRxStatus >= 0)
      {
         // Read success
         // Store byte
         aData[tIndex] = tRxChar;

         // If CR
         if (tRxChar == 10)
         {
            // Terminator detected, strip if off
            tGoing = false;
            aData[tIndex] = 0;
            tStatus = tIndex;
         }
         if (tIndex == aMaxNumBytes - 1)
         {
            // NumBytes limit was reached
            tGoing = false;
            aData[tIndex] = 0;
            tStatus = tIndex;
         }

         // Increment
         tIndex++;
      }
      else
      {
         // Read failure
         tStatus = tRxStatus;
         tGoing = false;
      }
   }
   return tStatus;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive a string, terminated with end of line LF (\r\n,13,10).

int SerialPort::doReceiveLineCRLF(char* aData, int aMaxNumBytes)
{
   int  tStatus = -1;
   int  tIndex = 0;
   int  tRxStatus = 0;
   char tRxChar = 0;
   char tLastRxChar = 0;
   bool tGoing = true;

   aData[0] = 0;

   // Loop to read single bytes, store them, and exit
   // when termination cr/lf is detected
   while (isValid() && tGoing)
   {
      // Save last received char.
      tLastRxChar = tRxChar;
      // Read one byte.
      tRxStatus = doReceiveOne(&tRxChar);
      if (tRxStatus >= 0)
      {
         // Read success.
         // Store byte.
         aData[tIndex] = tRxChar;

         // If CR
         if (tRxChar == 10 && tLastRxChar == 13)
         {
            // Terminator detected, strip if off
            tGoing = false;
            aData[tIndex - 1] = 0;
            tStatus = tIndex;
         }
         if (tIndex == aMaxNumBytes - 1)
         {
            // NumBytes limit was reached
            tGoing = false;
            aData[tIndex] = 0;
            tStatus = tIndex;
         }

         // Increment
         tIndex++;
      }
      else
      {
         // Read failure
         tStatus = tRxStatus;
         tGoing = false;
      }
   }
   return tStatus;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive one byte.

int  SerialPort::doReceiveOne(char *aData)
{
   return doReceiveBytes(aData, 1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive bytes.

int SerialPort::doReceiveBytes(char *aData, int aNumBytes)
{
   TS::print(5, "SerialPort::doReceiveBytes START %d", aNumBytes);

   // Locals.
   int tRet  = 0;
   
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Set the number of bytes to read.

   struct termios tOptions;
   tcgetattr(mSpecific->mPortFd, &tOptions);
   tOptions.c_cc[VMIN] = aNumBytes;
   tcsetattr(mSpecific->mPortFd, TCSANOW, &tOptions);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Poll the port for a read or a close.

   // Read.
   TS::print(5, "serial_poll_start");

   struct pollfd tPollFd[2];
   tPollFd[0].fd = mSpecific->mPortFd;
   tPollFd[0].events = POLLIN;
   tPollFd[0].revents = 0;
   tPollFd[1].fd = mSpecific->mEventFd;
   tPollFd[1].events = POLLIN;
   tPollFd[1].revents = 0;

   // Poll the port for read.
   tRet = poll(&tPollFd[0], 2, -1);

   // Test the valid flag for closing.
   if (!mValidFlag)
   {
      TS::print(1, "serial_poll_invalid close");
      return cRetCodeError;
   }

   // Test the return code for error.
   if (tRet < 0)
   {
      TS::print(1, "serial_poll_error_1 %d", errno);
      return cRetCodeError;
   }

   // Test the return code for timeout.
   if (tRet == 0)
   {
      TS::print(1, "serial_poll_error_2 timeout");
      return cRetCodeTimeout;
   }

   if (tPollFd[0].revents & POLLIN)
   {
      TS::print(5, "serial_poll_event0 %d %04X",tRet, tPollFd[0].revents);
   }

   if (tPollFd[1].revents & POLLIN)
   {
      TS::print(5, "serial_poll_event1 %d %04X", tRet, tPollFd[1].revents);
   }

   // Test the return code for closed port.
   if (tRet == 2)
   {
      TS::print(1, "serial_poll_error_3 close");
      return cRetCodeError;
   }

   TS::print(5, "serial_poll_pass %d",tRet);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read from port.

   // Read.
   TS::print(5, "serial_read_start");
   tRet = (int)read(mSpecific->mPortFd, aData, (size_t)aNumBytes);

   // Test the return code.
   if (tRet < 0)
   {
      TS::print(0, "serial_read_error_1 %d", errno);
      return cRetCodeError;
   }

   if (tRet != aNumBytes)
   {
      TS::print(0, "serial_read_error_2 %d", tRet);
      return cRetCodeError;
   }

   TS::print(5, "serial_read_pass");

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Done.

   // Read was successful. Return the number of bytes read.
   TS::print(4, "SerialPort::doReceiveBytes PASS %d", aNumBytes);
   return tRet;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return the number of bytes that are available to receive.

int SerialPort::getAvailableReceiveBytes()
{
   int tBytesAvaiable;
   ioctl(mSpecific->mPortFd, FIONREAD, &tBytesAvaiable);
   return tBytesAvaiable;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

