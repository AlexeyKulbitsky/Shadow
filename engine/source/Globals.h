#ifndef SHADOW_GLOBALS_INCLUDE
#define SHADOW_GLOBALS_INCLUDE

#include "Types.h"
#include "CommonTypes.h"
#include "math\smath.h"
#include "Event.h"
#include "Forward.h"

#include "Singleton.h"

#include "pempek_assert.h"
#include <pugixml.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H 

#include <vector>
#include <array>
#include <set>
#include <map>
#include <sstream>
#include <memory>
#include <regex>
#include <functional>

namespace sh
{

	typedef std::string		String;
	
	template<typename T, typename U>
	using Map = std::map<T, U>;

	template<typename T>
	using Set = std::set<T>;

	template<typename T>
	using SPtr = std::shared_ptr<T>;

}


#endif