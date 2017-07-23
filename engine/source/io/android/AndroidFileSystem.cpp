#include "AndroidFileSystem.h"

#include <unzip.h>
unzFile apkFile = nullptr;

namespace sh
{

namespace io
{

	AndroidFileSystem::AndroidFileSystem()
    {
    }

	AndroidFileSystem::~AndroidFileSystem()
    {
        unzClose(apkFile);
    }

	std::vector<char> AndroidFileSystem::ReadFile( const String& filename )
	{
        const auto& info = FindFile(filename);
        if (info.name == "")
            return std::vector<char>();

        int res = unzLocateFile(apkFile, info.absolutePath.c_str(), 0);
        if (res != UNZ_OK)
            return std::vector<char>();

        res = unzOpenCurrentFile(apkFile);
        if (res != UNZ_OK)
            return std::vector<char>();

        const size_t bufferSize = 5 * 1024 * 1024;
        char buffer[bufferSize];

        int bytesReaded = unzReadCurrentFile(apkFile, buffer, bufferSize);
        if (bytesReaded < 0)
        {
            unzCloseCurrentFile(apkFile);
            return std::vector<char>();
        }

        std::vector<char> result;
        result.insert(result.end(), &buffer[0], &buffer[bytesReaded]);
        unzCloseCurrentFile(apkFile);
        return result;
	}

    void AndroidFileSystem::SetApkPath(const String& apkPath)
    {
        m_apkPath = apkPath;

        apkFile = unzOpen(m_apkPath.c_str());
        if (apkFile == NULL)
            return;

        unz_global_info apkFileInfo;
        int res = unzGetGlobalInfo(apkFile, &apkFileInfo);
        if (res != UNZ_OK)
        {
            unzClose(apkFile);
            return;
        }

        // Collect all files
        for (uLong i = 0; i < apkFileInfo.number_entry; ++i)
        {
            // Get info about current file.
            const size_t maxFileName = 512;
            unz_file_info file_info;
            char filename[ maxFileName ];
            res = unzGetCurrentFileInfo(apkFile, &file_info, filename, maxFileName, NULL, 0, NULL, 0 );
            if (res != UNZ_OK)
            {
                unzClose(apkFile);
                return;
            }

            String fullPath = filename;
            String::size_type pos = fullPath.find_first_of('/');
            if (fullPath.substr(0, pos) == "assets")
            {
                String::size_type p = fullPath.find_last_of('/');
                String fileName = fullPath.substr(p + 1);

                FileInfo info;
                info.name = fileName;
                info.absolutePath = fullPath;
                m_fileList.insert(info);
            }


            if ((i + 1) < apkFileInfo.number_entry)
            {
                res = unzGoToNextFile(apkFile);
                if (res != UNZ_OK)
                {
                    unzClose(apkFile);
                    return;
                }
            }
        }
    }

	File AndroidFileSystem::LoadFile(const String& filename)
	{
		File file;
		file.m_data = ReadFile(filename);
		return file;
	}

    const FileInfo& AndroidFileSystem::FindFile(const String& fileName)
    {
        static const FileInfo errorFile("", "");
        Set<FileInfo>::iterator it = m_fileList.find(FileInfo(fileName, ""));
        if (it != m_fileList.end())
        {
            return *it;
        }
        return errorFile;
    }

} // io

} // sh

