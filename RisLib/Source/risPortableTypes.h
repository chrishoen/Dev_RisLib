#ifndef  _RISPORTABLETYPES_H_
#define  _RISPORTABLETYPES_H_
/*==============================================================================
These types are intended to be portable across different
software development environments.
==============================================================================*/

// Win32
namespace Ris
{
#if defined(WIN32)

typedef    signed     __int8     Int08;
typedef  unsigned     __int8     Uint08;
typedef    signed     __int16    Int16;
typedef  unsigned     __int16    Uint16;
typedef    signed     __int32    Int32;
typedef  unsigned     __int32    Uint32;
typedef    signed     __int64    Int64;
typedef  unsigned     __int64    Uint64;
typedef  float                   Real32;
typedef  double                  Real64;

typedef void*    HandleT;
typedef unsigned SocketT;

//******************************************************************************
// VxWorks VxSim

#elif (CPU_FAMILY==SIMNT)

typedef char                     Int08;
typedef unsigned char            Uint08;
typedef short                    Int16;
typedef unsigned short           Uint16;
typedef long                     Int32;
typedef unsigned int             Uint32;
typedef long long                Int64;
typedef unsigned long long       Uint64;
typedef float                    Real32;
typedef double                   Real64;

typedef int      HandleT;
typedef int      SocketT;

//******************************************************************************
// VxWorks GNU Power PC

//#elif (CPU_FAMILY==PPC604)
#else

typedef char                     Int08;
typedef unsigned char            Uint08;
typedef short                    Int16;
typedef unsigned short           Uint16;
typedef long                     Int32;
typedef unsigned int             Uint32;
typedef long long                Int64;
typedef unsigned long long       Uint64;
typedef float                    Real32;
typedef double                   Real64;

typedef int      HandleT;
typedef int      SocketT;

#endif
}

#endif

