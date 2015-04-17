
/* **************************************************************************

                 LOCKHEED MARTIN PROPRIETARY INFORMATION

                      EXPORT CONTROLLED INFORMATION

             IMPORTANT NOTICE - EXPORT CONTROLLED INFORMATION

   The Technical Data of this document and/or drawing contains data
   directly related to an article or a component of an article included
   on the United States Munitions List and as such is subject to the
   International Traffic in Arms Regulation (ITAR).  This data may not
   be disclosed, exported, transferred or diverted in any manner to any
   national foreign to the United States or to any country or
   destination outside the United States without the written approval
   of the United States Department of State.

         COPYRIGHT - LOCKHEED MARTIN, NIAGARA FALLS, NEW YORK, 2010

*****************************************************************************

   LOCKHEED MARTIN GRAVITY SYSTEMS
   Niagara Operations
   2221 Niagara Falls Boulevard
   Niagara Falls, New York  14304 - 1696

   FILE NAME       : DAS_IF.c
   ID Number       : 
   Initial Release : Month, Year <date of initial release, version 1.0>


   *** REVISION HISTORY ***

   Revision     : 
   Release date : 11/8/2010 gcl
   PPRs         : DSPR 45
   Change description:
      Reset task timer every time the DAS response is received
      ( re-sync DAS and SCP clock timers )

************************************************************************** */
#include <errno.h>   /* Error number definitions */
#include <fcntl.h>   /* File control definitions */
#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <stdlib.h>
#include <signal.h>  // task signal handler

#include "DAS_IF.h"

// DAS communicates through COM2
#define  DAS_TTY "/dev/ttyS1"

// Serial Port Configuration 
#define  SPC_OK             ( 0)
#define  SPC_OPEN_ERROR     (-2)
#define  SPC_GET_ATTR_ERROR (-3)
#define  SPC_O_SPEED_ERROR  (-4)
#define  SPC_I_SPEED_ERROR  (-5)
#define  SPC_STOP_BIT_ERROR (-6)
#define  SPC_FLUSH_ERROR    (-7)

// Sync Bytes 
#define  SB_OK              ( 0)
#define  SB_READ_ERROR      (-1)
#define  SB_MATCH_ERROR     (-2)

// Serial Data Message 
#define  MS_OK               ( 0)
#define  MS_MESSAGE_OK       ( 1)
#define  MS_SYNC_ERROR       (-1)
#define  MS_SYNC_BYTES_ERROR (-2)
#define  MS_PAYLOAD_ERROR    (-3)

INT32   serial_port; // DAS serial port

CMN_DATA_TYPE	*Cmn;
CHAR logMsg[4096];

UINT08	DASseq = 0;
UINT08	AGGseq = 0;

AGG_REQUEST	   AGGrequest;
DAS_MSG_STATUS	   msgStat;
DAS_DATA_VALIDITY  validity;
UINT32             GPSdataAge = 0;

struct itimerval itval;


/* ***************************** */
/* *** function unwhite() ****** */
/* ***************************** */
void unwhite( char *s )
{
  for( ; *s != NUL; s++ ) if isspace(*s) *s = '+';
}

/* ***************************** */
/* *** function set_iTimer() *** */
/* ***************************** */
// DSPR #45  11/8/2010 gcl
/* NOTES:

   Timers decrement from it_value to zero, generate a signal, and reset to
   it_interval. A timer which is set to zero (it_value is zero or the timer
   expires and it_interval is zero) stops.

   ITIMER_REAL
     decrements in real time, and delivers SIGALRM upon expiration
*/
void set_iTimer( void )
{
  struct itimerval itval;

  itval.it_interval.tv_sec  = 1;       // 1 second
  itval.it_interval.tv_usec = 0; 
  itval.it_value.tv_sec     = 0;
  itval.it_value.tv_usec    = 500000;  // 0.5 seconds
  setitimer( ITIMER_REAL, &itval, NULL );
}



/***************************************************************************
  handler( ) - LINUX SIGNAL HANDLER
****************************************************************************/
void handler ( int sig )
{
	switch (sig)
	{
	   case SIGALRM :
              GPSdataAge++;
	      msgStat.bits.RecordingOn = Cmn->AGGlog.Recording ? 1 : 0;
	      AGGrequest.Stat = msgStat.word;
	      AGGrequest.SeqNumber = ++AGGseq;
	      AGGrequest.AckSeqNumber = DASseq;
	      memset(AGGrequest.ASCIImsg, ' ', sizeof(AGGrequest.ASCIImsg));
	      SendDASrequest( &AGGrequest );
	      /* SET THE TIMEOUT STATUS BIT TO ERROR CONDITION BY DEFAULT */
	      msgStat.bits.TimeoutError = 1;
	   break;        

	   // SIGNALS RESULTING FROM SYS CLOCK SETTING 
	   case SIGCHLD :
         break;

	   case SIGINT :
	   case SIGQUIT:
			/* Exit program on break or control C */
			sprintf( logMsg, "Program aborted\n");
			logAGG( TSKNUM, logMsg );
			exit(0);
	   break;

	   case SIGPIPE :
			/* Bail out when client has disconnected */
			sprintf( logMsg, "Detected Socket Disconnect" );
			logAGG( TSKNUM, logMsg );
			exit(0);
	   break;

	   default :
			sprintf( logMsg, "Received signal %d", sig );
			logAGG( TSKNUM, logMsg );
			exit(0);
	   break;
	}
}

/*
**************************************************************************
  /dev/ttyS1 configuration code
**************************************************************************
*/

