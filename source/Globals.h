#ifndef SHADOW_GLOBALS_INCLUDE
#define SHADOW_GLOBALS_INCLUDE

#include <vector>
#include <set>
#include <map>

#include "Types.h"
#include "math\smath.h"
#include "Event.h"

namespace sh
{

	typedef std::string		String;
	
	template<typename T, typename U>
	using HashMap = std::map<T, U>;

	template<typename T>
	using HashSet = std::set<T>;
}


#endif