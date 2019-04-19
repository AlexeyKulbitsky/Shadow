#ifndef SHADOW_RESOURCE_INCLUDE
#define SHADOW_RESOURCE_INCLUDE

#include "prerequisities/Prerequisities.h"

namespace sh
{
	namespace io
	{
		struct FileInfo;
	}

	class SHADOW_API Resource
	{
	public:
		enum class Group
		{
			Textures = 0,
			Models = 1,
			Techniques = 2,
			Materials = 3,

			COUNT
		};

        const std::string& GetName() const;

	protected:
		std::string m_name;
	};
}

#endif