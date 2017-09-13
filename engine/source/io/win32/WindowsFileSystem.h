#ifndef SHADOW_WINDOWS_FILE_SYSTEM_INCLUDE
#define SHADOW_WINDOWS_FILE_SYSTEM_INCLUDE


#include "../FileSystem.h"

namespace sh
{

namespace io
{

	class WindowsFileSystem : public FileSystem
	{
	public:
		WindowsFileSystem();
		virtual ~WindowsFileSystem();

		virtual void Init() override;

		virtual const String& GetWorkingDirectory() const override;

		virtual void AddFolder(const String& folder, bool recursive = true) override;
		virtual const FileInfo& FindFile(const String& fileName) override;

		virtual bool SaveFile(const std::vector<char>& data, const String& fileName) override;
		virtual bool SaveFile(const String& data, const String& fileName) override;
		virtual std::vector<char> ReadFile(const String& filename) override;
		virtual File LoadFile(const String& filename) override;

	private:
		void CollectFilesFromFolder(const String& folder, bool recursive = true);
		void UpdateFileGroups();

	private:
		Set<String> m_folders;
		Set<FileInfo> m_fileList;
		String m_workingDirectoryPath;
	};

} // io

} // sh

#endif

