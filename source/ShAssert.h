#ifndef SHADOW_ASSERT_INCLUDE
#define SHADOW_ASSERT_INCLUDE

#if defined(_DEBUG) || defined(SHADOW_WINDOWS)
#define ASSERTS_ENABLED
#endif 

namespace assertLib
{
	enum class AssertType
	{
		BREAK,
		CONTINUE,
	};

	typedef	AssertType(*AssertHandler)(const char* condition, const char* msg,
		const char* file, int line);

	AssertType TestCondition(const char* condition, const char* file,
		int line, const char* msg, ...);

	AssertType TestConditionOnce(const char* condition, const char* file,
		int line, const char* msg, ...);

	void SetAssertHandler(AssertHandler);
}

assertLib::AssertType ShadowDefaultHandler(const char* condition, const char* msg, const char* file, const int line);

#ifdef _WIN32
#define SHBREAK() __debugbreak()
#define SHBREAKIF(x) if (x) { SHBREAK(); }
#else 
#define SHBREAK() break
#endif 


#ifdef ASSERTS_ENABLED

#define SHASSERT(cond) \
		do { \
			if (!(cond)) { \
				if ( assertLib::TestCondition(#cond, __FILE__, __LINE__, 0) == assertLib::AssertType::BREAK)  \
				{\
					SHBREAK();  \
				}\
			} \
		} while(0)

#define SHASSERT_MSG(cond, msg, ...) \
		do { \
			if (!(cond)) { \
				if (assertLib::TestCondition(#cond, __FILE__, __LINE__, (msg), ##__VA_ARGS__) == assertLib::AssertType::BREAK) \
					{ \
						SHBREAK();\
					} \
				} \
			} while(0)


#define SHASSERT_ONCE(cond) \
		do { \
			if (!(cond)) { \
				if (assertLib::TestConditionOnce(#cond, __FILE__, __LINE__, 0) == assertLib::AssertType::BREAK) \
					{ \
						SHBREAK();\
					} \
				} \
			} while(0)

#define SHASSERT_MSG_ONCE(cond, msg, ...) \
		do { \
			if (!(cond)) { \
				if (assertLib::TestConditionOnce(#cond, __FILE__, __LINE__, (msg), ##__VA_ARGS__) == assertLib::AssertType::BREAK) \
					{ \
						SHBREAK();\
					} \
				} \
			} while(0)

#else
#define SHASSERT(condition) do { SHUNUSED(condition); } while(0)
#define SHASSERT_MSG(condition, msg, ...) do { SHUNUSED(condition); SHUNUSED(msg); } while(0)	
#define SHASSERT_ONCE(condition) do { SHUNUSED(condition); } while(0)
#define SHASSERT_MSG_ONCE(condition, msg, ...) do { SHUNUSED(condition); SHUNUSED(msg); } while(0)	
#endif





#define  SASSERT(cond) SHASSERT(cond)

#define	 SASSERT2(cond, msg, ...)	SHASSERT_MSG(cond, msg, ##__VA_ARGS__)



#endif
