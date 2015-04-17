
/****************************************************************************

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

         COPYRIGHT - LOCKHEED MARTIN, WHEATFIELD, NEW YORK, 2002

****************************************************************************

   DIGITAL AIRBORNE GRAVITY GRADIOMETER SYSTEM

   LOCKHEED MARTIN NAVAL ELECTRONICS & SURVEILLANCE SYSTEMS -
     UNDERSEA SYSTEMS - NIAGARA
   2221 Niagara Falls Boulevard
   Niagara Falls, New York  14304

   FILE NAME       : DAS

   ID Number       : 
   Initial Release : Month, Year <date of initial release, version 1.0>

  
   *** REVISION HISTORY ***

   Revision     : 
   Release date : 
   PPRs         : 
   Change description:
     


*****************************************************************************/

////*************************************************************************
//
// Platform:	Linux 80x86 PC
//
// Module File Name: DAS.h	
//

//written by J.J.Kuhn,Jr. 02/07/02
//Temporary development revision 0

//Build/Release 


//*******************************************************************************
//
// NOTICE:	Contains confidential and proprietary information of Lockheed 
//			Martin Co. and is protected under the copyright laws
//			as an unpublished work of Lockheed Martin Corp.
//
//*****************************************************************************/

/* Module Description:  This file is a header to define variables and structures
   to be used by the RS232 serial interface between the SCP and the DAS.             */

//*****************************************************************************/

#ifndef _DAS_H
#define _DAS_H


typedef union {
	struct {
		BITS	ByteSumError    : 1;
		BITS	XORsumError     : 1;
		BITS	SeqNumError     : 1;
		BITS	TimeoutError    : 1;
		BITS	BITfailureError : 1;
		BITS	RecordingOn     : 1;
		BITS	Spare           : 1;
		BITS	MsgError        : 1;
	} bits;
	UINT08 word;
} DAS_MSG_STATUS;

typedef struct {
	UINT08		SeqNumber;
	UINT08		AckSeqNumber;

//	DAS_MSG_STATUS	Stat;
	BYTE		Stat;


	CHAR		ASCIImsg[80];
} AGG_REQUEST;


typedef union {
	struct {
		BITS	GPSdataValid 		: 1;
		BITS	BarAltDataValid 	: 1;
		BITS	FuelDataValid 		: 1;
		BITS	HeadingDataValid   	: 1;
		BITS	CommandPresent 		: 1;
		BITS	NewStrobeTime 		: 1;
		BITS	NewGPStime 	       	: 1;
		BITS	OldGPSdata 	       	: 1; // FOR SCP INTERNAL USE ONLY !
	} bits;
	UINT08 byte;
} DAS_DATA_VALIDITY;

typedef ENUM { 
	INVALID_TEMP = 0, 
	INVALID_PERM = 1, 
	VALID = 2, 
	DIFFERENTIAL = 3 
} DAS_GPS_SOLUTION_TYPE;

typedef struct {
	DOUBLE			Latitude;
	DOUBLE			Longitude;
	DOUBLE			HeightMSL;
	DOUBLE			Undulation;
	DOUBLE			SDLatitude;
	DOUBLE			SDLongitude;
	DOUBLE			SDHeightMSL;
	DOUBLE			PositionGPStime;
	DOUBLE			RcvClockOffset;
	DOUBLE			UTCoffset;
	DOUBLE			AgeOfData;
	DOUBLE			HorzSpeed;
	DOUBLE			VertSpeed;
	DOUBLE			GroundAngle;
	DOUBLE			VelocityGPStime;
	UINT16			ConstSeqNumber;
	UINT32			SolutionStatus;
	UINT32			VelocityStatus;
	DOUBLE			AltitudeGPStime;
	DOUBLE			RawBaroAltitude;
	DOUBLE			CorrectedAltitude;
	BYTE			AltitudeSeqNumber;
	DOUBLE			MarkedGPStime;
} DAS_GPS_DATA;

