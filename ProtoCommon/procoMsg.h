#ifndef _PROCOMSG_H_
#define _PROCOMSG_H_
/*==============================================================================
This file contains a set of classes that encapsulate the message set
that is used to communicate with Intranet. The messages are specified
in the IDD.

There is a class for each particular message in the set and there is a
base class that all of the messages inherit from.

These messages follow the ByteContent pattern, where they all inherit
from ByteContent so that they can be copied to/from ByteBuffers.

The base class is used to specify set membership, any inheriting class
is a member of the message set, and message objects can be referenced
anonymously via pointers to the the base class.

The base class provides a member function, makeFromByteBuffer, that
extracts particular messages from a byte buffer and returns a pointer
to the base class.

These messages all have the same common form, they all contain a
common message header. The base class has a Header member object that
encapsulates the header.

These are the messages:

Test
FirstMessage
StatusRequest
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
// The message set is encapsulated by classes that are defined in the following
// files.

// Contains header, parser, message base classes
#include "procoMsg_Base.h"    

// Contains particular message classes
#include "procoMsg_Message.h" 
#endif

