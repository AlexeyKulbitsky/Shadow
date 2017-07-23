#ifndef SHADOW_ANDROID_FILE_SYSTEM_INCLUDE
#define SHADOW_ANDROID_FILE_SYSTEM_INCLUDE

#include "../FileSystem.h"


#include <jni.h>

namespace sh
{

namespace io
{

	class AndroidFileSystem : public FileSystem
	{
	public:
		AndroidFileSystem();
		virtual ~AndroidFileSystem();
		virtual std::vector<char> ReadFile(const String& filename) override;

        void SetApkPath(const String& apkPath);
		virtual File LoadFile(const String& filename) override;
        virtual const FileInfo& FindFile(const String& fileName) override;

	private:
		String m_apkPath;
		Set<FileInfo> m_fileList;
	};

} // io

} // sh

#endif

