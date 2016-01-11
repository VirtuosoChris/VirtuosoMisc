/// @file bits.h
/// @brief - utility functions manipulating bits of integer types, including finding MSB, interleaving bits, etc.

#if __cplusplus >= 201103L
#define CONSTEXPR constexpr
#else
#define CONSTEXPR
#endif

#include <cstdlib>

/***** Functions *****/

/// find most significant bit of an integer type
template <typename INTTYPE>
inline int findMSB(INTTYPE n);

/// find most significant bit - compile time constant n
template <typename INTTYPE, INTTYPE n>
inline CONSTEXPR int findMSB();

/// is value of type INTTYPE a power of 2?
template<typename INTTYPE>
inline bool isPow2(INTTYPE val);

/// is power of 2 ? compile time constant
template <typename INTTYPE, INTTYPE val>
inline CONSTEXPR bool isPow2();

/// for some value of type INTTYPE return the next power of 2, or the original value if the value is already a power of 2
template<typename INTTYPE>
static INTTYPE nextPow2(INTTYPE val);

/// for some compile time constant VAL of type INTTYPE return the next power of 2, or the original value if the value is already a power of 2
template<typename INTTYPE, INTTYPE VAL>
static CONSTEXPR INTTYPE nextPow2();

/// INTTYPE is type out, SIGNIFICANTBITS is # lower significant bits in input, SPLIT is bits to separate
/// for example, with an 32 bit INTTYPE, SIGNIFICANTBITS = 7, and SPLIT = 1,
/// and with input value 00000000000000000000000001111111
/// we would have as the result: 00000000000001001001001001001001
/// The bit depth of the output type must be at least SIGNIFICANTBITS * (SPLIT+1)
/// This is useful for morton codes
template <typename INTTYPE, std::size_t SIGNIFICANTBITS, std::size_t SPLIT>
INTTYPE splitBits(INTTYPE val);

/********* Implementations ****************/

#ifdef __GNUC__

template <typename INTTYPE>
CONSTEXPR int countLeadingZeroes(INTTYPE val);

template<>
CONSTEXPR int countLeadingZeroes<unsigned int>(unsigned int in)
{
    return __builtin_clz(in);
}

template<>
CONSTEXPR int countLeadingZeroes<unsigned long>(unsigned long in)
{
    return __builtin_clzl(in);
}

#if __cplusplus >= 201103L
template<>
CONSTEXPR int countLeadingZeroes<long long>(long long in)
{
    return __builtin_clzll(in);
}
#endif

#endif

template <typename INTTYPE>
inline int findMSB(INTTYPE n)
{
#if defined(_MSC_VER)
    
    unsigned long rval;
    _BitScanReverse(&rval, n);
    return rval;
    
#elif __GNUC__
    
    return sizeof(INTTYPE) * 8 - countLeadingZeroes(n);
    
#else
    
#warning "No compiler specific implementation to fin most significant bit found; using fallback path"
    
    INTTYPE rval = 0;
    
    while (n >>= 1)
    {
        rval++;
    }
    
    return rval;
    
#endif
}


template <typename INTTYPE, INTTYPE n>
inline CONSTEXPR int findMSB()
{
#if defined(_MSC_VER)
    
    unsigned long rval;
    _BitScanReverse(&rval, n);
    return rval;
    
#elif __GNUC__
    
    return sizeof(INTTYPE) * 8 - countLeadingZeroes(n);
    
#else
    
#warning "No compiler specific implementation to fin most significant bit found; using fallback path"
    
    INTTYPE rval = 0;
    
    while (n >>= 1)
    {
        rval++;
    }
    
    return rval;
    
#endif
}


template<typename INTTYPE>
inline bool isPow2(INTTYPE val)
{
    return !(val & (val-1));
}


template <typename INTTYPE, INTTYPE val>
inline CONSTEXPR bool isPow2()
{
    return !(val & (val-1));
}


template <typename INTTYPE, int bitsSetInPattern,int numRepeats, int paddingBits, INTTYPE maskIn, int loopCounter>
struct MagicNumber
{
    inline static CONSTEXPR INTTYPE loop()
    {
        const CONSTEXPR INTTYPE mask = maskIn << (bitsSetInPattern + bitsSetInPattern * paddingBits) | (1<<bitsSetInPattern)-1;
        
        return MagicNumber<INTTYPE, bitsSetInPattern, numRepeats, paddingBits, mask, loopCounter+1>::loop();
    }
};

