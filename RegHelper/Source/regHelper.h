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
    // Write a bit at a pointer at a mask

    inline void writeBitAtMask32(volatile unsigned int* aPointer, unsigned int aBitMask, unsigned int aValue)
    {
        if (aValue) *aPointer |= aBitMask;
        else        *aPointer &= ~aBitMask;
    }

    inline void writeBitAtMask16(volatile unsigned short* aPointer, unsigned short aBitMask, unsigned int aValue)
    {
        if (aValue) *aPointer |= aBitMask;
        else        *aPointer &= ~aBitMask;
    }

    inline void writeBitAtMask08(volatile unsigned char* aPointer, unsigned char aBitMask, unsigned int aValue)
    {
        if (aValue) *aPointer |= aBitMask;
        else        *aPointer &= ~aBitMask;
    }

    //******************************************************************************
    //******************************************************************************
    //******************************************************************************
    // Write a bit at an address at a mask

    inline void writeBitAtMask32(unsigned int aAddress, unsigned int aBitMask, unsigned int aValue)
    {
        volatile unsigned int* tPointer = (unsigned int*)aAddress;
        if (aValue) *tPointer |= aBitMask;
        else        *tPointer &= ~aBitMask;
    }

    inline void writeBitAtMask16(unsigned int aAddress, unsigned short aBitMask, unsigned int aValue)
    {
        volatile unsigned short* tPointer = (unsigned short*)aAddress;
        if (aValue) *tPointer |= aBitMask;
        else        *tPointer &= ~aBitMask;
    }

    inline void writeBitAtMask08(unsigned int aAddress, unsigned char aBitMask, unsigned int aValue)
    {
        volatile unsigned char* tPointer = (unsigned char*)aAddress;
        if (aValue) *tPointer |= aBitMask;
        else        *tPointer &= ~aBitMask;
    }

    //******************************************************************************
    //******************************************************************************
    //******************************************************************************
    // Read a bit at a pointer at a mask

    inline unsigned int readBitAtMask32(volatile unsigned int* aPointer, unsigned int aBitMask)
    {
        return *aPointer & aBitMask ? 1 : 0;
    }

    inline unsigned int readBitAtMask16(volatile unsigned short* aPointer, unsigned short aBitMask)
    {
        return *aPointer & aBitMask ? 1 : 0;
    }

    inline unsigned int readBitAtMask08(volatile unsigned char* aPointer, unsigned char aBitMask)
    {
        return *aPointer & aBitMask ? 1 : 0;
    }

    //******************************************************************************
    //******************************************************************************
    //******************************************************************************
    // Read a bit at an address at a mask

    inline unsigned int readBitAtMask32(unsigned int aAddress, unsigned int aBitMask)
    {
        volatile unsigned int* tPointer = (unsigned int*)aAddress;
        return *tPointer & aBitMask ? 1 : 0;
    }

    inline unsigned int readBitAtMask16(unsigned int aAddress, unsigned short aBitMask)
    {
        volatile unsigned short* tPointer = (unsigned short*)aAddress;
        return *tPointer & aBitMask ? 1 : 0;
    }

    inline unsigned int readBitAtMask08(unsigned int aAddress, unsigned char aBitMask)
    {
        volatile unsigned char* tPointer = (unsigned char*)aAddress;
        return *tPointer & aBitMask ? 1 : 0;
    }

    //******************************************************************************
    //******************************************************************************
    //******************************************************************************
    // Write a bit at a pointer at a bit position

    inline void writeBitAtPos32(volatile unsigned int* aPointer, unsigned int aBitPos, unsigned int aValue)
    {
        unsigned int tValue = (1 << aBitPos);
        if (aValue) *aPointer |= tValue;
        else        *aPointer &= ~tValue;
    }

    inline void writeBitAtPos16(volatile unsigned short* aPointer, unsigned int aBitPos, unsigned int aValue)
    {
        unsigned short tValue = (1 << aBitPos);
        if (aValue) *aPointer |= tValue;
        else        *aPointer &= ~tValue;
    }

    inline void writeBitAtPos08(volatile unsigned char* aPointer, unsigned int aBitPos, unsigned int aValue)
    {
        unsigned char tValue = (1 << aBitPos);
        if (aValue) *aPointer |= tValue;
        else        *aPointer &= ~tValue;
    }

    //******************************************************************************
    // Write a bit at an address at a bit position

    inline void writeBitAtPos32(unsigned int aAddress, unsigned int aBitPos, unsigned int aValue)
    {
        volatile unsigned int* tPointer = (unsigned int*)aAddress;
        unsigned int tValue = (1 << aBitPos);
        if (aValue) *tPointer |= tValue;
        else        *tPointer &= ~tValue;
    }

    inline void writeBitAtPos16(unsigned int aAddress, unsigned int aBitPos, unsigned int aValue)
    {
        volatile unsigned short* tPointer = (unsigned short*)aAddress;
        unsigned short tValue = (1 << aBitPos);
        if (aValue) *tPointer |= tValue;
        else        *tPointer &= ~tValue;
    }

    inline void writeBitAtPos08(unsigned int aAddress, unsigned int aBitPos, unsigned int aValue)
    {
        volatile unsigned char* tPointer = (unsigned char*)aAddress;
        unsigned char tValue = (1 << aBitPos);
        if (aValue) *tPointer |= tValue;
        else        *tPointer &= ~tValue;
    }

    //******************************************************************************
    //******************************************************************************
    //******************************************************************************
    // Read a bit at a pointer at a bit position

    inline unsigned int readBitAtPos32(volatile unsigned int* aPointer, unsigned int aBitPos)
    {
        unsigned int tValue = (1 << aBitPos);
        return *aPointer & tValue ? 1 : 0;
    }

    inline unsigned int readBitAtPos16(volatile unsigned short* aPointer, unsigned int aBitPos)
    {
        unsigned short tValue = (1 << aBitPos);
        return *aPointer & tValue ? 1 : 0;
    }

    inline unsigned int readBitAtPos08(volatile unsigned char* aPointer, unsigned int aBitPos)
    {
        unsigned char tValue = (1 << aBitPos);
        return *aPointer & tValue ? 1 : 0;
    }

    //******************************************************************************
    //******************************************************************************
    //******************************************************************************
    // Read a bit at an address at a bit position

    inline unsigned int readBitAtPos32(unsigned int aAddress, unsigned int aBitPos)
    {
        volatile unsigned int* tPointer = (unsigned int*)aAddress;
        unsigned int tValue = (1 << aBitPos);
        return *tPointer & tValue ? 1 : 0;
    }

    inline unsigned int readBitAtPos16(unsigned int aAddress, unsigned int aBitPos)
    {
        volatile unsigned short* tPointer = (unsigned short*)aAddress;
        unsigned short tValue = (1 << aBitPos);
        return *tPointer & tValue ? 1 : 0;
    }

    inline unsigned int readBitAtPos08(unsigned int aAddress, unsigned int aBitPos)
    {
        volatile unsigned char* tPointer = (unsigned char*)aAddress;
        unsigned char tValue = (1 << aBitPos);
        return *tPointer & tValue ? 1 : 0;
    }

    //******************************************************************************
    //******************************************************************************
    //******************************************************************************
    // Write bits at a pointer at a bit position

    void writeBitsAtPos32(unsigned int* aPointer, unsigned int aBitPos, unsigned int aNumBits, unsigned int aValue)
    {
        unsigned int tMask = 0;
        for (unsigned i = 0; i<aNumBits; i++) tMask |= (1 << (aBitPos + i));
        unsigned tValue = (aValue << aBitPos);
        tValue &= tMask;
        *aPointer &= ~tMask;
        *aPointer |= tValue;
    }

    void writeBitsAtPos16(unsigned short* aPointer, unsigned int aBitPos, unsigned int aNumBits, unsigned short aValue)
    {
        unsigned short tMask = 0;
        for (unsigned i = 0; i<aNumBits; i++) tMask |= (1 << (aBitPos + i));
        unsigned tValue = (aValue << aBitPos);
        tValue &= tMask;
        *aPointer &= ~tMask;
        *aPointer |= tValue;
    }

    void writeBitsAtPos08(unsigned char* aPointer, unsigned int aBitPos, unsigned int aNumBits, unsigned char aValue)
    {
        unsigned char tMask = 0;
        for (unsigned i = 0; i<aNumBits; i++) tMask |= (1 << (aBitPos + i));
        unsigned tValue = (aValue << aBitPos);
        tValue &= tMask;
        *aPointer &= ~tMask;
        *aPointer |= tValue;
    }

    //******************************************************************************
    //******************************************************************************
    //******************************************************************************
    // Write bits at an address at a bit position

    void writeBitsAtPos32(unsigned aAddress, unsigned int aBitPos, unsigned int aNumBits, unsigned int aValue)
    {
        volatile unsigned int* tPointer = (unsigned int*)aAddress;
        unsigned int tMask = 0;
        for (unsigned i = 0; i<aNumBits; i++) tMask |= (1 << (aBitPos + i));
        unsigned tValue = (aValue << aBitPos);
        tValue &= tMask;
        *tPointer &= ~tMask;
        *tPointer |= tValue;
    }

    void writeBitsAtPos16(unsigned aAddress, unsigned int aBitPos, unsigned int aNumBits, unsigned short aValue)
    {
        volatile unsigned short* tPointer = (unsigned short*)aAddress;
        unsigned short tMask = 0;
        for (unsigned i = 0; i<aNumBits; i++) tMask |= (1 << (aBitPos + i));
        unsigned tValue = (aValue << aBitPos);
        tValue &= tMask;
        *tPointer &= ~tMask;
        *tPointer |= tValue;
    }

    void writeBitsAtPos08(unsigned aAddress, unsigned int aBitPos, unsigned int aNumBits, unsigned char aValue)
    {
        volatile unsigned char* tPointer = (unsigned char*)aAddress;
        unsigned char tMask = 0;
        for (unsigned i = 0; i<aNumBits; i++) tMask |= (1 << (aBitPos + i));
        unsigned tValue = (aValue << aBitPos);
        tValue &= tMask;
        *tPointer &= ~tMask;
        *tPointer |= tValue;
    }

    //******************************************************************************
    //******************************************************************************
    //******************************************************************************
    // Read bits at a pointer at a bit position

    unsigned int readBitsAtPos32(unsigned int* aPointer, unsigned int aBitPos, unsigned int aNumBits)
    {
        unsigned int tValue = ((*aPointer) >> aBitPos);
        unsigned int tMask = 0;
        for (unsigned i = 0; i<aNumBits; i++) tMask |= (1 << i);
        return tValue & tMask;
    }

    unsigned short readBitsAtPos16(unsigned short* aPointer, unsigned int aBitPos, unsigned int aNumBits)
    {
        unsigned short tValue = ((*aPointer) >> aBitPos);
        unsigned short tMask = 0;
        for (unsigned i = 0; i<aNumBits; i++) tMask |= (1 << i);
        return tValue & tMask;
    }

    unsigned int readBitsAtPos08(unsigned char* aPointer, unsigned int aBitPos, unsigned int aNumBits)
    {
        unsigned char tValue = ((*aPointer) >> aBitPos);
        unsigned char tMask = 0;
        for (unsigned i = 0; i<aNumBits; i++) tMask |= (1 << i);
        return tValue & tMask;
    }

    //******************************************************************************
    //******************************************************************************
    //******************************************************************************
    // Read bits at an address at a bit position

    unsigned int readBitsAtPos32(unsigned int aAddress, unsigned int aBitPos, unsigned int aNumBits)
    {
        volatile unsigned int* tPointer = (unsigned int*)aAddress;
        unsigned int tValue = ((*tPointer) >> aBitPos);
        unsigned int tMask = 0;
        for (unsigned i = 0; i<aNumBits; i++) tMask |= (1 << i);
        return tValue & tMask;
    }

    unsigned short readBitsAtPos16(unsigned int aAddress, unsigned int aBitPos, unsigned int aNumBits)
    {
        volatile unsigned short* tPointer = (unsigned short*)aAddress;
        unsigned short tValue = ((*tPointer) >> aBitPos);
        unsigned short tMask = 0;
        for (unsigned i = 0; i<aNumBits; i++) tMask |= (1 << i);
        return tValue & tMask;
    }

    unsigned int readBitsAtPos08(unsigned int aAddress, unsigned int aBitPos, unsigned int aNumBits)
    {
        volatile unsigned char* tPointer = (unsigned char*)aAddress;
        unsigned char tValue = ((*tPointer) >> aBitPos);
        unsigned char tMask = 0;
        for (unsigned i = 0; i<aNumBits; i++) tMask |= (1 << i);
        return tValue & tMask;
    }

}//namespace

#endif

