#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <functional>
#include <tuple>
#include "union_cast.h"
#include "utilities.h"

/*
 * An attempt at making a clean "type safe" interface for VTable hooks.
 * Doesn't seem to work with Link Time Code Generation
 */

namespace bs
{
#include "vtable_counter.inc"

	template<typename T, T>
	class vtable_hook;

	template<typename R, typename T, typename... Args, R(T::*mf)(Args...)>
	class vtable_hook<R(T::*)(Args...), mf>
	{
	public:
		using Func_t = R(T::*)(Args...);

		template<typename RT>
		struct DetourArgs
		{
			DetourArgs(T* _pthis, Func_t _originalFunc, Args... args) : pthis(_pthis), originalFunc(_originalFunc), tupleArgs(std::make_tuple(args...))
			{}

			RT callOriginal()
			{
				return call(bs::IntegerSequenceGenerator<sizeof...(Args)>::type());
			}

			T* pthis;
			Func_t originalFunc;
			std::tuple<Args...> tupleArgs;

		private:
			template<std::size_t... N>
			RT call(bs::IntegerSequence<N...>)
			{
				return (pthis->*originalFunc)(std::get<N>(tupleArgs)...);
			}
		};

		template<>
		struct DetourArgs < void >
		{
			DetourArgs(T* _pthis, Func_t _originalFunc, Args... args) : pthis(_pthis), originalFunc(_originalFunc), tupleArgs(std::make_tuple(args...))
			{}

			void callOriginal()
			{
				call(bs::IntegerSequenceGenerator<sizeof...(Args)>::type());
			}

			T* pthis;
			Func_t originalFunc;
			std::tuple<Args...> tupleArgs;

		private:
			template<std::size_t... N>
			void call(bs::IntegerSequence<N...>)
			{
				(pthis->*originalFunc)(std::get<N>(tupleArgs)...);
			}
		};

		using DetourArgs_t = DetourArgs < R >&;
		using FuncDetour_t = std::function < R(DetourArgs_t) >;

		static void hook(const T& object, FuncDetour_t f)
		{
			hook(&object, f);
		}

		static void hook(const T* object, FuncDetour_t f)
		{
			// You can't hook the same function twice.
			//if (origFunc) throw new std::exception;

			void** VTable = *(void***)(object);

			DWORD dwOld = 0;
			VirtualProtect(&VTable[get_index()], 4, PAGE_EXECUTE_READWRITE, &dwOld);

			origFunc = bs::union_cast<Func_t>(VTable[get_index()]);
			VTable[get_index()] = bs::union_cast<void*>(&DetourWrapper<R>::detour);

			VirtualProtect(&VTable[get_index()], 4, dwOld, 0);

			detourFunc = f;
		}

		/*static void unhook()
		{
			if (VTable)
			{
				DWORD dwOld = 0;
				VirtualProtect(&VTable[get_index()], 4, PAGE_EXECUTE_READWRITE, &dwOld);

				VTable[get_index()] = bs::union_cast<void*>(origFunc);

				VirtualProtect(&VTable[get_index()], 4, dwOld, 0);

				origFunc = nullptr;
				detourFunc = nullptr;
			}
		}*/

		static bool hooked()
		{
			return origFunc != nullptr;
		}

	private:
		static unsigned int get_index()
		{
			vtable_counter counter;
			return (counter.*(bs::union_cast<decltype(&vtable_counter::get0)>(mf)))();
		}

		template<typename ret>
		struct DetourWrapper
		{
			ret detour(Args... args)
			{
				return detourFunc(DetourArgs < R >(reinterpret_cast<T*>(this), origFunc, args...));
			}
		};

		template<>
		struct DetourWrapper<void>
		{
			void detour(Args... args)
			{
				detourFunc(DetourArgs < R >(reinterpret_cast<T*>(this), origFunc, args...));
			}
		};

		vtable_hook()
		{}

		//static void** VTable; //only needed for unhook which is not needed
		static Func_t origFunc;
		static FuncDetour_t detourFunc;
	};

	//template<typename R, typename T, typename... Args, R(T::*mf)(Args...)>
	//void** vtable_hook<R(T::*)(Args...), mf>::VTable = nullptr;
	template<typename R, typename T, typename... Args, R(T::*mf)(Args...)>
	typename vtable_hook<R(T::*)(Args...), mf>::Func_t vtable_hook<R(T::*)(Args...), mf>::origFunc = nullptr;
	template<typename R, typename T, typename... Args, R(T::*mf)(Args...)>
	typename vtable_hook<R(T::*)(Args...), mf>::FuncDetour_t vtable_hook<R(T::*)(Args...), mf>::detourFunc = nullptr;

#define HOOK_VTABLE_FUNCTION(obj, pFunc, detour) {using Hook_t = bs::vtable_hook<decltype(pFunc) ,pFunc>; Hook_t::hook(obj, detour);}
}