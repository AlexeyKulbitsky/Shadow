#ifndef SHADOW_FILE_INCLUDE
#define SHADOW_FILE_INCLUDE

#include "../Globals.h"

namespace sh
{

namespace io
{

	class File
	{
		friend class FileSystem;
		friend class WindowsFileSystem;
		friend class AndroidFileSystem;
	public:
		const std::vector<char>& GetData() const { return m_data; }

	private:
		std::vector<char> m_data;
	};

} // io

} // sh

#endif