INT32 configure_DAS_port( void )
{
  struct  termios port_info;
  speed_t baud_rate = B9600;
  INT32   retval;
  INT32   result = SPC_OK;
  
  serial_port = open(DAS_TTY, O_RDWR);
  if( serial_port < 0 )
  {    
    result = SPC_OPEN_ERROR;
  }
  else
  {
    if(result == SPC_OK)
    {
      retval = tcgetattr(serial_port, &port_info);
      if(retval != 0)
      {
        result = SPC_GET_ATTR_ERROR;
      }
    }
    
    if(result == SPC_OK)
    {  
      // The cfmakeraw() function sets the terminal attributes as follows:
      // portinfo->c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR
      //                              |ICRNL|IXON);
      //         ->c_oflag &= ~OPOST;
      //         ->c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
      //         ->c_cflag &= ~(CSIZE|PARENB);
      //         ->c_cflag |= CS8;
      cfmakeraw(&port_info);      
  
      retval = cfsetospeed(&port_info, baud_rate);
      if(retval != 0)
      {    
        result = SPC_O_SPEED_ERROR;
      }
    }
      
    if(result == SPC_OK)
    {
      retval = cfsetispeed(&port_info, baud_rate);
      if(retval != 0)
      {
        result = SPC_I_SPEED_ERROR;
      }
    }
    
    if(result == SPC_OK)
    {
      retval = tcsetattr(serial_port, TCSANOW, &port_info);
      if(retval != 0)
      {
        result = SPC_STOP_BIT_ERROR;
      }
    }
    
    if(result == SPC_OK)
    {
      retval = tcflush(serial_port, TCIFLUSH);
      if( retval != 0 )
      { 
        result = SPC_FLUSH_ERROR;
      }
    }

  } // end if( serial_port < 0 )
  
  return(result);

} // end function configure_DAS_port()

/*
**************************************************************************
  read specified number of bytes 
**************************************************************************
*/

INT32 read_bytes(UINT08* buffer, INT32 num_bytes)
{
  INT32 num_read = 0;
  INT32 retval = 0;

  while((num_read < num_bytes) && (retval >= 0))
  {
    retval = read(serial_port, &(buffer[num_read]), num_bytes-num_read);
    num_read += retval;
  }

  if( retval >= 0 )
  {
    retval = num_read;
  }
		
  return(retval);

} // end function read_bytes()

INT32 read_escape_bytes(UINT08* buffer, INT32 num_bytes)
{
  INT32 num_read = 0;
  INT32 retval   = 0;

  while((num_read < num_bytes) && (retval >= 0))
  {
    retval = read(serial_port, &(buffer[num_read]), 1);
    if (retval == 1)
    {
      if (buffer[num_read] != DLE)
      {
        ++num_read;
      }
      else
      {
        // Got a DLE, make sure that the next byte is a DLE
        retval = read(serial_port, &(buffer[num_read]), 1);
        if (retval == 1)
        {
          if (buffer[num_read] != DLE)
          {
             sprintf(logMsg, "ERROR MSG BODY EXPECTED DLE GOT 0x%02X\n", buffer[num_read]);
             logAGG(TSKNUM, logMsg);
             retval = -4;
          }
          else
          {
            ++num_read;
          }
        }
      }
    }
  }

  if( retval >= 0 )
  {
    retval = num_read;
  }
		
  return(retval);

} // end function read_escape_bytes()

/*
**************************************************************************
  write specified number of bytes 
**************************************************************************
*/

INT32 write_bytes(UINT08* buffer, INT32 num_bytes)
{
  INT32 num_written = 0;
  INT32 retval      = 0;

  while((num_written < num_bytes) && (retval >= 0))
  {
    retval = write(serial_port, &(buffer[num_written]), num_bytes-num_written);
    num_written += retval;
  }

  if( retval >= 0 )
  {
    retval = num_written;
  }
		
  return(retval);

} // end function write_bytes()

/*
**************************************************************************
  sync to DLE STX start marker
**************************************************************************
*/

INT32 synch_DAS_port(void)
{
  UINT08 onebyte;
  UINT08 synchbytes[2] = { DLE, STX };
  INT32  retval;
  INT32  synched;
  INT32  result = SB_OK;
  INT32  num_reads = 0;

  // The purpose of this function is to find a start sequence, then
  //   read a complete message so we should be getting another start
  //   sequence after this function is completed.

  // The following loop finds the start sequence
  synched = 0;
  while((synched < 2) && (result == SB_OK)) 
  {
    retval = read_bytes(&onebyte, 1U);
    
    if(retval != 1) // read did not return 1 byte
    {      
      result = SB_READ_ERROR;
    }
    else
    {
      if(onebyte == synchbytes[synched])
      {
        synched++;
      }
      else 
      {
        synched = 0;
      }
    }

    ++num_reads;
  } // end while

  if (num_reads != synched)
  {
    sprintf(logMsg, "SYNCH READ TOOK %d TRIES\n", num_reads);
    logAGG(TSKNUM, logMsg);
  }
	
  return(result);

} // end function synch_DAS_port()

/*
**************************************************************************
  match DLE ETX end marker
**************************************************************************
*/

INT32 match_DAS_port(void)
{
  UINT08 twobytes[2];
  UINT08 synchbytes[2] = { DLE, ETX };
  INT32  retval;
  INT32  synched;
  INT32  result = SB_OK;

  // The purpose of this function is to find a start sequence, then
  //   read a complete message so we should be getting another start
  //   sequence after this function is completed.

  // The following loop finds the start sequence
  synched = 0;

  retval = read_bytes(twobytes, 2U);
    
  if(retval != 2) // read did not return 1 byte
  {      
    result = SB_READ_ERROR;
  }
  else
  {
    if(twobytes[0] != synchbytes[0])
    {
      sprintf(logMsg, "NO DLE AT END 0x%02X\n", twobytes[0]);
      logAGG(TSKNUM, logMsg);
      result = SB_MATCH_ERROR;
    }
    
    if(twobytes[1] != synchbytes[1])
    {
      sprintf(logMsg, "NO ETX AT END 0x%02X\n", twobytes[1]);
      logAGG(TSKNUM, logMsg);
      result = SB_MATCH_ERROR;
    }
  }
	
  return(result);

} // end function match_DAS_port()


/* ********************************* */
/* *** function GetDASresponse() *** */
/* ********************************* */
DAS_MSG_STATUS GetDASresponse( UINT08* m )
{   
  UINT16          msgLen  = 0;
  UINT08          byteSum = 0;
  UINT08          bitsSum = 0;
  UINT08          msgByte = 0;
  UINT08          msgBits = 0;
  UINT16          i;
  UINT08          onebyte;
  UINT08          lenBytes[2];
  INT32           retval;
  DAS_MSG_STATUS  stat;

  stat.word = 0;

  if( synch_DAS_port() != SB_OK )
  {
    logAGG(TSKNUM, "ERROR SYNCHING DAS PORT\n");
    stat.bits.MsgError = 1;
    return stat;
  }

  // DLE / STX received
  //  sync next request, to be sent in 0.5 seconds
  // DSPR #45  11/8/2010 gcl
  set_iTimer();      

  retval = read_bytes(lenBytes, 2U);
  if( retval != 2 )
  {
    sprintf(logMsg, "ERROR READING LENGTH BYTES %d\n", retval); 
    logAGG(TSKNUM, logMsg);
    stat.bits.MsgError = 1;
    return stat;
  }

  msgLen = lenBytes[0] + (lenBytes[1] << 8);

  if (msgLen > 500)
  {
    sprintf(logMsg, "ERROR MESSAGE TOO LONG %d\n", msgLen); 
    logAGG(TSKNUM, logMsg);
    stat.bits.MsgError = 1;
    return stat;
  }
  else
  {
    if (msgLen != 261)
    {
      sprintf(logMsg, "UNEXPECTED MESSAGE LENGTH %d\n", msgLen);
      logAGG(TSKNUM, logMsg);
    }
  }

  // GET MESSAGE BODY
  retval = read_escape_bytes(m, msgLen);
  if (retval != msgLen)
  {
    sprintf(logMsg, "ERROR READING MSG BODY %d\n", retval); 
    logAGG(TSKNUM, logMsg);
    stat.bits.MsgError = 1;
    return stat;
  }

  // message body received; expect next char to be (single) DLE
  retval = match_DAS_port();
  if (retval != SB_OK)
  {  
    sprintf(logMsg, "ERROR MATCHING END BYTES %d\n", retval); 
    logAGG(TSKNUM, logMsg);
    stat.bits.MsgError = 1;
    return stat;
  }

  // GET BYTE CHECKSUM
  retval = read_bytes(&msgByte, 1U);
  if (retval != 1)
  {
    sprintf(logMsg, "ERROR READING BYTE SUM %d\n", retval); 
    logAGG(TSKNUM, logMsg);
    stat.bits.MsgError = 1;
    return stat;
  }

  // GET BIT CHECKSUM
  retval = read_bytes(&msgBits, 1U);
  if (retval != 1)
  {
    sprintf(logMsg, "ERROR READING BIT SUM %d\n", retval); 
    logAGG(TSKNUM, logMsg);
    stat.bits.MsgError = 1;
    return stat;
  }

  // Calculate the checksums
  for (i = 0; i < msgLen; i++)
  {
    byteSum += m[i];
    bitsSum ^= m[i];
  }

  // INCORPORATE ETX INTO SUMS
  byteSum += ETX;
  bitsSum ^= ETX;

  // CHECK BYTE SUM
  if (byteSum != msgByte)
  {
    sprintf(logMsg, "BAD BYTE SUM (%02X != %02X)\n", msgByte, byteSum);
    logAGG(TSKNUM, logMsg);
    stat.bits.ByteSumError = 1;
  }

  // CHECK BYTE SUM
  if (bitsSum != msgBits)
  {
    sprintf(logMsg, "BAD BITS SUM (%02X != %02X)\n", msgBits, bitsSum);
    logAGG(TSKNUM, logMsg);
    stat.bits.XORsumError = 1;
  }

  return stat;

} // end GetDASresponse()


