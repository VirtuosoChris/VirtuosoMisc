#include <iostream>
#include "../Timer.h"

//#ifdef __MSC_VER
#  include <intrin.h>
#  define __builtin_popcount __popcnt
//#endif

/*
//numerator, exponent
inline unsigned int modPow2Min1(unsigned int n, unsigned int s)
{
	const unsigned int d = 0x3;
	unsigned int m;

	for (m = n; n > d; n = m)
	{
		for (m = 0; n; n >>= 2)
		{
			m += n & d;
		}
	}
	// Now m is a value from 0 to d, but since with modulus division
	// we want m to be 0 when it is d.
	m = m == d ? 0 : m;

	return m;
}
*/


/*

//numerator, exponent
inline unsigned int modPow2Min1(unsigned int n)
{

	unsigned int section = n >> 2;
	unsigned int sectionMod = section & 0xf; // mod 4
	unsigned int nMod = n & 0xf;



}*/

inline uint_fast8_t popcnt32(uint32_t a)
{
	//should be implemented with an intrinsic probably, but with sufficient optimizations this will work cross platform (this should compile to a single popcnt instruction
	//return static_cast<uint_fast8_t>(std::bitset<32>(a)::count());
	return __builtin_popcount(a);
}
inline uint_fast8_t popcnt8(uint_fast8_t a)
{
	//return static_cast<uint_fast8_t>(std::bitset<8>(a)::count());
	return __builtin_popcount(a);
}

uint32_t fastmod3(uint32_t a)
{

	/*      b0 * (2^0) mod 3 = 1
	b1 * (2^1) mod 3 = 2
	b2 * (2^2) mod 3 = 1
	b3 * (2^3) mod 3 = 2
	b4 * (2^4) mod 3 = 1
	b5 * (2^5) mod 3 = 2
	b6 * (2^6) mod 3 = 1
	b7 * (2^7) mod 3 = 2*/

	uint_fast8_t m = popcnt32(a) + popcnt32(a & 0xAAAAAAAA); //m is 0-47
	m = popcnt8(m) + popcnt8(m & 0xAA);
	static const uint_fast32_t LUT = 0x24924;//look up table for 2 bits
	return (LUT >> (m << 1)) & 0x3;
}

#define ITERATIONS 0xffffffff
bool validate()
{
	for (int i = 0; i < 10000; i++)
	{
		if ((i % 3) != fastmod3(i))
		{
			return false;
		}
	}
	return true;
}

int main(void)
{

	std::cout << "VALIDATION PASSED : " << validate() << std::endl;

	{
		std::size_t orig1 = 0;

		Timer t;

		for (size_t i = 0; i < ITERATIONS; i++)
		{
			orig1 += i / 197;
		}

		float orig = t.getDelta();
		std::cout << "Modulus op time " << orig << " " << orig1<<  std::endl;
	}

	{
		std::size_t orig1 = 0;

		Timer t;

		for (size_t i = 0; i < ITERATIONS; i++)
		{
			orig1 += fastmod3(i);
		}

		float newV = t.getDelta();
		std::cout << "Fast mod op time " << newV << " "<<orig1<<std::endl;
	}

	system("pause");
}