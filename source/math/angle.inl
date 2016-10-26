namespace sh
{
	namespace math
	{
		namespace angle
		{


			//! returns an angle normalized to 0 .. 2PI
			template<typename T>
			inline T normalize(const T angle)
			{
				T a = angle;
				while (a >= k_2pi)
				{
					a -= k_2pi;
				}

				while (a < (T)0)
				{
					a += k_2pi;
				}

				return a;
			}

			//! returns an angle normalized to -PI .. +PI
			template<typename T>
			inline T normalizeInPI(const T angle)
			{
				T a = angle;
				while (a >= k_pi)
				{
					a -= k_2pi;
				}

				while (a < -k_pi)
				{
					a += k_2pi;
				}

				return a;
			}

		}
	}
}