/* ****************************** */
/* *** function setLoopMode() *** */
/* ****************************** */
VOID setLoopMode( CHAR* cmd, UINT08 mode )
{
   STRING msg;
   sprintf( msg, "LOOP MODE - %s : MODE = %04X\n", cmd, mode );
   logAGG( TSKNUM, msg );

   /* IF OPERATE/NULL COMMAND IS ENABLED	*/
   if (mode & 0x10) 
   {
      sprintf( msg, "POST:%s.Status=1\n" );
      Post( msg, TSK_EXEC );
   }

   /* IF HOLD/ACTIVE COMMAND IS ENABLED */
   if (mode & 0x20) 
   {
      sprintf( msg, "POST:%s.Hold=%c\n", cmd, mode & 0x02 ? '0' : '1' );
      Post( msg, TSK_EXEC );
   }

   /* IF STEP COMMAND IS REQUESTED */
   if (mode & 0x04) 
   {
      sprintf( msg, "POST:%s.OutStepCmd=STEP\n", cmd );
      Post( msg, TSK_EXEC );
   }

} // end setLoopMode()

	
/* ************************************ */
/* *** function ProcessDAScommand() *** */
/* ************************************ */
void ProcessDAScommand(	DAS_DATA *d )
{
	// note see DAS_COMM.h at include folder for definition
	// of DAS_COMMAND_TYPE et. al.

	static  UINT16  commandSeqNumber = -1;
	DAS_COMMAND_TYPE  cmd;
	STRING  msg;
		
	cmd = (DAS_COMMAND_TYPE) d->DAScommandInput;

	sprintf(msg, "DAS CMD : (%d) %02X %02X %02X %02X %02X %02X %02X %02X\n", cmd,
		(UINT08)(d->EventDescription[0]),(UINT08)(d->EventDescription[1]),
		(UINT08)(d->EventDescription[2]),(UINT08)(d->EventDescription[3]),
		(UINT08)(d->EventDescription[4]),(UINT08)(d->EventDescription[5]),
		(UINT08)(d->EventDescription[6]),(UINT08)(d->EventDescription[7]));
	logAGG(TSKNUM, msg );

	// different sequence number implies a new command
	if (commandSeqNumber != d->CommandSeqNumber) {

	   switch( cmd ) {

	      case	RUNUP_PLATFORM :
		Post("POST:Platform.Control.Runup=1\n", TSK_EXEC );
	      break;

	      case	SHUTDOWN_PLATFORM :
		Post("POST:Platform.Control.Runup=2\n", TSK_EXEC );
		break;

	      case	START_RECORDING :
		if (Cmn->AGGlog.Recording == FALSE)
		   Post("POST:AGGlog.Command=START+RECORDING\n", TSK_EXEC );
		break;

	      case	STOP_RECORDING :
		if (Cmn->AGGlog.Recording == TRUE)
		   Post("POST:AGGlog.Command=STOP+RECORDING\n", TSK_EXEC );
		break;

	      case	EVENT_MARK :
		strcpy( (CHAR*)msg,"POST:AGGlog.EventMarkMsg=" );
		strncat ( (CHAR*)msg, d->EventDescription, 
		           sizeof( d->EventDescription ) );
		unwhite( (CHAR*)msg );
		strncat ( (CHAR*)msg, "\n", sizeof(STRING) );
		Post( (CHAR*)&msg, TSK_EXEC );
		break;

	      case	READ_FILE_HEADER :
		sprintf( msg, "READ_FILE:%s\n", SHARED_HEADER_FILE );
		Post( msg, TSK_EXEC ); break;

	      case	READ_GGI_CAL_FILE :
		sprintf( msg, "READ_FILE:%s\n", SHARED_GGI_CAL_FILE );
		Post( msg, TSK_EXEC ); break;

		case	READ_GGI_LOOP_FILE :
		sprintf( msg, "READ_FILE:%s\n", SHARED_GGI_LOO_FILE );
		Post( msg, TSK_EXEC ); break;

	      case	READ_GGI_HARM_FILE :
		sprintf( msg, "READ_FILE:%s\n", SHARED_GGI_HRM_FILE );
		Post( msg, TSK_EXEC ); break;

	      case	READ_GYRO_CAL_FILE :
		sprintf( msg, "READ_FILE:%s\n", SHARED_GYR_CAL_FILE );
		Post( msg, TSK_EXEC ); break;

	      case	READ_ACCEL_CAL_FILE :
		sprintf( msg, "READ_FILE:%s\n", SHARED_ACC_CAL_FILE );
		Post( msg, TSK_EXEC ); break;

	      case	READ_PLATFORM_MODES :
		sprintf( msg, "READ_FILE:%s\n", SHARED_PLT_PRM_FILE );
		Post( msg, TSK_EXEC ); break;

	      case	READ_SELF_GRAD_FILE :
		sprintf( msg, "READ_FILE:%s\n", SHARED_SGC_PRM_FILE );
		Post( msg, TSK_EXEC ); break;

	      case	READ_FBC_FILE :
		sprintf( msg, "READ_FILE:%s\n", SHARED_FBC_PRM_FILE );
		Post( msg, TSK_EXEC ); break;

	      case	READ_DEMOD_FILE :
		sprintf( msg, "READ_FILE:%s\n", SHARED_DMD_PRM_FILE );
		Post( msg, TSK_EXEC ); break;

	      case	READ_RTC_FILE :
		sprintf( msg, "READ_FILE:%s\n", SHARED_RTC_PRM_FILE );
		Post( msg, TSK_EXEC ); break;

	      case	RUNUP_GGI :
		Post("POST:GGI.Control.Runup=1\n", TSK_EXEC );
	      break;

	      case	SHUTDOWN_GGI :
		Post("POST:GGI.Control.Runup=2\n", TSK_EXEC );
	      break;

	      case	ENABLE_CAROUSELING :
		Post("POST:Platform.Control.CarouselMode=ENABLE\n", TSK_EXEC );
		Post("POST:Platform.Control.ApplyCarouselMode=APPLY\n", TSK_EXEC );
	      break;

	      case	DISABLE_CAROUSELING :
		Post("POST:Platform.Control.CarouselMode=DISABLE\n", TSK_EXEC );
		Post("POST:Platform.Control.ApplyCarouselMode=APPLY\n", TSK_EXEC );
	      break;

	      case	SET_SPIN_MODULATION_ON :
		Post("POST:GGI.Control.ShakeOmegaS=ON\n", TSK_EXEC );
		Post("POST:GGI.Control.ApplyGGIctrl=APPLY\n", TSK_EXEC );
	      break;

	      case	SET_SPIN_MODULATION_OFF :
		Post("POST:GGI.Control.ShakeOmegaS=OFF\n", TSK_EXEC );
		Post("POST:GGI.Control.ApplyGGIctrl=APPLY\n", TSK_EXEC );
	      break;

	      case	SET_AXIAL_SHAKE_ON :
		Post("POST:Shake.Config.State=ENABLE\n", TSK_EXEC );
	      break;

	      case	SET_AXIAL_SHAKE_OFF :
		Post("POST:Shake.Config.State=DISABLE\n", TSK_EXEC );
	      break;

	      case	ENABLE_DYNAMIC_MAN :
		Post("POST:GGI.Control.DynMan=ON\n", TSK_EXEC );
	      break;

	      case	DISABLE_DYNAMIC_MAN :
		Post("POST:GGI.Control.DynMan=OFF\n", TSK_EXEC );
	      break;

	      case	SET_NAV_FIELD_DYN :
		Post("POST:Platform.Control.NavModeCmd=FIELD/DYN\n", TSK_EXEC );
	      break;

	      case	SET_NAV_FIELD_STATIC :
		Post("POST:Platform.Control.NavModeCmd=FIELD/STATIC\n", TSK_EXEC );
	      break;

	      case	SET_NAV_LAB_DYN :
		Post("POST:Platform.Control.NavModeCmd=LAB/DYN\n", TSK_EXEC );
	      break;

	      case	SET_NAV_LAB_STATIC :
		Post("POST:Platform.Control.NavModeCmd=LAB/STATIC\n", TSK_EXEC );
	      break;

	      case	SET_PLATFORM_MODE : 
		   sprintf( msg, "POST:Platform.Control.LRmode=%d\n", d->EventDescription[0] );
		   Post( msg, TSK_EXEC ); 
		break;

	      case	SET_CAROUSEL_RATE :
               sprintf( msg, "POST:Platform.Control.CarouselRate=%d\n", 
			(UINT08)d->EventDescription[0] + ((UINT08)d->EventDescription[1] << 8) );
		   Post( msg, TSK_EXEC ); 
               Post("POST:Platform.Control.ApplyCarouselRate=APPLY\n", TSK_EXEC );
	      break;

	      case	SET_GGI_LOOP_MODES :
               setLoopMode( "GGI.Control.Omega1_A", d->EventDescription[0] );
               setLoopMode( "GGI.Control.Omega1_B", d->EventDescription[4] );
               Post( "POST:GGI.Control.ApplyOmega1=APPLY\n", TSK_EXEC );

               setLoopMode( "GGI.Control.Omega3_A", d->EventDescription[1] );
               setLoopMode( "GGI.Control.Omega3_B", d->EventDescription[5] );
               Post( "POST:GGI.Control.ApplyOmega3=APPLY\n", TSK_EXEC );

               setLoopMode( "GGI.Control.OmegaS_A", d->EventDescription[2] );
               setLoopMode( "GGI.Control.OmegaS_B", d->EventDescription[6] );
               Post( "POST:GGI.Control.ApplyOmegaS=APPLY\n", TSK_EXEC );

               setLoopMode( "GGI.Control.OmegaA_A", d->EventDescription[3] );
               setLoopMode( "GGI.Control.OmegaA_B", d->EventDescription[7] );
               Post( "POST:GGI.Control.ApplyOmegaA=APPLY\n", TSK_EXEC );         

		   // Axial Sin and Cos are Not Implemented (Bytes 8 and 9)
	      break;

	      case	SET_GGI_HARM_MODES :
               setLoopMode( "GGI.Control.HN_DCbias_A", d->EventDescription[0] );
               setLoopMode( "GGI.Control.HN_DCbias_B", d->EventDescription[19] );
               Post( "POST:GGI.Control.ApplyHN[0]=APPLY\n", TSK_EXEC );         

               setLoopMode( "GGI.Control.HN_1Omega.Sin_A", d->EventDescription[1] );
               setLoopMode( "GGI.Control.HN_1Omega.Cos_A", d->EventDescription[2] );
               setLoopMode( "GGI.Control.HN_1Omega.Sin_B", d->EventDescription[20] );
               setLoopMode( "GGI.Control.HN_1Omega.Cos_B", d->EventDescription[21] );
               Post( "POST:GGI.Control.ApplyHN[1]=APPLY\n", TSK_EXEC );         

               setLoopMode( "GGI.Control.HN_2Omega.Sin_A", d->EventDescription[3] );
               setLoopMode( "GGI.Control.HN_2Omega.Cos_A", d->EventDescription[4] );
               setLoopMode( "GGI.Control.HN_2Omega.Sin_B", d->EventDescription[22] );
               setLoopMode( "GGI.Control.HN_2Omega.Cos_B", d->EventDescription[23] );
               Post( "POST:GGI.Control.ApplyHN[2]=APPLY\n", TSK_EXEC );         

               setLoopMode( "GGI.Control.HN_3Omega.Sin_A", d->EventDescription[5] );
               setLoopMode( "GGI.Control.HN_3Omega.Cos_A", d->EventDescription[6] );
               setLoopMode( "GGI.Control.HN_3Omega.Sin_B", d->EventDescription[24] );
               setLoopMode( "GGI.Control.HN_3Omega.Cos_B", d->EventDescription[25] );
               Post( "POST:GGI.Control.ApplyHN[3]=APPLY\n", TSK_EXEC );         

               setLoopMode( "GGI.Control.HN_4Omega.Sin_A", d->EventDescription[7] );
               setLoopMode( "GGI.Control.HN_4Omega.Cos_A", d->EventDescription[8] );
               setLoopMode( "GGI.Control.HN_4Omega.Sin_B", d->EventDescription[26] );
               setLoopMode( "GGI.Control.HN_4Omega.Cos_B", d->EventDescription[27] );
               Post( "POST:GGI.Control.ApplyHN[4]=APPLY\n", TSK_EXEC );         

               setLoopMode( "GGI.Control.HN_5Omega.Sin_A", d->EventDescription[9] );
               setLoopMode( "GGI.Control.HN_5Omega.Cos_A", d->EventDescription[10] );
               setLoopMode( "GGI.Control.HN_5Omega.Sin_B", d->EventDescription[28] );
               setLoopMode( "GGI.Control.HN_5Omega.Cos_B", d->EventDescription[29] );
               Post( "POST:GGI.Control.ApplyHN[5]=APPLY\n", TSK_EXEC );         

               setLoopMode( "GGI.Control.HN_6Omega.Sin_A", d->EventDescription[11] );
               setLoopMode( "GGI.Control.HN_6Omega.Cos_A", d->EventDescription[12] );
               setLoopMode( "GGI.Control.HN_6Omega.Sin_B", d->EventDescription[30] );
               setLoopMode( "GGI.Control.HN_6Omega.Cos_B", d->EventDescription[31] );
               Post( "POST:GGI.Control.ApplyHN[6]=APPLY\n", TSK_EXEC );         

               setLoopMode( "GGI.Control.HN_7Omega.Sin_A", d->EventDescription[13] );
               setLoopMode( "GGI.Control.HN_7Omega.Cos_A", d->EventDescription[14] );
               setLoopMode( "GGI.Control.HN_7Omega.Sin_B", d->EventDescription[32] );
               setLoopMode( "GGI.Control.HN_7Omega.Cos_B", d->EventDescription[33] );
               Post( "POST:GGI.Control.ApplyHN[7]=APPLY\n", TSK_EXEC );         

               setLoopMode( "GGI.Control.HN_8Omega.Sin_A", d->EventDescription[15] );
               setLoopMode( "GGI.Control.HN_8Omega.Cos_A", d->EventDescription[16] );
               setLoopMode( "GGI.Control.HN_8Omega.Sin_B", d->EventDescription[34] );
               setLoopMode( "GGI.Control.HN_8Omega.Cos_B", d->EventDescription[35] );
               Post( "POST:GGI.Control.ApplyHN[8]=APPLY\n", TSK_EXEC );         

               setLoopMode( "GGI.Control.HN_9Omega.Sin_A", d->EventDescription[17] );
               setLoopMode( "GGI.Control.HN_9Omega.Cos_A", d->EventDescription[18] );
               setLoopMode( "GGI.Control.HN_9Omega.Sin_B", d->EventDescription[36] );
               setLoopMode( "GGI.Control.HN_9Omega.Cos_B", d->EventDescription[37] );
               Post( "POST:GGI.Control.ApplyHN[9]=APPLY\n", TSK_EXEC );         
	      break;

	      case	SET_GGI_SUPP_MODES :
               setLoopMode( "GGI.Control.ClosedLoop1.A", d->EventDescription[0] );
               setLoopMode( "GGI.Control.ClosedLoop1.B", d->EventDescription[0] );
               Post( "POST:GGI.Control.ApplyFBC[0]=APPLY\n", TSK_EXEC );         

               setLoopMode( "GGI.Control.ClosedLoop1.A", d->EventDescription[0] );
               setLoopMode( "GGI.Control.ClosedLoop1.B", d->EventDescription[12] );
               Post( "POST:GGI.Control.ApplyFBC[0]=APPLY\n", TSK_EXEC );         

               setLoopMode( "GGI.Control.ClosedLoop2.A", d->EventDescription[1] );
               setLoopMode( "GGI.Control.ClosedLoop2.B", d->EventDescription[13] );
               Post( "POST:GGI.Control.ApplyFBC[1]=APPLY\n", TSK_EXEC );         

               setLoopMode( "GGI.Control.ClosedLoop3.A", d->EventDescription[2] );
               setLoopMode( "GGI.Control.ClosedLoop3.B", d->EventDescription[14] );
               Post( "POST:GGI.Control.ApplyFBC[2]=APPLY\n", TSK_EXEC );         

               setLoopMode( "GGI.Control.ClosedLoop4.A", d->EventDescription[3] );
               setLoopMode( "GGI.Control.ClosedLoop4.B", d->EventDescription[15] );
               Post( "POST:GGI.Control.ApplyFBC[3]=APPLY\n", TSK_EXEC );         

               setLoopMode( "GGI.Control.ClosedLoop5.A", d->EventDescription[4] );
               setLoopMode( "GGI.Control.ClosedLoop5.B", d->EventDescription[16] );
               Post( "POST:GGI.Control.ApplyFBC[4]=APPLY\n", TSK_EXEC );         

               setLoopMode( "GGI.Control.ClosedLoop6.A", d->EventDescription[5] );
               setLoopMode( "GGI.Control.ClosedLoop6.B", d->EventDescription[17] );
               Post( "POST:GGI.Control.ApplyFBC[5]=APPLY\n", TSK_EXEC );         

               setLoopMode( "GGI.Control.ClosedLoop7.A", d->EventDescription[6] );
               setLoopMode( "GGI.Control.ClosedLoop7.B", d->EventDescription[18] );
               Post( "POST:GGI.Control.ApplyFBC[6]=APPLY\n", TSK_EXEC );         

               setLoopMode( "GGI.Control.ClosedLoop8.A", d->EventDescription[7] );
               setLoopMode( "GGI.Control.ClosedLoop8.B", d->EventDescription[19] );
               Post( "POST:GGI.Control.ApplyFBC[7]=APPLY\n", TSK_EXEC );         

               setLoopMode( "GGI.Control.ClosedLoop9.A", d->EventDescription[8] );
               setLoopMode( "GGI.Control.ClosedLoop9.B", d->EventDescription[20] );
               Post( "POST:GGI.Control.ApplyFBC[8]=APPLY\n", TSK_EXEC );         

               setLoopMode( "GGI.Control.ClosedLoop10.A", d->EventDescription[9] );
               setLoopMode( "GGI.Control.ClosedLoop10.B", d->EventDescription[21] );
               Post( "POST:GGI.Control.ApplyFBC[9]=APPLY\n", TSK_EXEC );         

               setLoopMode( "GGI.Control.ClosedLoop11.A", d->EventDescription[10] );
               setLoopMode( "GGI.Control.ClosedLoop11.B", d->EventDescription[22] );
               Post( "POST:GGI.Control.ApplyFBC[10]=APPLY\n", TSK_EXEC );         

               setLoopMode( "GGI.Control.ClosedLoop12.A", d->EventDescription[11] );
               setLoopMode( "GGI.Control.ClosedLoop12.B", d->EventDescription[23] );
               Post( "POST:GGI.Control.ApplyFBC[11]=APPLY\n", TSK_EXEC );
	      break;

	      default : break;
	   }

	   commandSeqNumber = d->CommandSeqNumber;

	} // end if (commandSeqNumber != d->CommandSeqNumber)

} // end processDAScommand()


