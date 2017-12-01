#ifndef SHADOW_FILE_INCLUDE
#define SHADOW_FILE_INCLUDE

#include "../Globals.h"

namespace sh
{

namespace io
{

	class SHADOW_API File
	{
		friend class FileSystem;
		friend class WindowsFileSystem;
		friend class AndroidFileSystem;
	public:
		const bool IsEmpty() const { return m_data.size() == 0U; }
		const char* GetRawData() const { return m_data.data(); }
		const size_t GetSize() const { return m_data.size(); }
		const std::vector<char>& GetData() const { return m_data; }

	private:
		std::vector<char> m_data;
	};

} // io

} // sh

#endif