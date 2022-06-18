/*==============================================================================
Print utility
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risPortableCalls.h"

#include "risBaseDir.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constants.

// Max string size used in subsequent calls.
static const int cMaxBaseDirStringSize = 1000;

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Regional variables.

// Base directory path.
char rBaseDirPath[cMaxBaseDirStringSize] = { 0 };

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Set the program base directory to the current directory.

void setBaseDirectoryToCurrent()
{
   setBaseDirectory(portableGetCurrentDir());
}

// Set the program base directory to the program bin directory.
void setBaseDirectoryToBin()
{
   setBaseDirectory(portableGetBinDir());
}

// Set the program current working directory and the base 
// directory to the program bin directory.
void chdirBaseDirectoryToBin()
{
   Ris::portableChdirToBin();
   setBaseDirectory(portableGetBinDir());
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Set the program base directory path.

void setBaseDirectory(char* aDirPath)
{
   // Store the path to the regional variable.
   strcpy(rBaseDirPath, aDirPath);

   // Get the string length.
   int tLength = (int)strlen(rBaseDirPath);

   // Convert backward slashes to forward slashes.
   for (int i = 0; i < tLength; i++)
   {
      if (rBaseDirPath[i] == '\\')
      {
         rBaseDirPath[i] = '/';
      }
   }

   // If the path doesn't end in a forward slash then add one.
   if (rBaseDirPath[tLength - 1] != '/')
   {
      strcat(rBaseDirPath, "/");
   }
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Set the program base directory path.
//
// These can be used in a cross platofrm program to set the base directory. 
// If the program runs under windows then the first executes and the second
// is ignored. If the program runs under linux the the second is executed
// and the first is ignored.
//
// For example, the two consequetive statments
//
// setBaseDirectoryForWindows("C:/Alpha");
// setBaseDirectoryForLinux("/opt/alpha");
//
// will correctly set the directory, based on where the program is
// running.

void setBaseDirectoryForWindows(char* aDirPath)
{
   // Guard. Exit if linux.
   if (!Ris::portableIsWindows()) return;

   // Store the path to the regional variable.
   strcpy(rBaseDirPath, aDirPath);

   // Get the string length.
   int tLength = (int)strlen(rBaseDirPath);

   // Convert backward slashes to forward slashes.
   for (int i = 0; i < tLength; i++)
   {
      if (rBaseDirPath[i] == '\\')
      {
         rBaseDirPath[i] = '/';
      }
   }

   // If the path doesn't end in a forward slash then add one.
   if (rBaseDirPath[tLength - 1] != '/')
   {
      strcat(rBaseDirPath,"/");
   }
}

void setBaseDirectoryForLinux(char* aDirPath)
{
   // Guard. Exit if windows.
   if (Ris::portableIsWindows()) return;

   // Store the path to the regional variable.
   strcpy(rBaseDirPath, aDirPath);

   // Get the string length.
   int tLength = (int)strlen(rBaseDirPath);

   // If the path doesn't end in a forward slash then add one.
   if (rBaseDirPath[tLength - 1] != '/')
   {
      strcat(rBaseDirPath, "/");
   }
}


//****************************************************************************
//****************************************************************************
//****************************************************************************
// Return the alpha directory path. The input is a temp char buffer.
// The buffer is written to with the requested string to and a pointer
// to it is returned.

char* getBaseDirectory()
{
   return &rBaseDirPath[0];
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Change the program directory to the base directory path global variable.

void chdirToBaseDirectory()
{
   Ris::portableChdir(rBaseDirPath);
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Return the full file path to a file that lives somewhere under the base
// directory. The first input is a temp char buffer. The second input is
// a file path that is relative to the base directory path. This returns
// the concatenation of the base pathe and the relative path.
//
// For example, if the base directory path is "/opt/cproc" and the
// given filename is "files/MyParms.txt", then this will return
// "/opt/cproc/files/MyParms.txt".

char* getBaseFullFilePath(char* aBuffer, const char* aRelativeFilePath)
{
   strcpy(aBuffer, rBaseDirPath);
   strcat(aBuffer, aRelativeFilePath);
   return aBuffer;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
} //namespace

