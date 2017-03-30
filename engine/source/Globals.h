#ifndef SHADOW_GLOBALS_INCLUDE
#define SHADOW_GLOBALS_INCLUDE

#include "Types.h"
#include "CommonTypes.h"
#include "math\smath.h"
#include "Event.h"
#include "Forward.h"

#include "pempek_assert.h"
#include <pugixml.hpp>

#include <vector>
#include <array>
#include <set>
#include <map>
#include <sstream>
#include <functional>

namespace sh
{

	typedef std::string		String;
	
	template<typename T, typename U>
	using Map = std::map<T, U>;

	template<typename T>
	using Set = std::set<T>;

}


#endif