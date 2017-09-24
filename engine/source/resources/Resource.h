#ifndef SHADOW_RESOURCE_INCLUDE
#define SHADOW_RESOURCE_INCLUDE

#include "../Globals.h"

namespace sh
{
	class Resource
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

		virtual void Load(const String& filePath){}
		virtual void Load(const std::vector<char>& data) { }
		virtual void Load(const std::vector<String>& fileNames){}
		virtual void Unload(){}

		void SetName(const String& name) { m_name = name; }
		void SetFileName(const String& fileName) { m_fileName = fileName; }

		const String& GetName() const { return m_name; }
		const String& GetFileName() const { return m_fileName; }

	protected:
		String m_name;
		String m_fileName;
	};
}

#endif