/* ********************************* */
/* *** function SendDASrequest() *** */
/* ********************************* */
void SendDASrequest( AGG_REQUEST *r )
{
  // called from signal handler at 1Hz
  UINT08* ptr;
  UINT08  buffer[512];
	UINT08	byteSum = 0;
	UINT08	bitsSum = 0;	
	INT32   i;
	INT16   msgLen = sizeof(AGG_REQUEST);
	INT32   idx = 0;
	INT32   retval;

  ptr = (UINT08 *)r;

  buffer[idx] = DLE;
  ++idx;
  buffer[idx] = STX;
  ++idx;
  buffer[idx] = msgLen & 0x00FF;
  ++idx;
  buffer[idx] = (msgLen & 0xFF00) >> 8;
  ++idx;

	for (i = 0; i < msgLen; i++)
	{
	  if(ptr[i] == DLE)
	  {
	    buffer[idx] = DLE;
	    ++idx;
	  }

	  byteSum += ptr[i];
	  bitsSum ^= ptr[i];
		
	  buffer[idx] = ptr[i];
	  ++idx;
	}
	
	buffer[idx] = DLE;
	++idx;
  buffer[idx] = ETX;
  ++idx;  

	byteSum += ETX;
	bitsSum ^= ETX;
	
	buffer[idx] = byteSum;
	++idx;
	buffer[idx] = bitsSum;
	++idx;

	retval = write_bytes(buffer, idx);
	if (retval != idx)
	{
	  sprintf(logMsg, "ERROR WRITING ADAS MESSAGE %d\n", retval);
	  logAGG(TSKNUM, logMsg);
	}
}


