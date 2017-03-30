#ifndef SHADOW_HARDWARE_BUFFER_INCLUDE
#define SHADOW_HARDWARE_BUFFER_INCLUDE

#include "../Globals.h"

namespace sh
{
	namespace video
	{
		class HardwareBuffer
		{
		public:
			
			/*explicit*/ HardwareBuffer(Usage usage) : m_size(0U), m_usage(usage){}
			virtual ~HardwareBuffer(){}

			virtual void SetData(size_t offset, size_t length, const void* data) = 0;
			virtual void GetData(size_t offset, size_t length, const void* data) = 0;

			// Returns the size of the buffer in bytes
			size_t GetSize() const { return m_size; }
			Usage GetUsage() const { return m_usage; }
		protected:
			size_t m_size;
			Usage m_usage;

		private:
			//HardwareBuffer() : m_usage(Usage::STATIC){}
			//HardwareBuffer(const HardwareBuffer& other){}
		};
	}
}

#endif
