/**********************************************************************************
*
*  Module:       type.h
*
*  Description:  Defines global variables used throughout the code.
*
*  Company:      Stenograph Corp.
*                596 W. Lamont Rd.
*                Elmhurst, IL  60126
*
*  Notice:       All rights reserved.  This material contains the trade secrets
*                and confidential information of Stenograph Corp, which embody
*                substantial creative effort, ideas and expressions.  No part of
*                this material may be reproduced or transmitted in otherwise,
*                including photocopying and recording in connection with any
*                information storage or retrieval system, without the specific
*                written permission from Stenograph Corp.
*                Copyright 2008, Stenograph Corp.
*
**********************************************************************************/

/**********************************************************************************
*
* Rev. 1.0 Created                                             DBF  10/18/2012
*
**********************************************************************************/

#ifndef _TYPE_H_

#define _TYPE_H_

#define _MAX_UINT32_ 0xFFFFFFFF
#define _MIN_INT32_ 0x80000000

#define NEG_BIT   0x80000000
#define BYTE_NEG_BIT   ((BYTE)(0x80))


typedef void * HANDLE;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef short int16;
typedef unsigned int uint32;
typedef int int32;
typedef          long long int int64;
typedef unsigned long long int uint64;

#ifndef __cplusplus
////typedef unsigned char bool;
#endif

#ifndef BYTE
typedef unsigned char BYTE;
typedef unsigned int UINT;
#endif

typedef unsigned int Steno;

#ifndef __cplusplus
#define true ((bool) 1)
#define false ((bool) 0)
#endif

#define BIT0  (1<<0)
#define BIT1  (1<<1)
#define BIT2  (1<<2)
#define BIT3  (1<<3)
#define BIT4  (1<<4)
#define BIT5  (1<<5)
#define BIT6  (1<<6)
#define BIT7  (1<<7)
#define BIT8  (1<<8)
#define BIT9  (1<<9)
#define BIT10 (1<<10)
#define BIT11 (1<<11)
#define BIT12 (1<<12)
#define BIT13 (1<<13)
#define BIT14 (1<<14)
#define BIT15 (1<<15)
#define BIT16 (1<<16)
#define BIT17 (1<<17)
#define BIT18 (1<<18)
#define BIT19 (1<<19)
#define BIT20 (1<<20)
#define BIT21 (1<<21)
#define BIT22 (1<<22)
#define BIT23 (1<<23)
#define BIT24 (1<<24)
#define BIT25 (1<<25)
#define BIT26 (1<<26)
#define BIT27 (1<<27)
#define BIT28 (1<<28)
#define BIT29 (1<<29)
#define BIT30 (1<<30)
#define BIT31 (1<<31)

typedef struct Sgstn_s
{
  uint32 uiSteno; //sgstn format steno
  uint32 uiTime; //sgstn format timestamp
} Sgstn_t;

#endif //_TYPE_H_
