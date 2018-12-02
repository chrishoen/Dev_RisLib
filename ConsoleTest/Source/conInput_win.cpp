/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <conio.h>
#include <ctype.h>

#include "conInputReader.h"
#include "conInputReader_win.h"

namespace Con
{

void initializeInput()
{
   gInputReader.initialize();
}

void finalizeInput()
{
   gInputReader.finalize();
}

void doReadInputString(char* aInputString)
{
   gInputReader.doReadString(aInputString);
}

void doWaitForPressEnter()
{
   gInputReader.doWaitForPressEnter();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

