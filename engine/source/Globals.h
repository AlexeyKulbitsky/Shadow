#ifndef SHADOW_GLOBALS_INCLUDE
#define SHADOW_GLOBALS_INCLUDE

#include "Types.h"
#include "CommonTypes.h"
#include "math/smath.h"
#include "Event.h"
#include "Forward.h"

#include "Singleton.h"

#include "pempek_assert.h"
#include <pugixml.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <vector>
#include <array>
#include <queue>
#include <list>
#include <set>
#include <map>
#include <sstream>
#include <memory>
#include <regex>
#include <functional>

namespace sh
{

	using String = std::string;
	
	template<typename T, typename U>
	using Map = std::map<T, U>;

	template<typename T>
	using Set = std::set<T>;

	template<typename T>
	using List = std::list<T>;

	template<typename T>
	using Queue = std::queue<T>;

	template<typename T>
	using SPtr = std::shared_ptr<T>;

	template<typename T>
	using WPtr = std::weak_ptr<T>;

}


#endif