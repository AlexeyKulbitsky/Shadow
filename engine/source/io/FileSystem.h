#ifndef SHADOW_FILE_SYSTEM_INCLUDE
#define SHADOW_FILE_SYSTEM_INCLUDE

#include "../Globals.h"
#include "File.h"

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

			virtual const String& GetWorkingDirectory() const 
			{ 
				static String res;
				return res;
			}

			virtual void AddFolder(const String& folder, bool recursive = true) {}
			virtual const FileInfo& FindFile(const String& fileName) 
			{ 
				static FileInfo res;
				return res;
			}
			
			virtual bool SaveFile(const std::vector<char>& data, const String& fileName) { return false; }
			virtual bool SaveFile(const String& data, const String& fileName) { return false; }
			virtual std::vector<char> ReadFile(const String& filename) { return std::vector<char>(); }
			virtual File LoadFile(const String& filename) { return File(); }

			const std::vector<String>& GetImageFileNames() const { return m_imageFileNames; }

		protected:
			std::vector<String> m_imageFileNames;
		};
	}
}

#endif