#ifndef SHADOW_ANDROID_FILE_SYSTEM_INCLUDE
#define SHADOW_ANDROID_FILE_SYSTEM_INCLUDE

#include "../FileSystem.h"

#if defined SHADOW_ANDROID
#include <jni.h>
#include <android/asset_manager.h>

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

		void SetAssetManager(AAssetManager* manager) { m_assetManager = manager; }
		void SetDataPath(const String& dataPath) { m_dataPath = dataPath; }

	private:
		AAssetManager* m_assetManager;
		String m_dataPath;
	};

} // io

} // sh

#endif


#endif