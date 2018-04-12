#ifndef SHADOW_MAC_FILE_SYSTEM_INCLUDE
#define SHADOW_MAC_FILE_SYSTEM_INCLUDE


#include "../FileSystem.h"

namespace sh
{

namespace io
{

	class MacFileSystem : public FileSystem
	{
	public:
		MacFileSystem();
		virtual ~MacFileSystem();

		virtual void Init() override;

		virtual const String& GetWorkingDirectory() const override;

		virtual void AddFolder(const String& folder, bool recursive = true) override;
		virtual void AddInternalFolder(const String& folder) override;

		virtual bool SaveFile(const std::vector<char>& data, const String& fileName) override;
		virtual bool SaveFile(const String& data, const String& fileName) override;
		virtual File LoadFile(const String& filename) override;
		virtual File LoadFile(std::weak_ptr<FileInfo> fileInfo) override;

		virtual bool CreateFolder(const String& path) override;
		virtual bool Rename(const String& oldName, const String& newName) override;
		virtual bool Delete(const String& filePath) override;
		virtual bool Copy(const String& srcFileName, const String& dstFileName) override;
        
        virtual void ShowOpenFileDialog() override;

	private:
		void CollectFilesFromFolder(const String& folder, const SPtr<FolderInfo>& root, bool recursive = true);

	private:
		String m_workingDirectoryPath;
	};

} // io

} // sh

#endif

