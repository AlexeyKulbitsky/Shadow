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
	public:
		const std::vector<u8>& GetData() const { return m_data; }
		u8* GetData() { return m_data.data(); }
	private:
		std::vector<u8> m_data;
	};

} // io

} // sh

#endif