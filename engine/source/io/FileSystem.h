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

		class FileSystem
		{
		public:
			FileSystem();
			~FileSystem();

			void Init();

			const String& GetWorkingDirectory() const;

			void AddFolder(const String& folder, bool recursive = true);
			const FileInfo& FindFile(const String& fileName);

		private:
			void CollectFilesFromFolder(const String& folder, bool recursive = true);

		private:
			Set<String> m_folders;
			Set<FileInfo> m_fileList;
			String m_workingDirectoryPath;
		};
	}
}

#endif