namespace sh
{
	namespace math
	{

		//////////////////////////////////////////////////////////////////////////

		inline bool isPow2(uint32_t a)
		{
			return !(a & (a - 1)) && a;
		}

		inline uint32_t getSmallestPow2(uint32_t val)
		{
			uint32_t result = 1;
			while (result < val)
			{
				result <<= 1;
			}
			return result;
		}

		inline int getPow2(uint32_t val)
		{
			int shift = -1;
			while (val)
			{
				shift++;
				val >>= 1;
			}
			return shift;
		}

		inline uint32_t getOnBitsCount(uint32_t x)
		{
			// From Hacker's Delight (http://books.google.com/books?id=iBNKMspIlqEC)
			x = x - ((x >> 1) & 0x55555555);
			x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
			x = (x + (x >> 4)) & 0x0F0F0F0F;
			x = x + (x >> 8);
			x = x + (x >> 16);
			return x & 0x0000003F;
		}

	}
}