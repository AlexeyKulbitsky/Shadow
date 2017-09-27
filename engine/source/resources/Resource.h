#ifndef SHADOW_RESOURCE_INCLUDE
#define SHADOW_RESOURCE_INCLUDE

#include "../Globals.h"

namespace sh
{
	namespace io
	{
		struct FileInfo;
	}

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

		// Save resource to file using its fileinfo
		virtual void Save() {}

		void SetName(const String& name) { m_name = name; }
		//void SetFileName(const String& fileName) { m_fileName = fileName; }
		void SetFileInfo(std::weak_ptr<io::FileInfo> fileInfo) { m_fileInfo = fileInfo; }

		const String& GetName() const { return m_name; }
		//const String& GetFileName() const { return m_fileName; }
		std::weak_ptr<io::FileInfo> GetFileInfo() const { return m_fileInfo; }
		bool HasChanged() const { return m_changed; }
		void SetChanged(bool changed) { m_changed = changed; }

	protected:
		String m_name;
		//String m_fileName;
		std::weak_ptr<io::FileInfo> m_fileInfo;
		bool m_changed = false;
	};
}

#endif