#ifndef _RISBYTECONTENT_H_
#define _RISBYTECONTENT_H_
/*==============================================================================

ByteContent and ByteBuffer classes.

ByteBuffer is a smart pointer that addresses a fixed section of memory.

ByteContent is a base class for something that can be copied to/from a
ByteBuffer.

These classes are intended to support the construction of message classes.
The idea here is that each message is encapsulated by a class that inherits
from ByteContent and supplies a member function that allows it to be copied
to/from a ByteBuffer, which can be transported via some communications
medium. All copies are done in network order. 

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risPortableTypes.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a base class for classes that can be copied to/from ByteBuffers.
// Each inheriting class supplies a copyToFrom member function that can be
// called by ByteBuffer put and get functions to copy it.

class ByteBuffer;

class ByteContent
{
public:
   ByteContent();

   // This is supplied by each particular inheriting class to copy itself
   // to/from a byte buffer.
   //
   // The following functions
   //
   //   ByteContent::put  (ByteContent* content);
   //   ByteContent::get  (ByteContent* content);
   //   ByteContent::copy (ByteContent* content);
   //
   // end up calling this member function, which does the actual copy

   virtual void copyToFrom (ByteBuffer* aBuffer) {}

   // This is a resource counter that is used to keep track of deletions
   // when copies of an inheritor are used. The constructor sets the
   // resource counter to one, increase increases it, the macro
   // decreases it and if it is zero then deletes it.

   int mResourceCount;

   void setResource(int aN);
   void increaseResource(int aN=1);
};

//******************************************************************************
// This macro decreases the resource count and deletes when zero.
#define DecreaseResource(aX) \
{ \
   if (--aX->mResourceCount==0) delete aX; \
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This class encapsualtes byte buffers. It represents memory sections. It
// provides member functions to do some simple memory management for a buffer.
// It provides primitive data type put and get operations that allow primitives
// to be copied to/from a byte buffer in network order. It also provides "copy"
// member functions that either put to a buffer or get from a buffer, based on
// a copy direction flag.
//
// This class maintains a buffer base pointer, which contains the base address
// of the buffer, a buffer working pointer, which contains an addresss within
// the buffer where the next put or get operation will be performed. It also
// maintains a buffer length, which contains the number of bytes that have
// been put into the buffer.

class  ByteBuffer
{
public:
   //-------------------------------------------------------------
   // structors.

   // no memory allocated or assigned
   //
   // this constructor          does not do a memory alloc operation.
   // its associated destructor does not do a memory free  operation.
   ByteBuffer ();

   // assigns pre-allocated memory
   //
   // this constructor          does not do a memory alloc operation.
   // its associated destructor does not do a memory free  operation.
   ByteBuffer (char* address,int size);

   // allocates memory of given size  
   // 
   // this constructor          does do a memory alloc operation.
   // its associated destructor does do a memory free  operation.
   // If fill zero is true then all bytes are set to zero.
   ByteBuffer (int size,bool fillZero=false);

   // copy constructor and assignemnt operator.
   // these are set up so that multiple byte buffer objects can
   // point to the same buffer.
   // they copy all members of a source object to a destination
   // object, with the exception of the memory flag, which is
   // cleared to indicate no memory allocation. this is done so
   // that a new byte buffer object can point into the same buffer
   // as an original byte buffer object, but they don't both try to
   // delete the buffer in their destructors.

   ByteBuffer (const ByteBuffer& buffer);
   ByteBuffer& operator= (const ByteBuffer& buffer);

   // deallocates any allocated memory.
  ~ByteBuffer ();
   
   //-------------------------------------------------------------
   // copy the content of a byte buffer.
   // allocates memory of max size of a source buffer
   // and copies contents of source buffer into it.
   void copyByteBuffer (ByteBuffer* source);

   //-------------------------------------------------------------
   // simple memory management.
   // if allocated then destructor frees it

   void  memAlloc (int size);
   void  memFree  ();

   //-------------------------------------------------------------
   // buffer maximum size
   int getMaxLength();

   //-------------------------------------------------------------
   // buffer base address.

   void  setBaseAddress (char* address,int size);
   char* getBaseAddress ();

   //-------------------------------------------------------------
   // buffer length. This is the number of bytes that have
   // actually been put into the buffer, the actual data length.

   void setLength(int value);
   int  getLength();

   //-------------------------------------------------------------
   // error codes.

   void setError(int value);
   int  getError();

   //-------------------------------------------------------------
   // pointer operations.
   // reset sets the working pointer to the base address and
   // sets the length to zero.
   // rewind sets the working pointer to the base address
   // forward advances the working pointer and the length, if the
   // copy direction is for a put operation.
   // advance advances the working pointer and the length

   void  reset();
   void  rewind();
   void  forward     (int byteSize=1);
   void  advance     (int byteSize=1);
   void  fillZero    (int byteSize=1);

   //-------------------------------------------------------------
   // Show

   void  show();

   //-------------------------------------------------------------
   // buffer position.

   // these set the buffer working pointer.
   bool  setPosition (char* address);
   bool  setPosition (int index);

   // these get the buffer working pointer.
   char* getPosition ();
   char* getPositionC();
   void* getPositionV();

   //-------------------------------------------------------------
   // copy direction.
   //
   // this determines the direction of copy operations
   // copy to does a put, copy from does a get.

   void setCopyTo ();
   void setCopyFrom ();

   bool isCopyTo ();
   bool isCopyFrom ();

   //-------------------------------------------------------------
   // This sets the network order of the buffer and whether or not
   // byte swapping happens.
   //
   // If network order is true then bytes are copied into the 
   // buffer in network order, big endian. If false then little
   // endian.
   //
   // If network order is true and the machine is little endian
   // then byte swapping happens. If netwok order is true and
   // the machine is big endian then byte swapping does not 
   // happen. The other two cases follow.
   //
   // The default is non network order.

   void setNetworkOrder (bool aNetworkOrder);

   //-------------------------------------------------------------
   // explicit copy operations for raw byte data.
   // these copy bytes to/from a byte buffer.
   // putData  copies bytes to   a byte buffer.
   // getData  copies bytes from a byte buffer.
   // copyData copies bytes to/from a byte buffer,
   // based on the byte buffer copy direction.

   bool putData  (void* data,int size);
   bool getData  (void* data,int size);
   bool copyData (void* data,int size);

   // This does the same, but at a fixed buffer index
   bool copyData (int index,void* data,int size);

   //-------------------------------------------------------------
   // copy does either a put or a get.
   //
   // These either put an item to the byte buffer or get an
   // item from the byte buffer, based on the value of the
   // buffer copy direction flag.

   bool copy (unsigned char* item);
   bool copy (signed char* item);
   bool copy (unsigned short* item);
   bool copy (unsigned int* item);
   bool copy (unsigned long long* item);
   bool copy (char*  item);
   bool copy (short*  item);
   bool copy (int*  item);
   bool copy (long long*  item);
   bool copy (float* item);
   bool copy (double
* item);
   bool copy (bool* item);
   bool copy (ByteContent* content);

   bool copy24        (unsigned int* item);
   bool copyEnum      (int*    item);
   bool copyZString   (char*   string,int upperBound);
   bool copyFString   (char*   string,int fixedSize);
   bool copyFString2  (char*   string,int fixedSize);
   bool copyZString   (unsigned char* string,int upperBound);
   bool copyFString   (unsigned char* string,int fixedSize);
   bool copyFString2  (unsigned char* string,int fixedSize);
   bool copyBitArray  (bool*   bitArray, int numOfBits,int offset=0);
   bool copyBitValues (unsigned int* bitValues,int numOfBits,int offset=7);
   bool copyBitValues (int*    bitValues,int numOfBits,int offset=7);

   //-------------------------------------------------------------
   // More copies
   //
   // These do the same as the above, but at a fixed buffer index.

   bool copy (int index, unsigned char* item);
   bool copy (int index, signed char* item);
   bool copy (int index, unsigned short* item);
   bool copy (int index, unsigned int* item);
   bool copy (int index, unsigned long long* item);
   bool copy (int index, char*  item);
   bool copy (int index, short*  item);
   bool copy (int index, int*  item);
   bool copy (int index, long long*  item);
   bool copy (int index, float* item);
   bool copy (int index, double
* item);
   bool copy (int index, bool* item);
   bool copy (int index, ByteContent* content);

   bool copy24        (int index, unsigned int* item);
   bool copyEnum      (int index, int*    item);
   bool copyZString   (int index, char*   string,int upperBound);
   bool copyFString   (int index, char*   string,int fixedSize);
   bool copyFString2  (int index, char*   string,int fixedSize);
   bool copyZString   (int index, unsigned char* string,int upperBound);
   bool copyFString   (int index, unsigned char* string,int fixedSize);
   bool copyFString2  (int index, unsigned char* string,int fixedSize);
   bool copyBitArray  (int index, bool*   bitArray, int numOfBits,int offset=0);
   bool copyBitValues (int index, unsigned int* bitValues,int numOfBits,int offset=7);
   bool copyBitValues (int index, int*    bitValues,int numOfBits,int offset=7);

   // helpers
   static void convertBitArrayTo    (bool* bitArray,unsigned short& x);
   static void convertBitArrayFrom  (bool* bitArray,unsigned short& x);
   static void convertBitArrayTo    (bool* bitArray,unsigned int& x);
   static void convertBitArrayFrom  (bool* bitArray,unsigned int& x);

   //ZString is a null terminated string with an upper bound
   //FString is a fixed size string
   //BitArray is a fixed size array of booleans packed into bits

   //-------------------------------------------------------------
   // explicit put operations for byte content.
   // these put byte content to a byte buffer


   bool put           (ByteContent* content);
   bool putToBuffer   (ByteContent* content);

   // explicit get operations for byte content.
   // these get byte content from a byte buffer

   bool get           (ByteContent* content);
   bool getFromBuffer (ByteContent* content);

   //-------------------------------------------------------------
   // put and get operations for primitive data types.
   //
   // put copies an item into the buffer in network order at the
   // current value of the buffer working pointer and advances the
   // pointer. 
   //
   // get copies an item out of the buffer in host order at the
   // current value of the buffer working pointer and advances the
   // pointer. 

   bool put (void* pItem, int pItemSizeInBytes);
   bool get (void* pItem, int pItemSizeInBytes);

   bool putItem (unsigned char  item);
   bool putItem (signed char  item);
   bool putItem (unsigned short  item);
   bool putItem (unsigned int  item);
   bool putItem (unsigned long long  item);

   bool putItem (char   item);
   bool putItem (short   item);
   bool putItem (int   item);
   bool putItem (long long   item);

   bool putItem (float  item);
   bool putItem (double
  item);

   bool getItem (unsigned char* item);
   bool getItem (signed char* item);
   bool getItem (unsigned short* item);
   bool getItem (unsigned int* item);
   bool getItem (unsigned long long* item);

   bool getItem (char*  item);
   bool getItem (short*  item);
   bool getItem (int*  item);
   bool getItem (long long*  item);

   bool getItem (float* item);
   bool getItem (double
* item);

   //-------------------------------------------------------------
   // put and get operations for some other types.

   bool putItem (bool  item);
   bool getItem (bool* item);

   bool put24   (unsigned int  item);
   bool get24   (unsigned int* item);

   bool putEnum (int  item);
   bool getEnum (int* item);

   bool putZString (char* string,int upperBound);
   bool getZString (char* string,int upperBound);

   bool putFString (char* string,int fixedSize);
   bool getFString (char* string,int fixedSize);
   bool getFString2(char* string,int fixedSize);

   bool putBitArray  (bool*   bitArray, int numOfBits,int offset=0);
   bool getBitArray  (bool*   bitArray, int numOfBits,int offset=0);

   bool putBitValues (unsigned int* bitValues,int numOfBits,int offset=7);
   bool getBitValues (unsigned int* bitValues,int numOfBits,int offset=7);

protected:

   //-------------------------------------------------------------
   // pointer members.
   // The base pointer contains the address of the beginning of the
   // buffer. The working pointer contains the address in the buffer
   // to which the next put or get operation will operate. The
   // working length contains the number of bytes that have been put
   // into the buffer. The max length is maxlength of the buffer,
   // it is the size allocated for the buffer

   char* mBasePtr;
   char* mWorkingPtr;
   int   mWorkingLength;
   int   mMaxLength;

   //-------------------------------------------------------------
   // copy direction.
   // this determines the direction of copy operations
   // copy to does a put, copy from does a get.

   enum CopyDirection
   {
      COPY_TO   = 0,
      COPY_FROM = 1
   };

   CopyDirection mCopyDirection;

   //-------------------------------------------------------------
   // Here is a general purpose error code.
   int mError;

   //-------------------------------------------------------------
   // byte swapping.
   //
   // this determines if copy operations do byte swapping.
   // the default for little endian machines is to do
   // byte swapping and the default for big endian machines 
   // is to not do byte swapping. These can be used to
   // override the default.

   bool mByteSwapping;

   //-------------------------------------------------------------
   // Memory allocation code,
   // determines if the destructor does a memFree.

   int mMemAllocCode;
};
//******************************************************************************
//  Here are some error codes.

namespace ByteErrorCode
{
   const int BUFF_OVERFLOW      = -101;
   const int BAD_POINTER        = -102;
}

//******************************************************************************
// This is a class that provides message byte padding.

template <unsigned byteSize> class BytePadding : public ByteContent
{
public:
   void copyToFrom (ByteBuffer* buffer)
   {
      if (buffer->isCopyTo())
      {
         buffer->fillZero(byteSize);
      }
      else
      {
         buffer->forward(byteSize);
      }
   }
};

//******************************************************************************
// This is a byte block class that inherits from ByteContent.

template <unsigned cByteSize> class ByteBlock : public ByteContent
{
public:
   ByteBlock() : mNumOfBytes(0){}

   int  mNumOfBytes;
   unsigned char mBytes[cByteSize];

   void copyToFrom (ByteBuffer* aBuffer)
   {
      aBuffer->copy(&mNumOfBytes);
      aBuffer->copyData((void*)&mBytes[0],mNumOfBytes);
   }

   void put (void* aBytes,int aNumOfBytes)
   {
      if (aNumOfBytes>cByteSize) aNumOfBytes=cByteSize;
      unsigned char* tBytes = (unsigned char*)aBytes;
      for (int i=0;i<aNumOfBytes;i++) mBytes[i]=tBytes[i];
      mNumOfBytes = aNumOfBytes;
   }

   void get (void* aBytes,int* aNumOfBytes=0)
   {
      if (mNumOfBytes>cByteSize) mNumOfBytes=0;
      unsigned char* tBytes = (unsigned char*)aBytes;
      for (int i=0;i<mNumOfBytes;i++) tBytes[i]=mBytes[i];
      if (aNumOfBytes) *aNumOfBytes = mNumOfBytes;
   }
};

//******************************************************************************
// This is a fixed size string class that inherits from ByteContent.

template <unsigned StringSize> class FString : public ByteContent
{
public:
   FString() {mPtr=(char*)&mBytes[0];}

   unsigned char mBytes[StringSize];

   void copyToFrom (ByteBuffer* aBuffer)
   {
      aBuffer->copyData((void*)&mBytes[0],StringSize);
   }

   char* mPtr;
};

//******************************************************************************

void swapBytes4(void* pItem); 

//******************************************************************************
}//namespace
#endif

