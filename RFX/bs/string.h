#pragma once
#include <string>
#include <sstream>
#include <assert.h>

namespace bs
{
	template< size_t bufferSize = 512, typename... TArgs >
	inline std::string str_format(const char * fmtString, TArgs... args)
	{
		int nBuf;
		char szBuffer[bufferSize];
		nBuf = sprintf_s(szBuffer, fmtString, args...);
		assert(nBuf < sizeof(szBuffer));//Output truncated as it was > sizeof(szBuffer)

		return std::string(szBuffer);
	}

	template< size_t bufferSize = 512, typename... TArgs >
	inline std::wstring str_formatw(const wchar_t * fmtString, TArgs... args)
	{
		int nBuf;
		wchar_t szBuffer[512];
		nBuf = swprintf_s(szBuffer, fmtString, args...);
		assert(nBuf < sizeof(szBuffer));//Output truncated as it was > sizeof(szBuffer)

		return std::wstring(szBuffer);
	}
}