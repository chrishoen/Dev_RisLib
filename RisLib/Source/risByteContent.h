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
   // This is supplied by each particular inheriting class to copy itself
   // to/from a byte buffer.
   //

   virtual void copyToFrom (ByteBuffer* aBuffer) {}
};

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
   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   // Constructors.

   // Default constructor        does not do a memory alloc operation.
   ByteBuffer ();

   // Allocates heap memory of given size  
   // This constructor           does do a memory alloc operation.
   // It's associated destructor does do a memory free  operation.
   ByteBuffer (int aSize);

   // Assigns pre-allocated memory
   // This constructor           does not do a memory alloc operation.
   // It's associated destructor does not do a memory free  operation.
   ByteBuffer (char* aAddress,int aSize);

   // Copy constructor and assignemnt operator.
   // These are set up so that multiple byte buffer objects can
   // point to the same buffer.
   // They copy all members of a source object to a destination
   // object, with the exception of the memory flag, which is
   // cleared to indicate no memory allocation. This is done so
   // that a new byte buffer object can point into the same buffer
   // as an original byte buffer object, but they don't both try to
   // delete the buffer in their destructors.

   ByteBuffer (const ByteBuffer& buffer);
   ByteBuffer& operator= (const ByteBuffer& buffer);

   // Deallocates any allocated memory.
  ~ByteBuffer ();
   
   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   // Buffer management.

   //---------------------------------------------------------------------------
   // Heap memory, if allocated then destructor frees it

   void  memAlloc (int aSize);
   void  memFree  ();

   //---------------------------------------------------------------------------
   // Buffer maximum size

   int getMaxLength();

   //---------------------------------------------------------------------------
   // Buffer base address.

   void  setBaseAddress (char* aAddress,int aSize);
   char* getBaseAddress ();

   //---------------------------------------------------------------------------
   // Buffer length. This is the number of bytes that have
   // actually been put into the buffer, the actual data length.

   void setLength(int aValue);
   int  getLength();

   //---------------------------------------------------------------------------
   // Error codes.

   void setError(int aValue);
   int  getError();

   //---------------------------------------------------------------------------
   // Pointer operations.
   // reset sets the working pointer to the base address and
   // sets the length to zero.
   // rewind sets the working pointer to the base address
   // forward advances the working pointer and the length, if the
   // copy direction is for a put operation.
   // advance advances the working pointer and the length

   void  reset();
   void  rewind();
   void  forward     (int aByteSize=1);
   void  advance     (int aByteSize=1);
   void  fillZero    (int aByteSize=1);

   //---------------------------------------------------------------------------
   // Buffer position.

   // These set the buffer working pointer.
   bool  setPosition (char* aAddress);
   bool  setPosition (int   aIndex);

   // These get the buffer working pointer.
   char* getPosition ();
   char* getPositionC();
   void* getPositionV();

   //---------------------------------------------------------------------------
   // Copy direction.
   //
   // this determines the direction of copy operations
   // copy to does a put, copy from does a get.

   void setCopyTo ();
   void setCopyFrom ();

   bool isCopyTo ();
   bool isCopyFrom ();

   //---------------------------------------------------------------------------
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

   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   // Copy operations

   //---------------------------------------------------------------------------
   // copy does either a copy to the buffer or a copy from the buffer
   //
   // These either put an item to the byte buffer or get an
   // item from the byte buffer, based on the value of the
   // buffer copy direction flag.

   bool copy (unsigned char*      aValue);
   bool copy (unsigned short*     aValue);
   bool copy (unsigned int*       aValue);
   bool copy (unsigned long long* aValue);
   bool copy (char*               aValue);
   bool copy (short*              aValue);
   bool copy (int*                aValue);
   bool copy (long long*          aValue);
   bool copy (float*              aValue);
   bool copy (double*             aValue);
   bool copy (bool*               aValue);

   bool copyEnum      (int*           aValue);
   bool copyZString   (unsigned char* aString,int aUpperBound);
   bool copyFString   (unsigned char* aString,int aSize);
   bool copyZString   (char*          aString,int aUpperBound);
   bool copyFString   (char*          aString,int aSize);

   //ZString is a null terminated string with an upper bound
   //FString is a fixed size string

   //---------------------------------------------------------------------------
   // Copy an object that inherits from ByteContent to/from a byte buffer.

   bool copy          (ByteContent* content);
   bool putToBuffer   (ByteContent* content);
   bool getFromBuffer (ByteContent* content);

   //---------------------------------------------------------------------------
   // Copy raw byte data.

   bool copyData (void* aData,int aSize);

   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   // Members

   //---------------------------------------------------------------------------
   // Pointer members.
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

   //---------------------------------------------------------------------------
   // Copy direction. This specifies the direction of copy operations.
   // Copy to does a put, copy from does a get.

   enum CopyDirection
   {
      COPY_TO   = 0,
      COPY_FROM = 1
   };

   CopyDirection mCopyDirection;

   //---------------------------------------------------------------------------
   // General purpose error code.

   int mError;

   //---------------------------------------------------------------------------
   // Byte swapping. This determines if copy operations do byte swapping. The 
   // default for little endian machines is to not do byte swapping and the
   // default for big endian machines is to do byte swapping.

   bool mByteSwapping;

   //---------------------------------------------------------------------------
   // Memory allocation code, itdetermines if the destructor does a memFree.

   int mMemAllocCode;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
//  Error codes.

namespace ByteErrorCode
{
   const int BUFF_OVERFLOW      = -101;
   const int BAD_POINTER        = -102;
}

//******************************************************************************
//******************************************************************************
// This is a class that provides message byte padding.

template <unsigned cByteSize> class BytePadding : public ByteContent
{
public:
   void copyToFrom (ByteBuffer* buffer)
   {
      if (buffer->isCopyTo())
      {
         buffer->fillZero(cByteSize);
      }
      else
      {
         buffer->forward(cByteSize);
      }
   }
};

//******************************************************************************
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
//******************************************************************************
// This is a fixed size string class that inherits from ByteContent.

template <unsigned cStringSize> class FString : public ByteContent
{
public:
   FString() {mPtr=(char*)&mBytes[0];}

   void copyToFrom (ByteBuffer* aBuffer)
   {
      aBuffer->copyData((void*)&mBytes[0],cStringSize);
      mBytes[cStringSize] = 0;
   }

   unsigned char mBytes[cStringSize+1];
   char* mPtr;
};

//******************************************************************************
}//namespace
#endif

