#pragma once

#include "bs\string_functor.h"
#include "bs\window.h"

#include "BF2Classes.h"

/*
* This file defines an easy to use interfaces for BF2 engine modders(/hackers)
* These functons are to be called only after BF2 has called create window.
*/

#include <d3d9.h>
#include <d3dx9.h>

namespace DICE
{
	namespace detail
	{
		extern CClassManager* g_pClassManager;
		extern CMainConsole* g_pMainConsole;
		extern CMainConsoleObjects* g_pMainConsoleObjects;
		extern CHudInformationLayer* g_pHudInformationLayer;

		extern ::std::map<::std::string, ID3DXEffect*> gShaders;

		void createHookedConsoleObject(bs::string_functor* pFunctor, const ::std::string& name, int minNumArgs, int maxNumArgs);
		void dumpConsoleObjects(::std::ostream& os);
	}
	void		init(bs::IWindow* pWindow);
	void		update();
	bs::IWindow	createConsoleWindow();
	::std::string invoke(const ::std::string& name);


	struct HudString;
	namespace HudItems
	{
		void		init();

		void		createBool(::std::string _name, bool* pvar = nullptr);
		void		createInt(::std::string _name, int* pvar = nullptr);
		void		createFloat(::std::string _name, float* pvar = nullptr);
		void		createString(::std::string _name, HudString* pvar = nullptr);

		bool*		getBool(::std::string _name);
		int*		getInt(::std::string _name);
		float*		getFloat(::std::string _name);
		char**		getString(::std::string _name);

		void		setBool(::std::string _name, bool _value);
		void		setInt(::std::string _name, int _value);
		void		setFloat(::std::string _name, float _value);
		void		setString(::std::string _name, std::string _value);
	}

	struct HudString
	{
		friend void HudItems::createString(::std::string, HudString*);

		HudString();
		HudString(const ::std::string& s);
		~HudString();

		HudString& operator= (const ::std::string& s);
		size_t size();
		const char* c_str();

	private:
		char* data;
	};

	namespace memory
	{
		using bf_free_t = void(__cdecl *)(void*); extern bf_free_t bf_free;
		using bf_malloc_t = void*(__cdecl *)(size_t, int); extern bf_malloc_t bf_malloc;
		using getSizeOfAllocation_t = int(__cdecl *)(void*); extern getSizeOfAllocation_t getSizeOfAllocation;
	}

	namespace ShaderManager
	{
		void init();

		::std::string setFloatBySemantic(const ::std::string& shaderName, const ::std::string& semantic, float val);
		::std::string setFloat2BySemantic(const ::std::string& shaderName, const ::std::string& semantic, const D3DXVECTOR2& val);
		::std::string setFloat3BySemantic(const ::std::string& shaderName, const ::std::string& semantic, const D3DXVECTOR3& val);
		::std::string setFloat4BySemantic(const ::std::string& shaderName, const ::std::string& semantic, const D3DXVECTOR4& val);
		::std::string setMatrixBySemantic(const ::std::string& shaderName, const ::std::string& semantic, const D3DXMATRIX& val);
		::std::string setTextureBySemantic(const ::std::string& shaderName, const ::std::string& semantic, IDirect3DTexture9* val);
	}

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

// Refractor style Vector stream operators
inline ::std::istream& operator >> (::std::istream& input, D3DXVECTOR2& rhs)
{
	input >> rhs.x;
	if (input.peek() != '/')
	{
		input.clear();
		rhs.y = rhs.x;
		return input;
	}
	input.ignore();

	input >> rhs.y;
	return input;

}

inline ::std::istream& operator >> (::std::istream& input, D3DXVECTOR3& rhs)
{
	input >> rhs.x;
	if (input.peek() != '/')
	{
		input.clear();
		rhs.z = rhs.y = rhs.x;
		return input;
	}
	input.ignore();

	input >> rhs.y;
	if (input.peek() != '/')
	{
		input.clear();
		rhs.z = rhs.y;
		return input;
	}
	input.ignore();

	input >> rhs.z;

	return input;

}

inline ::std::istream& operator >> (::std::istream& input, D3DXVECTOR4& rhs)
{
	input >> rhs.x;
	if (input.peek() != '/')
	{
		input.clear();
		rhs.w = rhs.z = rhs.y = rhs.x;
		return input;
	}
	input.ignore();

	input >> rhs.y;
	if (input.peek() != '/')
	{
		input.clear();
		rhs.w = rhs.z = rhs.y;
		return input;
	}
	input.ignore();

	input >> rhs.z;
	if (input.peek() != '/')
	{
		input.clear();
		rhs.w = rhs.z;
		return input;
	}
	input.ignore();

	input >> rhs.w;

	return input;
}