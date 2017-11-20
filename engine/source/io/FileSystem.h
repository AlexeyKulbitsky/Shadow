#ifndef SHADOW_FILE_SYSTEM_INCLUDE
#define SHADOW_FILE_SYSTEM_INCLUDE

#include "../Globals.h"
#include "File.h"

namespace sh
{
	namespace io
	{
		struct FileSystemComponent
		{
			enum class Type
			{
				File,
				Folder
			};

			FileSystemComponent(const String& _name, const String& _absolutePath) : name(_name), absolutePath(_absolutePath) {}
			void Rename(const String& newName);

			virtual Type GetType() const = 0;

			String name;
			String absolutePath;
		};



		struct FileInfo : public FileSystemComponent
		{
			FileInfo(const String& name, const String& absolutePath) : FileSystemComponent(name, absolutePath) {}
			FileInfo(const FileInfo& other) : FileSystemComponent(other.name, other.absolutePath) {}
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

			virtual Type GetType() const override final { return Type::File; }
		};

		struct FolderInfo : public FileSystemComponent
		{
			FolderInfo(const String& name, const String& absolutePath) : FileSystemComponent(name, absolutePath) {}

			virtual Type GetType() const override final { return Type::Folder; }

			std::weak_ptr<FileSystemComponent> FindChildByName(const String& name);

			std::vector<SPtr<FileSystemComponent>> children;
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
			virtual void AddInternalFolder(const String& folder) {}
			//virtual const FileInfo& FindFile(const String& fileName) 
			//{ 
			//	static FileInfo res;
			//	return res;
			//}
			
			virtual bool SaveFile(const std::vector<char>& data, const String& fileName) { return false; }
			virtual bool SaveFile(const String& data, const String& fileName) { return false; }
			//virtual std::vector<char> ReadFile(const String& filename) { return std::vector<char>(); }
			virtual File LoadFile(const String& filename) { return File(); }
			virtual File LoadFile(std::weak_ptr<FileInfo> fileInfo) { return File(); }

			virtual bool CreateFolder(const String& path) { return false; }
			virtual bool Rename(const String& oldPath, const String& newPath) { return false; }
			virtual bool Delete(const String& filePath) { return false; }

			void UpdateResourceGroups();

			const std::vector<std::weak_ptr<FileInfo>>& GetImageFileInfos() const { return m_imageFileInfos; }
			const std::vector<std::weak_ptr<FileInfo>>& GetModelFileInfos() const { return m_modelFileInfos; }
			const std::vector<std::weak_ptr<FileInfo>>& GetMaterialFileInfos() const { return m_materialFileInfos; }
			const std::vector<std::weak_ptr<FileInfo>>& GetRenderTechniqueFileInfos() const { return m_renderTechniqueFileInfos; }
			const SPtr<FolderInfo> GetRoot() const { return m_root; }
			const std::vector<SPtr<FolderInfo>>& GetInternalFolders() const { return m_internalDataRoots; }

		private:
			void UpdateRecursive(const SPtr<FolderInfo>& fsComponent, 
								 const std::vector<String>& imageExtensions,
								 const std::vector<String>& modelExtensions);

		protected:
			std::vector<std::weak_ptr<FileInfo>> m_imageFileInfos;
			std::vector<std::weak_ptr<FileInfo>> m_modelFileInfos;
			std::vector<std::weak_ptr<FileInfo>> m_materialFileInfos;
			std::vector<std::weak_ptr<FileInfo>> m_renderTechniqueFileInfos;

			// Root folder for current project data
			SPtr<FolderInfo> m_root;

			// Root folders for internal engine data
			std::vector<SPtr<FolderInfo>> m_internalDataRoots;
		};
	}
}

#endif