typedef struct {
	UINT08			FuelLevel01;
	UINT08			FuelLevel02;
	UINT08			FuelLevel03;
	UINT08			FuelLevel04;
	UINT08			FuelLevel05;
	UINT08			FuelLevel06;
	UINT08			FuelLevel07;
	UINT08			FuelLevel08;
	UINT08			FuelLevel09;
	UINT08			FuelLevel10;
	UINT08			FuelLevel11;
	UINT08			FuelLevel12;
} DAS_FUEL_DATA;



typedef struct {
	BYTE			SeqNumber;
	BYTE			AckSeqNumber;
	BYTE			ErrorMsg;
	DAS_DATA_VALIDITY	DataValidity;
	DAS_GPS_DATA		GPSdata;
	DAS_FUEL_DATA		FuelData;
	DOUBLE			Heading;
	UINT08			DAScommandInput;
	UINT08			CommandSeqNumber;
	DOUBLE			EventMarkTime;
	SINGLE			EventDescripDelay;
	CHAR			EventDescription[60];
} DAS_MSG_DATA;

typedef struct {
	BYTE			SeqNumber;
	BYTE			AckSeqNumber;
	BYTE			ErrorMsg;
	DAS_DATA_VALIDITY	DataValidity;
	DAS_GPS_DATA		GPSdata;
	DAS_FUEL_DATA		FuelData;
	DOUBLE			Heading;
	UINT08			DAScommandInput;
	UINT08			CommandSeqNumber;
	DOUBLE			EventMarkTime;
	SINGLE			EventDescripDelay;
	CHAR			EventDescription[60];
	DOUBLE			PositionUTCtime;
	DOUBLE			PositionAGGtime;
	DOUBLE			VelocityUTCtime;
	DOUBLE			VelocityAGGtime;
	DOUBLE			MarkedUTCtime;
	DOUBLE			MarkedAGGtime;
	DOUBLE			AltitudeUTCtime;
	DOUBLE			AltitudeAGGtime;
	DOUBLE			AGGoffset;
        UINT32			CommWDT;
} DAS_DATA;


typedef enum {
	RUNUP_PLATFORM 		= 0x00,
	SHUTDOWN_PLATFORM 	= 0x01,
	START_RECORDING 	= 0x02,
	STOP_RECORDING 		= 0x03,
	EVENT_MARK 		= 0x04,
	READ_FILE_HEADER 	= 0x05,
	READ_GGI_CAL_FILE 	= 0x06,
	READ_GGI_LOOP_FILE 	= 0x07,
	READ_GGI_HARM_FILE 	= 0x08,
	READ_GYRO_CAL_FILE 	= 0x09,
	READ_ACCEL_CAL_FILE 	= 0x0A,
	READ_PLATFORM_MODES 	= 0x0B,
	READ_SELF_GRAD_FILE	= 0x0C,
	READ_FBC_FILE		= 0x0D,
	READ_DEMOD_FILE		= 0x0E,
	READ_RTC_FILE		= 0x0F,
	RUNUP_GGI		= 0x10,
	SHUTDOWN_GGI		= 0x11,
	ENABLE_CAROUSELING	= 0x12,
	DISABLE_CAROUSELING	= 0x13,
	SET_SPIN_MODULATION_ON	= 0x14,
	SET_SPIN_MODULATION_OFF	= 0x15,
	SET_AXIAL_SHAKE_ON	= 0x16,
	SET_AXIAL_SHAKE_OFF	= 0x17,
	ENABLE_DYNAMIC_MAN	= 0x18,
	DISABLE_DYNAMIC_MAN	= 0x19,
	SET_NAV_FIELD_DYN	= 0x1A,
	SET_NAV_FIELD_STATIC	= 0x1B,
	SET_NAV_LAB_DYN		= 0x1C,
	SET_NAV_LAB_STATIC	= 0x1D,

	SET_PLATFORM_MODE	= 0x20,
	SET_CAROUSEL_RATE	= 0x21,

	SET_GGI_LOOP_MODES	= 0x30,
	SET_GGI_HARM_MODES	= 0x31,
	SET_GGI_SUPP_MODES	= 0x32,

} DAS_COMMAND_TYPE;

#endif
