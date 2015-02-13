#ifndef _REGHELPER_H_
#define _REGHELPER_H_

/*==============================================================================
Description:
Register helper utilities, used to access bits in registers
==============================================================================*/

namespace Reg
{

    //******************************************************************************
    //******************************************************************************
    //******************************************************************************
    // Write a value at a pointer

    inline void writeValue32(volatile unsigned int* aPointer, unsigned int aValue)
    {
        *aPointer = aValue;
    }

    inline void writeValue16(volatile unsigned int* aPointer, unsigned short aValue)
    {
        *aPointer = aValue;
    }

    inline void writeValue08(volatile unsigned int* aPointer, unsigned char aValue)
    {
        *aPointer = aValue;
    }

    //******************************************************************************
    //******************************************************************************
    //******************************************************************************
    // Write a value at an address

    inline void writeValue32(unsigned int aAddress, unsigned int aValue)
    {
        volatile unsigned int* tPointer = (unsigned int*)aAddress;
        *tPointer = aValue;
    }

    inline void writeValue16(unsigned int aAddress, unsigned short aValue)
    {
        volatile unsigned short* tPointer = (unsigned short*)aAddress;
        *tPointer = aValue;
    }

    inline void writeValue08(unsigned int aAddress, unsigned char aValue)
    {
        volatile unsigned char* tPointer = (unsigned char*)aAddress;
        *tPointer = aValue;
    }

    //******************************************************************************
    //******************************************************************************
    //******************************************************************************
    // Read a value at a pointer

    inline unsigned int readValue32(volatile unsigned int* aPointer)
    {
        return *aPointer;
    }

    inline unsigned short readValue16(volatile unsigned short* aPointer)
    {
        return *aPointer;
    }

    inline unsigned char readValue16(volatile unsigned char* aPointer)
    {
        return *aPointer;
    }

    //******************************************************************************
    //******************************************************************************
    //******************************************************************************
    // Read a value at an address

    inline unsigned int readValue32(unsigned int aAddress)
    {
        volatile unsigned int* tPointer = (unsigned int*)aAddress;
        return *tPointer;
    }

    inline unsigned short readValue16(unsigned int aAddress)
    {
        volatile unsigned short* tPointer = (unsigned short*)aAddress;
        return *tPointer;
    }

    inline unsigned char readValue08(unsigned int aAddress)
    {
        volatile unsigned char* tPointer = (unsigned char*)aAddress;
        return *tPointer;
    }

    //******************************************************************************
    //******************************************************************************
    //******************************************************************************
    // Write a bit at a pointer

    inline void writeBit32(volatile unsigned int* aPointer, unsigned int aBitNum, bool aValue)
    {
        unsigned int tMask = (1 << aBitNum);
        if (aValue) *aPointer |= tMask;
        else        *aPointer &= ~tMask;
    }

    inline void writeBit16(volatile unsigned short* aPointer, unsigned int aBitNum, bool aValue)
    {
        unsigned short tMask = (1 << aBitNum);
        if (aValue) *aPointer |= tMask;
        else        *aPointer &= ~tMask;
    }

    inline void writeBit08(volatile unsigned char* aPointer, unsigned int aBitNum, bool aValue)
    {
        unsigned char tMask = (1 << aBitNum);
        if (aValue) *aPointer |= tMask;
        else        *aPointer &= ~tMask;
    }

    //******************************************************************************
    // Write a bit at an address

    inline void writeBit32(unsigned int aAddress, unsigned int aBitNum, bool aValue)
    {
        volatile unsigned int* tPointer = (unsigned int*)aAddress;
        unsigned int tMask = (1 << aBitNum);
        if (aValue) *tPointer |= tMask;
        else        *tPointer &= ~tMask;
    }

    inline void writeBit16(unsigned int aAddress, unsigned int aBitNum, bool aValue)
    {
        volatile unsigned short* tPointer = (unsigned short*)aAddress;
        unsigned short tMask = (1 << aBitNum);
        if (aValue) *tPointer |= tMask;
        else        *tPointer &= ~tMask;
    }

    inline void writeBit08(unsigned int aAddress, unsigned int aBitNum, bool aValue)
    {
        volatile unsigned char* tPointer = (unsigned char*)aAddress;
        unsigned char tMask = (1 << aBitNum);
        if (aValue) *tPointer |= tMask;
        else        *tPointer &= ~tMask;
    }

    //******************************************************************************
    //******************************************************************************
    //******************************************************************************
    // Read a bit at a pointer

    inline bool readBit32(volatile unsigned int* aPointer, unsigned int aBitNum)
    {
        unsigned int tMask = (1 << aBitNum);
        return (*aPointer & tMask)!=0;
    }

    inline bool readBit16(volatile unsigned short* aPointer, unsigned int aBitNum)
    {
        unsigned short tMask = (1 << aBitNum);
        return (*aPointer & tMask)!=0;
    }

    inline bool readBit08(volatile unsigned char* aPointer, unsigned int aBitNum)
    {
        unsigned char tMask = (1 << aBitNum);
        return (*aPointer & tMask)!=0;
    }

    //******************************************************************************
    //******************************************************************************
    //******************************************************************************
    // Read a bit at an address

    inline bool readBit32(unsigned int aAddress, unsigned int aBitNum)
    {
        volatile unsigned int* tPointer = (unsigned int*)aAddress;
        unsigned int tMask = (1 << aBitNum);
        return (*tPointer & tMask)!=0;
    }

    inline bool readBit16(unsigned int aAddress, unsigned int aBitNum)
    {
        volatile unsigned short* tPointer = (unsigned short*)aAddress;
        unsigned short tMask = (1 << aBitNum);
        return (*tPointer & tMask)!=0;
    }

