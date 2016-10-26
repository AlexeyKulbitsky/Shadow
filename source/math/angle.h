#pragma once
namespace sh
{


	namespace math
	{
		namespace angle
		{

			//! returns an angle normalized to 0 - 2PI
			template<typename T>
			T normalize(T angle);

			template<typename T>
			T normalizeInPI(T angle);

		}
	}

}
