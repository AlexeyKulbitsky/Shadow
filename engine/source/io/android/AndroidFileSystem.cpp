#include "AndroidFileSystem.h"

#if defined SHADOW_ANDROID

namespace sh
{

namespace io
{

	AndroidFileSystem::AndroidFileSystem() {}
	AndroidFileSystem::~AndroidFileSystem() {}

	std::vector<char> AndroidFileSystem::ReadFile( const String& filename )
	{
        //String fullName = "shaders/" + filename;
		//AAsset* asset = AAssetManager_open(m_assetManager, fullName.c_str(), AASSET_MODE_STREAMING);

        std::vector<char> buffer;

        AAsset *asset = NULL;

        std::vector<String> folders = {"fonts", "gui", "icons", "models", "scenes", "shaders", "textures"};

        for (size_t i = 0U; i < folders.size(); ++i)
        {
            AAssetDir* assetDir = AAssetManager_openDir(m_assetManager, folders[i].c_str());
            const char* tempName;
            while ((tempName = AAssetDir_getNextFileName(assetDir)) != NULL)
            {
                if(!strcmp(tempName, filename.c_str()))
                {
                    String fullName = folders[i] + "/" + tempName;
                    asset = AAssetManager_open(m_assetManager, fullName.c_str(), AASSET_MODE_STREAMING);
                    //AAssetDir_close(assetDir);
                    break;
                }
            }
            AAssetDir_close(assetDir);
        }


        if (asset == NULL)
            return buffer;

		size_t size = AAsset_getLength(asset);
		buffer.resize(size);
		int read = AAsset_read(asset, buffer.data(), size);

        AAsset_close(asset);

		return buffer;
	}

	File AndroidFileSystem::LoadFile(const String& filename)
	{
		File file;
		file.m_data = ReadFile(filename);
		return file;
	}

} // io

} // sh

#endif