/* ****************************************** */
/* *** function UpdateGPSPosVelAltTimes() *** */
/* ****************************************** */
/* 
   DSPR 63  1/18/2012 gcl / mjs

   Moved the Position, Velocity and Altitude GPS time update to here for 
   nominal 1Hz updates.  Previously existed in GPSClockSync()

*/
void UpdateGPSPosVelAltTimes( DAS_DATA* d,  UINT32 systemClock )
{
    DOUBLE  now;
    DOUBLE  offsetSum;
    STRING  msg;

    now = MakeSystemTime( systemClock ) + Cmn->AGGclock.TimeOffset;

    /*  *   SUM RECEIVER CLOCK OFFSET AND UTC OFFSET FOR UTC TIMES	*/
    offsetSum = d->GPSdata.RcvClockOffset + d->GPSdata.UTCoffset;

    /*  *   SAVE UTC TIMES AFTER ADJUSTING FOR OFFSETS			*/
    Cmn->DASdata.PositionUTCtime =  d->GPSdata.PositionGPStime - offsetSum;
    Cmn->DASdata.VelocityUTCtime =  d->GPSdata.VelocityGPStime - offsetSum;
    Cmn->DASdata.AltitudeUTCtime =  d->GPSdata.AltitudeGPStime - offsetSum;

	if ( ( Cmn->DASdata.PositionUTCtime  > now ) ||
	     ( Cmn->DASdata.VelocityUTCtime  > now ) ||
	     ( Cmn->DASdata.AltitudeUTCtime  > now ) ) {
           sprintf( msg, "UTC clock trouble. now = %lf\nPosition = %lf\nVelocity = %lf\nAltitudeUTC = %lf\n",
                    now,
	            Cmn->DASdata.PositionUTCtime,
	            Cmn->DASdata.VelocityUTCtime,
	            Cmn->DASdata.AltitudeUTCtime );
           logAGG( TSKNUM, msg );
           /*  *  *   INFORM PLATFORM NOT TO USE DATA UNTIL TIME SYNC IS DONE	*/
	   Cmn->DASdata.DataValidity.bits.OldGPSdata = TRUE;
	}

	/*   CALCULATE AGG TIMES USING UTC TIME AND AGG OFFSET		*/
	Cmn->DASdata.PositionAGGtime =  
		Cmn->DASdata.PositionUTCtime - Cmn->DASdata.AGGoffset;
	Cmn->DASdata.VelocityAGGtime = 
		Cmn->DASdata.VelocityUTCtime - Cmn->DASdata.AGGoffset;
	Cmn->DASdata.AltitudeAGGtime =
		Cmn->DASdata.AltitudeUTCtime - Cmn->DASdata.AGGoffset;

} /* end UpdateGPSPosVelAltTimes() */


