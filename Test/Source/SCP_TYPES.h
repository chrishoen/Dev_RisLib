
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

   FILE NAME       : SCP_TYPES
   ID Number       : 
   Initial Release : Month, Year <date of initial release, version 1.0>

  
   *** REVISION HISTORY ***

   Revision     : 
   Release date : 
   PPRs         : 
   Change description:
     


*****************************************************************************/

////****************************************************************************
//
// Platform:	Linux 80x86 PC
//
// Module File Name: SCP_TYPES.h //

//written by J.J.Kuhn,Jr. 02/07/02
//Temporary development revision 0

//Build/Release 


//*******************************************************************************
//
// NOTICE:	Contains confidential and proprietary information of Lockheed 
//			Martin Co. and is protected under the copyright laws
//			as an unpublished work of Lockheed Martin Corp.
//
//*******************************************************************************/

/* Module Description:  This file contains the system control processor Global
   Data type Definitions.   */
//*******************************************************************************/

#ifndef _SCP_TYPES_H
#define _SCP_TYPES_H

// #include <pthread.h>

#pragma pack (1)


#define ENUM		enum

typedef	void		VOID;

typedef unsigned	int	BITS;
typedef signed		char	CHAR;

typedef signed		char	BYTE;
typedef signed		char	INT08;
typedef signed	 short	int	INT16;
typedef signed	 long	int	INT32;

typedef unsigned	char	UBYTE;
typedef unsigned	char	UINT08;
typedef unsigned short	int	UINT16;
typedef unsigned long	int	UINT32;	

typedef 		float	SINGLE;
typedef 		double	DOUBLE;

typedef			CHAR	STRING[256];

typedef ENUM 	{ OFF = 0, ON = 1 } BOOLEAN;

typedef struct {
	DOUBLE			roll;
	DOUBLE			pitch;
	DOUBLE			heading;
} ANG_VECTOR_TYPE;

typedef struct {
	DOUBLE			X;
	DOUBLE			Y;
	DOUBLE			Z;
} XYZ_VECTOR_TYPE;

typedef struct {
	DOUBLE			X;
	DOUBLE			Y;
	DOUBLE			Z;
	DOUBLE			RA;
} XYZ_RA_VECTOR_TYPE;

typedef struct {
	DOUBLE			U;
	DOUBLE			V;
	DOUBLE			W;
} UVW_VECTOR_TYPE;

typedef struct {
	DOUBLE		heading;
	DOUBLE		lattitude;
	DOUBLE		longitude;
	DOUBLE		altitude;
} GPS_VECTOR_TYPE;

#define FOREVER	for(;;)

#define	TRUE	1
#define FALSE	0

#define	SUCCESS	1
#define	FAILURE	0


#define CONFIG_FNAME_LEN 40
#define CONFIG_DESC_LEN	 40

typedef	struct {
	BOOLEAN			Default;
	BOOLEAN			Prev;
	BOOLEAN			Next;
	BOOLEAN			Load;
	BOOLEAN			Save;
	CHAR			ConfigFile[CONFIG_FNAME_LEN];
	CHAR			ConfigDesc[CONFIG_DESC_LEN];
} FILE_CTRL_TYPE;


typedef ENUM {
	PRIORITY_MAX	= 5,
	PRIORITY_HI	= 4,
	PRIORITY_NORM	= 3,
	PRIORITY_LO	= 2,
	PRIORITY_MIN	= 1
} PRIORITY_TYPE;

#define MAX_FILTER_LEN 255

typedef struct {
	UINT16	Type;
	UINT16	Len;
	DOUBLE	Coeff[MAX_FILTER_LEN];
	DOUBLE	TimeDelay;
	UINT16	DecimationRatio;
} FILTER_DATA;

typedef struct {
	DOUBLE	Volts[8];
	DOUBLE	CPUtemp;
	DOUBLE	MBtemp;
} SCP_MONITOR_DATA;

/* DEFINE A FEW GLOBAL ARRAY INDEXES */
#define _X	0
#define _Y	1
#define _Z	2
#define _RA	3

/* DEFINE SOME COMMONLY USED CONVERSION CONSTANTS			*/
#include <math.h>

#define RAD2DEG		((DOUBLE)180.0 / (DOUBLE)M_PI)
#define DEG2RAD		((DOUBLE)M_PI / (DOUBLE)180.0)
#define SEC2HOUR	((DOUBLE)1 / (DOUBLE)3600.0)
#define HOUR2SEC	((DOUBLE)3600.0)
#define	GRAV_CONST	((DOUBLE)9.81)

/* RADIANS TO 18 BIT ENCODER VALUE CONVERSION				*/
#define RAD2BIN(x) ( (DOUBLE)x * ( (DOUBLE)0x020000 / (DOUBLE)M_PI) )

#endif