    inline bool readBit08(unsigned int aAddress, unsigned int aBitNum)
    {
        volatile unsigned char* tPointer = (unsigned char*)aAddress;
        unsigned char tMask = (1 << aBitNum);
        return (*tPointer & tMask)!=0;
    }

    //******************************************************************************
    //******************************************************************************
    //******************************************************************************
    // Write bits at a pointer

    void writeBits32(unsigned int* aPointer, unsigned int aBitNum, unsigned int aNumBits, unsigned int aValue)
    {
        unsigned int tMask = 0;
        for (unsigned int i = 0; i<aNumBits; i++) tMask |= (1 << (aBitNum + i));
        unsigned tValue = (aValue << aBitNum);
        tValue &= tMask;
        *aPointer &= ~tMask;
        *aPointer |= tValue;
    }

    void writeBits16(unsigned short* aPointer, unsigned int aBitNum, unsigned int aNumBits, unsigned short aValue)
    {
        unsigned short tMask = 0;
        for (unsigned int i = 0; i<aNumBits; i++) tMask |= (1 << (aBitNum + i));
        unsigned tValue = (aValue << aBitNum);
        tValue &= tMask;
        *aPointer &= ~tMask;
        *aPointer |= tValue;
    }

    void writeBits08(unsigned char* aPointer, unsigned int aBitNum, unsigned int aNumBits, unsigned char aValue)
    {
        unsigned char tMask = 0;
        for (unsigned int i = 0; i<aNumBits; i++) tMask |= (1 << (aBitNum + i));
        unsigned tValue = (aValue << aBitNum);
        tValue &= tMask;
        *aPointer &= ~tMask;
        *aPointer |= tValue;
    }

    //******************************************************************************
    //******************************************************************************
    //******************************************************************************
    // Write bits at an address

    void writeBits32(unsigned int aAddress, unsigned int aBitNum, unsigned int aNumBits, unsigned int aValue)
    {
        volatile unsigned int* tPointer = (unsigned int*)aAddress;
        unsigned int tMask = 0;
        for (unsigned int i = 0; i<aNumBits; i++) tMask |= (1 << (aBitNum + i));
        unsigned tValue = (aValue << aBitNum);
        tValue &= tMask;
        *tPointer &= ~tMask;
        *tPointer |= tValue;
    }

    void writeBits16(unsigned int aAddress, unsigned int aBitNum, unsigned int aNumBits, unsigned short aValue)
    {
        volatile unsigned short* tPointer = (unsigned short*)aAddress;
        unsigned short tMask = 0;
        for (unsigned int i = 0; i<aNumBits; i++) tMask |= (1 << (aBitNum + i));
        unsigned tValue = (aValue << aBitNum);
        tValue &= tMask;
        *tPointer &= ~tMask;
        *tPointer |= tValue;
    }

    void writeBits08(unsigned int aAddress, unsigned int aBitNum, unsigned int aNumBits, unsigned char aValue)
    {
        volatile unsigned char* tPointer = (unsigned char*)aAddress;
        unsigned char tMask = 0;
        for (unsigned int i = 0; i<aNumBits; i++) tMask |= (1 << (aBitNum + i));
        unsigned tValue = (aValue << aBitNum);
        tValue &= tMask;
        *tPointer &= ~tMask;
        *tPointer |= tValue;
    }

    //******************************************************************************
    //******************************************************************************
    //******************************************************************************
    // Read bits at a pointer

    unsigned int readBits32(unsigned int* aPointer, unsigned int aBitNum, unsigned int aNumBits)
    {
        unsigned int tValue = ((*aPointer) >> aBitNum);
        unsigned int tMask = 0;
        for (unsigned int i = 0; i<aNumBits; i++) tMask |= (1 << i);
        return tValue & tMask;
    }

    unsigned short readBits16(unsigned short* aPointer, unsigned int aBitNum, unsigned int aNumBits)
    {
        unsigned short tValue = ((*aPointer) >> aBitNum);
        unsigned short tMask = 0;
        for (unsigned int i = 0; i<aNumBits; i++) tMask |= (1 << i);
        return tValue & tMask;
    }

    unsigned int readBits08(unsigned char* aPointer, unsigned int aBitNum, unsigned int aNumBits)
    {
        unsigned char tValue = ((*aPointer) >> aBitNum);
        unsigned char tMask = 0;
        for (unsigned int i = 0; i<aNumBits; i++) tMask |= (1 << i);
        return tValue & tMask;
    }

    //******************************************************************************
    //******************************************************************************
    //******************************************************************************
    // Read bits at an address

    unsigned int readBits32(unsigned int aAddress, unsigned int aBitNum, unsigned int aNumBits)
    {
        volatile unsigned int* tPointer = (unsigned int*)aAddress;
        unsigned int tValue = ((*tPointer) >> aBitNum);
        unsigned int tMask = 0;
        for (unsigned int i = 0; i<aNumBits; i++) tMask |= (1 << i);
        return tValue & tMask;
    }

    unsigned short readBits16(unsigned int aAddress, unsigned int aBitNum, unsigned int aNumBits)
    {
        volatile unsigned short* tPointer = (unsigned short*)aAddress;
        unsigned short tValue = ((*tPointer) >> aBitNum);
        unsigned short tMask = 0;
        for (unsigned int i = 0; i<aNumBits; i++) tMask |= (1 << i);
        return tValue & tMask;
    }

    unsigned int readBits08(unsigned int aAddress, unsigned int aBitNum, unsigned int aNumBits)
    {
        volatile unsigned char* tPointer = (unsigned char*)aAddress;
        unsigned char tValue = ((*tPointer) >> aBitNum);
        unsigned char tMask = 0;
        for (unsigned int i = 0; i<aNumBits; i++) tMask |= (1 << i);
        return tValue & tMask;
    }

}//namespace

#endif