template <typename INTTYPE, int bitsSetInPattern,int numRepeats, int paddingBits, INTTYPE maskIn>
struct MagicNumber<INTTYPE, bitsSetInPattern, numRepeats, paddingBits, maskIn, numRepeats>
{
    inline static CONSTEXPR INTTYPE loop()
    {
        return maskIn;
    }
};


/// gives binary number that are bitsSetInPattern zeroes, followed by bitsSetInPattern ones, repeated numRepeats times.
/// for example, with input args 2, 2 we have 00110011.  These numbers are computed as masks for split bits
template <typename INTTYPE, int bitsSetInPattern,int numRepeats, int paddingBits>
inline CONSTEXPR INTTYPE magicNumber()
{
    return MagicNumber<INTTYPE, bitsSetInPattern, numRepeats, paddingBits, 0,0>::loop();
}


template <typename INTTYPE, std::size_t SEPARATEBITS,  std::size_t REPEATS, std::size_t SHIFT>
struct SplitBits
{
    inline static void loop(INTTYPE& rval)
    {
        const int shiftAdjusted = SEPARATEBITS * SHIFT;
        
        static const CONSTEXPR INTTYPE magicN = magicNumber<INTTYPE, SHIFT, REPEATS, SEPARATEBITS>();
        
        rval = (rval | (rval << shiftAdjusted)) & magicN;
        
        static const std::size_t nextShift = SHIFT>>1;
        static const std::size_t nextRepeats = REPEATS<<1;
        
        SplitBits<INTTYPE, SEPARATEBITS,nextRepeats, nextShift>::loop(rval);
    }
};

template <typename INTTYPE, std::size_t SEPARATEBITS,  std::size_t REPEATS>
struct SplitBits<INTTYPE, SEPARATEBITS, REPEATS, 0>
{
    inline static void loop(INTTYPE& rval)
    {
    }
};

template<typename INTTYPE, INTTYPE B>
static CONSTEXPR INTTYPE nextPow2()
{
    return !isPow2<INTTYPE, B>() ? 1 << (findMSB<INTTYPE, B>()+1) : B;
}

template<typename INTTYPE>
static INTTYPE nextPow2(INTTYPE val)
{
    return !isPow2<INTTYPE>(val) ? 1 << (findMSB<INTTYPE>(val)+1) : val;
}


// A is type out, B is # lower significant bits in input, C is bits to separate
template <typename A, std::size_t B, std::size_t C>
A splitBits(A val)
{
#if __cplusplus >= 201103L
    static_assert((sizeof(A)<<3) >= (B * (C+1)), "Result type does not have sufficient bits to store bitdepth * paddingbits");
#endif
    
    // this algorithm only works correctly on integers with a power of 2 bit depth.  If a non-pot bit depth is passed in, round up to the next pot
    static CONSTEXPR const A bitDepth = nextPow2<std::size_t, B>();
    
    A rval = val & ((1<<B)-1); //mask off lower order bits
    
    const static CONSTEXPR std::size_t SHIFTLOOP = bitDepth>>1;
    SplitBits<A, C, 2, SHIFTLOOP>::loop(rval);
    
    return rval;
}


#include <iostream>

/// test cases
void mortonTest()
{
    std::size_t val = 0xffff;
    
    std::cout<<splitBits<std::size_t, 8, 1>(val)<<std::endl;
    std::cout<<splitBits<std::size_t, 8, 2>(val)<<std::endl;
    std::cout<<splitBits<std::size_t, 8, 3>(val)<<std::endl;
    std::cout<<splitBits<std::size_t, 16, 2>(val)<<std::endl;
    std::cout<<splitBits<std::size_t, 7, 2>(val)<<std::endl;
    
    std::cout<<"\n\nShould be:"<<std::endl;
    std::cout<<0b101010101010101<<std::endl;
    std::cout<<0b1001001001001001001001<<std::endl;
    std::cout<<0b10001000100010001000100010001<<std::endl;
    std::cout<<0b1001001001001001001001001001001001001001001001<<std::endl;
    std::cout<<0b1001001001001001001<<std::endl;
    
}


int main(int argc, char* argv[])
{
    mortonTest();

    return 0;
}