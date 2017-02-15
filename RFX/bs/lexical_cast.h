#pragma once
#include <string>
#include <sstream>

/*
 * Defines a cast function which uses streams to cast between std::string
 * and any type that defines stream operators "<<" and ">>".
 */

namespace bs
{
	class bad_lexical_cast : public std::exception
	{};

	namespace detail
	{
		// Helper struct, which may not be needed if Microsoft implements partial specialization of function templates.
		template<typename TReturn, typename TVal>
		struct lexical_cast_helper
		{
			TReturn operator ()(const TVal& val)
			{
				return val;
			}
		};

		template<typename TVal>
		struct lexical_cast_helper<std::string, TVal>
		{
			std::string operator ()(const TVal& val)
			{
				std::stringstream ss;
				ss << val;
				return ss.str();
			}
		};

		template<typename TReturn>
		struct lexical_cast_helper<TReturn, std::string>
		{
			TReturn operator ()(const std::string& val)
			{
				TReturn retVal;
				std::stringstream ss(val);
				ss >> retVal;
				if (ss.fail())
				{
					bad_lexical_cast ex;
					throw ex;
				}
				return retVal;
			}
		};

		/*template<typename T>
		struct lexical_cast_helper<T*, std::string>
		{
			T* operator ()(const std::string& val)
			{
				void* retVal;
				std::stringstream ss(val);
				ss >> retVal;
				if (ss.fail())
				{
					bad_lexical_cast ex;
					throw ex;
				}
				return reinterpret_cast<T*>(retVal);
			}
		};*/

	}

	template <typename TReturn, typename TVal>
	inline TReturn lexical_cast(const TVal& val)
	{
		detail::lexical_cast_helper<TReturn, TVal> caster;
		return caster(val);
	}

	// some optimizations generally way faster than streams
	template <>
	inline std::string lexical_cast(const std::string& s)
	{
		return s;
	}

	template <>
	inline std::string lexical_cast(const int& val)
	{
		// base 10 signed int max len is 11 chars
		char buf[11];
		_itoa_s(val, buf, 11, 10);
		return buf;
	}

	template <>
	inline int lexical_cast(const std::string& s)
	{
		return (int)strtol(s.c_str(), nullptr, 10);
	}

	template <>
	inline unsigned int lexical_cast(const std::string& s)
	{
		return (unsigned int)strtoul(s.c_str(), nullptr, 10);
	}

	template <>
	inline long lexical_cast(const std::string& s)
	{
		return strtol(s.c_str(), nullptr, 10);
	}

	template <>
	inline unsigned long lexical_cast(const std::string& s)
	{
		return strtoul(s.c_str(), nullptr, 10);
	}

	template <>
	inline float lexical_cast(const std::string& s)
	{
		return (float)atof(s.c_str());
	}

	template <>
	inline double lexical_cast(const std::string& s)
	{
		return atof(s.c_str());
	}

	// c strings
	// allows to do it without any string copies
	template <typename TReturn>
	inline TReturn lexical_cast(const char* s)
	{
		//detail::lexical_cast_helper<TReturn, std::string> caster;
		//return caster(std::string(s));
		return std::string(s);
	}

	template <>
	inline int lexical_cast(const char*s)
	{
		return (int)strtol(s, nullptr, 10);
	}

	template <>
	inline unsigned int lexical_cast(const char* s)
	{
		return (unsigned int)strtoul(s, nullptr, 10);
	}

	template <>
	inline long lexical_cast(const char* s)
	{
		return strtol(s, nullptr, 10);
	}

	template <>
	inline unsigned long lexical_cast(const char* s)
	{
		return strtoul(s, nullptr, 10);
	}

	template <>
	inline float lexical_cast(const char* s)
	{
		return (float)atof(s);
	}

	template <>
	inline double lexical_cast(const char* s)
	{
		return atof(s);
	}
}