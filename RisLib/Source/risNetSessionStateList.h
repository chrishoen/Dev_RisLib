#ifndef _RISNETSESSIONSTATELIST_H_
#define _RISNETSESSIONSTATELIST_H_

/*==============================================================================

session state list class.

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risPortableTypes.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
// Session state list
//
// This class encapsulates a list of state variables associated with a set of 
// sessions.
//
// It can be thought of as a set of ordered pairs from two sets: session
// indices and session identifiers {index,ident}.
//
// This is used by code that organizes collections of sessions by assigning
// each session a session index and by maintaining arrays of state variables
// that can be indexed with the session index. A session index can vary from
// 0..MAX_NUM_SESSIONS-1. A session can also be assigned an identifier, which
// is usually the id of the entity to which it connects.
//
//******************************************************************************

class SessionStateList
{
public:
   SessionStateList();

   //--------------------------------------------------------------
   // Interface:

   // Add an ordered pair to the list
   virtual void add(int aIndex,int aIdent);

   // Remove an ordered pair from the list
   void remove(int aIndex);

   // Get the identifier associated with a session index
   int getIdent(int aIndex);

   // Get the session index associated with an identifier
   int getIndex(int aIdent);

   // Get any valid identifier, used when clients request
   // a zero ident 
   int getAnyIdent();

   // Test for valid session index
   bool isValidIndex(int aIndex);

   // Helpers
   bool isEmpty(){return mNumSessions==0;}
   bool isFull() {return mNumSessions==MaxSessions;}

   //--------------------------------------------------------------
   // Data:

   enum {MaxSessions=20};
   enum {InvalidValue=-1};

   // These arrays are indexed by a session index
   
   // Identifier for a session index
   int  mIdent[MaxSessions];      

   // Peer session index for a session index.
   // It is the session index of the peer to which this session
   // is associated
   int  mPeer[MaxSessions];

   // State code for a session index
   int  mStateCode[MaxSessions];

   // number of sessions in the list
   int  mNumSessions;

   //--------------------------------------------------------------
   // Show:

   void show(char* aLabel);
};

}//namespace
}//namespace

#endif