/* ******************************* */
/* *** function GPSclockSync() *** */
/* ******************************* */
/* 
   DSPR 63  1/18/2012 gcl / mjs

   Reoved the Position, Velocity and Altitude GPS time updates to 
   new function UpdateGPSPosVelAltTimes()

*/
static	DOUBLE	OldUTCoffset = 0;
static	UINT16	GPSupdateNumber = 0;	
GPSclockSync( DAS_DATA* d,  UINT32 systemClock )
{
	DOUBLE	markedAGGclock;
	DOUBLE	now;
	DOUBLE	offsetSum;
	DOUBLE	timeDifference;
	DOUBLE	newAGGtime;
	DOUBLE	deltaUTCoffset;	
	DOUBLE	oldAGGoffset;
	DOUBLE	timeOffset;
	STRING	timeMsg;
	STRING	msg;

	/* CHECK AGE OF LAST GPS DATA */
	// GPSdataAge is incremented at 1Hz in the signal handler
	if ( GPSdataAge >= GPS_AGE_LIMIT ) // 10 minutes
	{
	   /* RESET AGE FILTER ALGORITHM WHEN THE DATA GETS TOO OLD */
	   GPSupdateNumber = 0;
	   /* INFORM PLATFORM NOT TO USE DATA UNTIL TIME SYNC IS DONE */
	   Cmn->DASdata.DataValidity.bits.OldGPSdata = TRUE;
	}
	GPSdataAge = 0;

	/* 9/28/2010 gcl
	  The SCP hardware sends a sync pulse to the ADAS every 4 seconds,
	  when the [ system clock counter mod 4 seconds ] is 0
	*/

	/* GET VALUE OF SYSTEM CLOCK */
		//  MODULO_4_SECONDS (0x00003FFF)
		// ~MODULO_4_SECONDS (0xFFFFC000)
	// compute AGG clock at last 4 sec time mark, when pulse was generated
	markedAGGclock = MakeSystemTime( systemClock & ~MODULO_4_SECONDS );
	now = MakeSystemTime( systemClock ) + Cmn->AGGclock.TimeOffset;

	/* SUM RECEIVER CLOCK OFFSET AND UTC OFFSET FOR UTC TIMES */
	offsetSum = d->GPSdata.RcvClockOffset + d->GPSdata.UTCoffset;

	/* SAVE MARKED UTC TIME AFTER ADJUSTING FOR OFFSETS */
	Cmn->DASdata.MarkedUTCtime   =  d->GPSdata.MarkedGPStime - offsetSum;
	timeDifference = Cmn->DASdata.MarkedUTCtime - markedAGGclock;

	if ( ( Cmn->DASdata.MarkedUTCtime > now ) ||
	     ( timeDifference < -3.0 ) ) 
	{
           sprintf( msg, "Marked UTC time trouble. Time Difference is %lf\nnow = %lf\nMarkedUTC = %lf\n",
              timeDifference, 
	      now,
	      Cmn->DASdata.MarkedUTCtime );
           logAGG( TSKNUM, msg );
           Cmn->DASdata.DataValidity.bits.OldGPSdata = TRUE;
	}

	/* CALCULATE ANY CHANGE IN UTC OFFSET (RARE). SAVE LAST VALUE */
	deltaUTCoffset = OldUTCoffset - Cmn->DASdata.GPSdata.UTCoffset;
	OldUTCoffset = Cmn->DASdata.GPSdata.UTCoffset;

	/* RECOMPUTE AGG TIME OFFSET ( aka drift ) USING AVERAGING ALGORITHM (FROM PPS)	*/
	if (GPSupdateNumber < 10)
	{
	  GPSupdateNumber++;
	}
	oldAGGoffset = Cmn->DASdata.AGGoffset;
	timeOffset = Cmn->AGGclock.TimeOffset + deltaUTCoffset + 
	               ((timeDifference - Cmn->AGGclock.TimeOffset) / GPSupdateNumber);

	/* ALWAYS APPLY NEW TIME OFFSET FROM FILTER DURING INITIAL UPDATES */
	if (GPSupdateNumber < 10) 
	{
		Cmn->DASdata.AGGoffset = Cmn->AGGclock.TimeOffset = timeOffset; 
	}
	/* OTHERWISE, CHECK NEW TIME OFFSET FOR SANITY	*/
	else 
	{
	   if ( fabs(oldAGGoffset - timeOffset) < 0.1 ) 
	   {
	      Cmn->DASdata.AGGoffset = 
	        Cmn->AGGclock.TimeOffset = timeOffset; 
	   } 
	   else 
	   {
	      sprintf( msg, "Significant time offset jump from %lf to %lf\n",  oldAGGoffset, timeOffset) ;
	      logAGG( TSKNUM, msg );
	   } 
	} // end if GPSupdateNumber 

#if (0) /* JJK Debug code */ 
{
FILE* fp;
   if ( (fp = fopen("/scp/log/UTCdata", "a+")) != NULL ) {
     fprintf(fp, "%08X   %10.8f   %10.8f   %10.8f\n", 
           systemClock,
           markedAGGclock,  
	   Cmn->DASdata.MarkedUTCtime,
           timeOffset ); 
     fclose(fp);
   }
}
#endif

	/* CALCULATE MARKED AGG TIME USING UTC TIME AND AGG OFFSET */
	Cmn->DASdata.MarkedAGGtime   = 
		Cmn->DASdata.MarkedUTCtime   - Cmn->DASdata.AGGoffset;

} // end GPSclockSync ()


