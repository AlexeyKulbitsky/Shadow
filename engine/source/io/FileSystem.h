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
			const SPtr<FolderInfo> GetRoot() const { return m_root; }

		protected:
			std::vector<String> m_imageFileNames;
			SPtr<FolderInfo> m_root;
		};
	}
}

#endif