#ifndef SHADOW_VK_DELETER_INCLUDE
#define SHADOW_VK_DELETER_INCLUDE

#include "../../pempek_assert.h"
#include <functional>
//#define VK_USE_PLATFORM_WIN32_KHR
//#include <vulkan/vulkan.h>

#include "VulkanFunctions.h"

namespace sh
{
	namespace video
	{
		template <typename T>
		class VulkanDeleter 
		{
		public:
			VulkanDeleter() : VulkanDeleter([](T, VkAllocationCallbacks*) {}) {}

			VulkanDeleter(std::function<void(T, VkAllocationCallbacks*)> deletef)
			{
				this->deleter = [=](T obj) { deletef(obj, nullptr); };
			}

			VulkanDeleter(const VulkanDeleter<VkInstance>& instance, std::function<void(VkInstance, T, VkAllocationCallbacks*)> deletef)
			{
				this->deleter = [&instance, deletef](T obj) { deletef(instance, obj, nullptr); };
			}

			VulkanDeleter(const VulkanDeleter<VkDevice>& device, std::function<void(VkDevice, T, VkAllocationCallbacks*)> deletef)
			{
				this->deleter = [&device, deletef](T obj) { deletef(device, obj, nullptr); };
			}

			~VulkanDeleter()
			{
				Cleanup();
			}

			const T* operator &() const 
			{
				return &object;
			}

			T* Replace() 
			{
				Cleanup();
				return &object;
			}

			operator T() const 
			{
				return object;
			}

			void operator=(T rhs) 
			{
				if (rhs != object) 
				{
					Cleanup();
					object = rhs;
				}
			}

			template<typename V>
			bool operator==(V rhs) 
			{
				return object == T(rhs);
			}

		private:
			T object{ VK_NULL_HANDLE };
			std::function<void(T)> deleter;

			void Cleanup() 
			{
				if (object != VK_NULL_HANDLE) 
				{
					deleter(object);
				}
				object = VK_NULL_HANDLE;
			}
		};
	}
}

#endif