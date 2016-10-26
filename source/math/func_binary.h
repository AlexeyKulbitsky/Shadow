#pragma once
namespace sh
{


	namespace math
	{
		//returns true if the argument is a power of 2.
		// isPow2(2) == true
		// isPow2(3) == false
		// isPow2(16) == true
		bool isPow2(uint32_t a);

		//returns the smallest power of 2 that is greater or equal than the argument
		// getSmallestPow2(127) == 128
		// getSmallestPow2(128) == 128
		// getSmallestPow2(129) == 256
		uint32_t getSmallestPow2(uint32_t val);

		//returns the log2 of the argument
		// getPow2(4) == 2
		// getPow2(5) == 2
		// getPow2(7) == 2
		// getPow2(8) == 3
		int getPow2(uint32_t val);

		//returns the number of 1 bits in the argument
		// getOnBitsCount(4) == 1
		// getOnBitsCount(5) == 2
		// getOnBitsCount(7) == 3
		uint32_t getOnBitsCount(uint32_t x);

	}

}
