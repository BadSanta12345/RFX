#include "Refractor.h"

namespace DICE
{
	namespace HudItems
	{
		::std::vector<void*> pureScriptItems; // Items created from script or for some other reason without a pointer to a variable

		bool* getBool(::std::string _name)
		{
			return detail::g_pHudInformationLayer->getBoolByName(&std::string(_name.c_str()));
		}

		int* getInt(::std::string _name)
		{
			return detail::g_pHudInformationLayer->getIntByName(&std::string(_name.c_str()));
		}

		float* getFloat(::std::string _name)
		{
			return detail::g_pHudInformationLayer->getFloatByName(&std::string(_name.c_str()));
		}

		char** getString(::std::string _name)
		{
			return detail::g_pHudInformationLayer->getStringByName(&std::string(_name.c_str()));
		}

		void createBool(::std::string _name, bool* pvar)
		{
			if (!getBool(_name))
			{
				if (!pvar)
				{
					pvar = new bool;
					pureScriptItems.push_back(pvar);
				}
				detail::g_pHudInformationLayer->createBool(&std::string(_name.c_str()), pvar);
			}
		}

		void createInt(::std::string _name, int* pvar)
		{
			if (!getInt(_name))
			{
				if (!pvar)
				{
					pvar = new int;
					pureScriptItems.push_back(pvar);
				}
				detail::g_pHudInformationLayer->createInt(&std::string(_name.c_str()), pvar);
			}
		}

		void createFloat(::std::string _name, float* pvar)
		{
			if (!getFloat(_name))
			{
				if (!pvar)
				{
					pvar = new float;
					pureScriptItems.push_back(pvar);
				}
				detail::g_pHudInformationLayer->createFloat(&std::string(_name.c_str()), pvar);
			}
		}

		void createString(::std::string _name, HudString* pvar)
		{
			if (!getString(_name))
			{
				char** ppvar;
				if (!pvar)
				{
					ppvar = (char**)memory::bf_malloc(sizeof(char*), 0);
					pureScriptItems.push_back(ppvar);
					*ppvar = "";
				}
				else
				{
					ppvar = &pvar->data;
				}
				detail::g_pHudInformationLayer->createString(&std::string(_name.c_str()), ppvar);
			}
		}

		void init()
		{
			createConsoleObject(bs::lambda([](::std::string _name) {auto r = getBool(_name); return r ? *r : false; }), "hudItemsgetBool");
			createConsoleObject(bs::lambda([](::std::string _name) {auto r = getInt(_name); return r ? *r : 0;}), "hudItemsgetInt");
			createConsoleObject(bs::lambda([](::std::string _name) {auto r = getFloat(_name); return r ? *r : 0.0f; }), "hudItemsgetFloat");
			createConsoleObject(bs::lambda([](::std::string _name)->char* {auto r = getString(_name); return r ? *r : ""; }),	"hudItemsgetString");

			createConsoleObject(setInt, "hudItemssetInt");

			createConsoleObject(bs::lambda([](::std::string _name){ createBool(_name); }),	"hudItemscreateBool");
			createConsoleObject(bs::lambda([](::std::string _name){ createInt(_name); }),	"hudItemscreateInt");
			createConsoleObject(bs::lambda([](::std::string _name){ createFloat(_name); }),	"hudItemscreateFloat");
			createConsoleObject(bs::lambda([](::std::string _name){ createString(_name); }),"hudItemscreateString");
		}

		void setBool(::std::string _name, bool _value)
		{
			auto pItem = detail::g_pHudInformationLayer->getBoolByName(&std::string(_name.c_str()));
			if (!pItem)
			{
				pItem = getBool(_name);
			}
			if (pItem)
			{
				*pItem = _value;
			}
		}

		void setInt(::std::string _name, int _value)
		{
			auto pItem = detail::g_pHudInformationLayer->getIntByName(&std::string(_name.c_str()));
			if (pItem)
			{
				*pItem = _value;
			}
		}

		void setFloat(::std::string _name, float _value)
		{
			auto pItem = detail::g_pHudInformationLayer->getFloatByName(&std::string(_name.c_str()));
			if (pItem)
			{
				*pItem = _value;
			}
		}

		void setString(::std::string _name, ::std::string _value)
		{
			auto pItem = detail::g_pHudInformationLayer->getStringByName(&std::string(_name.c_str()));
			if (pItem)
			{
				auto size = memory::getSizeOfAllocation(*pItem);
				if (size > (int)_value.size())
				{
					strcpy_s(*pItem, size, _value.c_str());
				}
				else
				{
					memory::bf_free(*pItem);
					*pItem = reinterpret_cast<char*>(memory::bf_malloc(sizeof(char) * (_value.size() + 1), 0));
					strcpy_s(*pItem, _value.size() + 1, _value.c_str());
				}
			}
		}
	}
}