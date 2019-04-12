#ifndef SHADOW_SINGLETON_INCLUDE
#define SHADOW_SINGLETON_INCLUDE

#include <type_traits>

namespace sh
{
	template<typename T>
	class Singleton
	{
	public:
		static T* GetInstance() { return instance(); }

		static void CreateInstance()
		{
			instance() = new T();
		}

		template<typename ChildType>
		static void CreateInstance()
		{
			static_assert(std::is_base_of<T, ChildType>::value, "Invalid child type for creating singleton instance!");
			instance() = new ChildType();
		}

		static void DestroyInstance()
		{
			delete instance();
			instance() = nullptr;
		}

		static void SetInstance(T* _instance)
		{
			if (instance())
				delete instance();
			instance() = _instance;
		}

	protected:
		Singleton() { }
		Singleton(const Singleton&) { }
		Singleton(Singleton&&) { }
		Singleton& operator=(const Singleton&) { return *this; }
		Singleton& operator=(Singleton&& ) { return *this; }

		static T*& instance()
		{
			static T* inst = nullptr;
			return inst;
		}

	protected:
		//static T* s_instance;
	};

	//template<typename T>
	//T* Singleton<T>::s_instance = nullptr;

} // sh

#endif