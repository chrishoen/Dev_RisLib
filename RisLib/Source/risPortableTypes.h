#ifndef  _RISPORTABLETYPES_H_
#define  _RISPORTABLETYPES_H_
/*==============================================================================
These types are intended to be portable across different
software development environments.
==============================================================================*/

// Win32
namespace Ris
{

typedef char                     Int08;
typedef unsigned char            Uint08;
typedef short                    Int16;
typedef unsigned short           Uint16;
typedef int                      Int32;
typedef unsigned int             Uint32;
typedef long long int            Int64;
typedef unsigned long long       Uint64;
typedef float                    Real32;
typedef double                   Real64;

// microsoft specific
#if defined(WIN32)

typedef void*    HandleT;
typedef unsigned SocketT;

// gcc specific
#else

typedef int      HandleT;
typedef int      SocketT;

#endif
}

#endif