/* **************************************** */
/* Quick Routine to set PC SW and HW Clocks */
/* **************************************** */
VOID SetPCclock( DOUBLE now ) 
{
   STRING msg;
   struct timeval tv;

   tv.tv_sec = now;
   tv.tv_usec = (now - tv.tv_sec) * 1000000;
   sprintf( msg, "Adjusting PC Clock to %s", ctime( &tv.tv_sec ) );
   logAGG ( TSKNUM, msg );
   settimeofday( &tv, NULL );
   system( "/sbin/hwclock -w \n" );
}


/*
**************************************************************************
  MAIN()
**************************************************************************
*/
static BOOLEAN ValidUTCdataReceived = FALSE;
int main(int argc, char *argv[])
{
  BYTE		buf[512];
  DAS_DATA	*dptr = (DAS_DATA*)&buf[0];
  STRING	logCmd;

  UINT16	i;
  STRING	msg;
  UINT32	pri;
  UINT32	systemClock;

  DOUBLE	nowSCP;
  DOUBLE	nowUTC;
  DOUBLE	timeSinceMark;
  UINT16	strobeCounter = 0;
  INT32         retval;

  // DSPR 63 : 1/18/2012 gcl / mjs
  UINT32        clockGPSPos;

  /* Intercept all interceptable signals */
  for (i = 0; i < 64; i++) 
  {
    signal( i, handler );
  }

  Cmn = getSharedMem();

  pri = setPriority( TSKPRI );
  sprintf( msg, "Task %d running at priority %d\n", TSKNUM, pri );
  logAGG( TSKNUM, msg );

  Cmn->Tskdat[TSKNUM].TaskNumber = TSKNUM;
  Cmn->Tskdat[TSKNUM].TID = getpid();

  /* SET TIME OFFSET TO 0. WHEN GPS DATA ARRIVES THIS GETS ADJUSTED */
  Cmn->AGGclock.TimeOffset = 0;

  // open communications channel
  retval = configure_DAS_port();
  if( retval < 0 )
  {
    sprintf( logMsg, "ERROR opening serial port (%d)\n", retval );
    logAGG(TSKNUM, logMsg);
    exit(-1);
  }
  else
  {
    sprintf( logMsg, "serial port open (%d)\n", retval );
    logAGG(TSKNUM, logMsg);
  }


  /* SET INTERVAL TIMER TO 1 HZ */
  // DSPR #45  11/8/2010 gcl
  set_iTimer();

  Cmn->DASdata.CommWDT = 0;
  FOREVER
  {
    /* BUMP THE WATCHDOG TO LET EVERYONE KNOW WE'RE ALIVE */
    Cmn->Tskdat[TSKNUM].Watchdog++;
    memset( buf, 0, sizeof(buf) );
    msgStat = GetDASresponse( buf );
    systemClock = GetSystemClock();

    /* IF ALL APPEARS WELL WITH MESSAGE FROM DAS, STORE THE DATA */
    if ( !msgStat.bits.MsgError &&
         !msgStat.bits.XORsumError && 
         !msgStat.bits.ByteSumError &&
         !msgStat.bits.TimeoutError )
    {
      /* IF MESSAGE IS GOOD, RESET COMM WATCHDOG (10.0 sec) */
      Cmn->DASdata.CommWDT = 100;

      if ((UINT08)buf[0] != ++DASseq)
      {
         sprintf( msg, "DAS Sequence Error : %02X\n", buf[0] & 0x00FF );
         logAGG(TSKNUM, msg);
         msgStat.bits.SeqNumError = 1;
         DASseq = buf[0];
      }

      /* STORE THE DATA in common memory */
      memcpy( &Cmn->DASdata, buf, sizeof(DAS_MSG_DATA) );
      validity = dptr->DataValidity;

      /* IF NEW GPS TIME VALIDITY FLAG IS SET */
      if (validity.bits.NewStrobeTime) 
      {
        /* 9/28/2010 gcl
           The SCP hardware sends a sync pulse to the ADAS every 4 seconds,
           when the [ system clock counter mod 4 seconds ] is 0
        */
        GPSclockSync( dptr, systemClock  );

        timeSinceMark = (systemClock & MODULO_4_SECONDS) * (1.0/4096.0);
        nowUTC = timeSinceMark + Cmn->DASdata.MarkedUTCtime;

        /* IF THIS IS THE FIRST UTC TIME RECEIVED */
        if ( !ValidUTCdataReceived ) 
        {
           GPSupdateNumber = 0;
           ValidUTCdataReceived = TRUE;

           // 4/6/2010 gcl
           //  library function was simplified in MLGS / MT
           //  SyncSystemClock ( nowUTC );
           SyncSystemClock ( systemClock, nowUTC );

           SetPCclock( GPS_TO_UNIX_TIME(nowUTC) );
           sprintf( msg, "EVENT:%d\n", CLOCK_ADJUST_EVENT );
           Post( msg, TSK_EXEC );
        } 
        else 
        {
          // do nothing
        }
      } // end if validity.bits.NewStrobeTime


      // DSPR 63 : 1/18/2012 gcl / mjs
      //    update to determine GPS times of position, velocity, altitude with 
      //    every DAS message receipt.
      UpdateGPSPosVelAltTimes( dptr, systemClock );

      // DSPR 63 : 1/18/2012 gcl / mjs
      //    for platform GPS coordinate extrapolation calculation, 
      //    changed to use clock count value at GPS time of position
      //    instead of using the clock count value at time of DAS message receipt.
      clockGPSPos = MakeSystemClock( Cmn->DASdata.PositionAGGtime );
      sprintf( msg, "DAS_DATA:%08X %04X\n", clockGPSPos, *(UINT08*)&validity & 0x0000FFFF );
      Post( msg, TSK_EXEC );
      Post( msg, TSK_PLT_INTERFACE );
      Post( msg, TSK_DATALOG );

      /* IS THERE A COMMAND PRESENT WITHIN THE MESSAGE? */
      if ( validity.bits.CommandPresent )
      {
        ProcessDAScommand(dptr);
      }
    }
    else
    { 
      // one of the 'error' bits was set 
      sprintf( msg, "DAS_IF: MSG ERROR 0x%x\n", msgStat.word );
      logAGG(TSKNUM, msg);
    } // end if !msgStat.bits

  } // end FOREVER

  return 0;

} // end main

