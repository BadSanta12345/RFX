#pragma once

#include "bs\string_functor.h"
#include "bs\window.h"

#include "BF2Classes.h"

/*
* This file defines an easy to use interfaces for BF2 engine modders(/hackers)
* These functons are to be called only after BF2 has called create window.
*/

namespace BF2
{
	// These are singletons in BF2 and will not change during process lifetime
	//extern detail::CClassManager* g_pClassManager;
	//extern detail::CMainConsole* g_pMainConsole;

	namespace detail
	{
		void createHookedConsoleObject(bs::string_functor* pFunctor, const ::std::string& name, int minNumArgs, int maxNumArgs);
		void dumpConsoleObjects(::std::ostream& os);
	}
	void		init(bs::IWindow* pWindow);
	void		update();
	bs::IWindow	createConsoleWindow();
	::std::string invoke(const ::std::string& name);


	/*struct HudString;
	namespace HudItems
	{
		void		createBool(std::string _name, bool* pvar = nullptr);
		void		createInt(std::string _name, int* pvar = nullptr);
		void		createFloat(std::string _name, float* pvar = nullptr);
		void		createString(std::string _name, HudString* pvar = nullptr);

		bool*		getBool(std::string _name);
		int*		getInt(std::string _name);
		float*		getFloat(std::string _name);
		char**		getString(std::string _name);

		void		setBool(std::string _name, bool _value);
		void		setInt(std::string _name, int _value);
		void		setFloat(std::string _name, float _value);
		void		setString(std::string _name, std::string _value);
	}*/

/* Template functions */

	template<typename TFunc, typename... TDefaultArgs>
	void createConsoleObject(TFunc _func, const ::std::string& name, TDefaultArgs... _defaultArgs)
	{
		using traits = bs::function_traits<TFunc>;
		static_assert(sizeof...(TDefaultArgs) <= traits::arity, "Too many default args for console object.");

		using TFunctor = bs::basic_string_functor<TFunc, TDefaultArgs...>;
		detail::createHookedConsoleObject(
			//!!This is not deleted anywhere!!
			// TODO: proper clean up on exit
			// not very critical as windows will do the clean up anyway
			new TFunctor(_func, _defaultArgs...),
			name,
			TFunctor::nrOfArgs - TFunctor::nrOfDefaultArgs,
			TFunctor::nrOfArgs);
	}
}