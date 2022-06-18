#pragma once

/*==============================================================================
This unit provides a set of functions that provide management of a
fixed program base directory to be used by programs that use rislib.
==============================================================================*/
//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Methods.

// Set the program base directory to the current directory.
void setBaseDirectoryToCurrent();

// Set the program base directory to the program bin directory.
void setBaseDirectoryToBin();

// Set the program base directory to the program bin directory.
void setBaseDirectoryToBin();

// Set the program current working directory and the base 
// directory to the program bin directory.
void chdirBaseDirectoryToBin();

// Set the program base directory path global variable.
void setBaseDirectory(char* aDirPath);

// Set the program base directory path global variable.
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
void setBaseDirectoryForWindows(char* aDirPath);
void setBaseDirectoryForLinux(char* aDirPath);

// Return the program base directory path.
char* getBaseDirectory();

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Methods.

// Change the program directory to the base directory path global variable.
void chdirToBaseDirectory();

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Methods.

// Return the full file path to a file that lives somewhere under the base
// directory. The first input is a temp char buffer. The second input is
// a file path that is relative to the base directory path. This returns
// the concatenation of the base pathe and the relative path.
//
// For example, if the base directory path is "/opt/cproc" and the
// given filename is "files/MyParms.txt", then this will return
// "/opt/cproc/files/MyParms.txt".
char* getBaseFullFilePath (char* aBuffer, const char* aRelativeFilePath);


//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace