#ifndef SHADOW_FILE_SYSTEM_INCLUDE
#define SHADOW_FILE_SYSTEM_INCLUDE

#include "../Globals.h"

namespace sh
{
	namespace io
	{
		struct FileInfo
		{
			String name;
			String absolutePath;

			FileInfo(const String& _name, const String& _absolutePath) : name(_name) , absolutePath(_absolutePath) {}
			FileInfo(const FileInfo& other) : name(other.name) , absolutePath(other.absolutePath) {}
			FileInfo() : FileInfo("", "") {}
			FileInfo(const String& name) : FileInfo(name, "") {}

			bool operator<(const FileInfo& other) const
			{
				return name < other.name;
			}

			bool operator==(const FileInfo& other) const 
			{
				return name == other.name;
			}
		};

		class FileSystem : public Singleton<FileSystem>
		{
		public:
			FileSystem();
			virtual ~FileSystem();

			virtual void Init() {}

			virtual const String& GetWorkingDirectory() const { return String(); }

			virtual void AddFolder(const String& folder, bool recursive = true) {}
			virtual const FileInfo& FindFile(const String& fileName) { return FileInfo(); }
			
			virtual bool SaveFile(const std::vector<char>& data, const String& fileName) { return false; }
			virtual bool SaveFile(const String& data, const String& fileName) { return false; }
			virtual std::vector<char> ReadFile(const String& filename) { return std::vector<char>(); }
		};
	}
}

#endif