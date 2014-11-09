#ifndef _RISCRITICALSECTION_H_
#define _RISCRITICALSECTION_H_
/*==============================================================================

This file contains functions that imlement a critical section.

==============================================================================*/
#include "risPortableTypes.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

void  enterCriticalSection();
void  leaveCriticalSection();

}//namespace
#endif
