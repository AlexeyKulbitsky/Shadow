#include "ShAssert.h"

#include <cstdio>
#include <cstdarg>
#include <string>
#include <set>

///////////////////////////////////////////////////////////////////////////////

#pragma warning( disable : 4996)

std::set<std::string> s_assertOnceMessages;

///////////////////////////////////////////////////////////////////////////////

assertLib::AssertType DefaultHandler(const char* condition, const char* msg,
	const char* file, const int line)
{
	std::printf("%s(%d): Assert Failure: ", file, line);

	if (condition != NULL)
		std::printf("'%s' ", condition);

	if (msg != NULL)
		std::printf("%s", msg);

	std::printf("\n");

	return assertLib::AssertType::CONTINUE;
}

///////////////////////////////////////////////////////////////////////////////

static assertLib::AssertHandler s_handler = &DefaultHandler;

///////////////////////////////////////////////////////////////////////////////

void assertLib::SetAssertHandler(assertLib::AssertHandler h)
{
	s_handler = h;
}

///////////////////////////////////////////////////////////////////////////////

assertLib::AssertType assertLib::TestCondition(const char* condition, const char* file,
	int line, const char* msg, ...)
{
	char messageBuffer[8192];
	const char* message = NULL;
	if (msg != NULL)
	{
		{
			va_list args;
			va_start(args, msg);
			vsnprintf(messageBuffer, 8192, msg, args);
			va_end(args);
		}

		message = messageBuffer;
	}

	return s_handler(condition, message, file, line);
}

///////////////////////////////////////////////////////////////////////////////

assertLib::AssertType assertLib::TestConditionOnce(const char* condition, const char* file,
	int line, const char* msg, ...)
{
	char messageBuffer[8192];
	const char* message = NULL;
	if (msg != NULL)
	{
		{
			va_list args;
			va_start(args, msg);
			vsnprintf(messageBuffer, 8192, msg, args);
			va_end(args);
		}

		message = messageBuffer;
	}

	if (message == NULL)
	{
		sprintf(messageBuffer, "%s:%d", file, line);
		std::string tmp = messageBuffer;
		if (s_assertOnceMessages.find(tmp) != s_assertOnceMessages.end())
		{
			return assertLib::AssertType::CONTINUE;
		}
		s_assertOnceMessages.insert(tmp);
	}
	else
	{
		std::string tmp = message;
		if (s_assertOnceMessages.find(tmp) != s_assertOnceMessages.end())
		{
			return assertLib::AssertType::CONTINUE;
		}
		s_assertOnceMessages.insert(tmp);
	}

	return s_handler(condition, message, file, line);
}
