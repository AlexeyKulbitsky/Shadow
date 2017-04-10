#ifndef SHADOW_SINGLETON_INCLUDE
#define SHADOW_SINGLETON_INCLUDE

#include <type_traits>

namespace sh
{
	template<typename T>
	class Singleton
	{
	public:
		static T* GetInstance() { return s_instance; }

		static void CreateInstance()
		{
			s_instance = new T();
		}

		template<typename ChildType>
		static void CreateInstance()
		{
			static_assert(std::is_base_of<T, ChildType>::value, "Invalid child type for creating singleton instance!");
			s_instance = new ChildType();
		}

		static void DestroyInstance()
		{
			delete s_instance;
			s_instance = nullptr;
		}

	protected:
		Singleton() { }
		Singleton(const Singleton&) { }
		Singleton(Singleton&&) { }
		Singleton& operator=(const Singleton&) { return *this; }
		Singleton& operator=(Singleton&& ) { return *this; }

	protected:
		static T* s_instance;
	};

	template<typename T>
	T* Singleton<T>::s_instance = nullptr;

} // sh

#endif