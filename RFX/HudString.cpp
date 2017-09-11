#include "Refractor.h"

namespace DICE
{
	HudString::HudString()
	{
		data = nullptr;
	}

	HudString::HudString(const ::std::string& s)
	{
		data = reinterpret_cast<char*>(memory::bf_malloc(sizeof(char) * (s.size() + 1), 0));
		strcpy_s(data, s.size() + 1, s.c_str());
	}

	HudString::~HudString()
	{
		// Memory.dll seems to automagically deallocate data and doing it here gives an error!? TODO: investigate wether this actuallly leaks or what
		//if(data)
		//	dice::memory::bf_free(data);
	}

	HudString& HudString::operator= (const ::std::string& s)
	{
		auto size = this->size();
		if (size > (int)s.size())
		{
			strcpy_s(data, size, s.c_str());
		}
		else
		{
			if (data)
				memory::bf_free(data);
			data = reinterpret_cast<char*>(memory::bf_malloc(sizeof(char) * (s.size() + 1), 0));
			strcpy_s(data, s.size() + 1, s.c_str());
		}
		return *this;
	}

	size_t HudString::size()
	{
		if (data)
		{
			return memory::getSizeOfAllocation(data);
			//return strlen(data)-1;
		}
		return 0;
	}

	const char* HudString::c_str()
	{
		return data;
	}
}