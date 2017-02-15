#pragma once
/*
 * Defines a function wrapper class which allows most functions to be called with std::vector<std::string> as parameter and return an std::string representation of the actual return value.
 * When called the object will automatically convert the string arguments to the appropriate type (with stream operators << >> / lexical_cast) and calls the wrapped function.
 * This could be used in simple scripting engine or in-game development console.
 */
#include "lexical_cast.h"
#include "utilities.h"
#include "function_traits.h"
#include <functional>
#include <vector>
#include <algorithm> 
#include <cctype>
#include <type_traits>

#include <array> // experimental

namespace bs
{
	class string_functor
	{
	public:
		virtual std::string operator ()(std::vector<std::string>& _args) const = 0;
	};
	
	template<typename TFunc, typename... TDefaultArgs>
	class basic_string_functor : public string_functor
	{
		using traits = bs::function_traits<TFunc>;
		using TConstReferenceArgs = typename traits::tuple_arguments_type;
		using TConstArgs = typename bs::tuple_refrence_remover<TConstReferenceArgs>::type;
		using TArgs = typename bs::tuple_const_remover<TConstArgs>::type;

		class bad_argument_conversion : public std::exception
		{
		public:
			bad_argument_conversion(size_t _index, const std::string& _typeName) : index(_index), typeName(_typeName) {}
			size_t index;
			std::string typeName;
		};

	public:
		const static size_t nrOfArgs = traits::arity;
		const static size_t nrOfDefaultArgs = sizeof...(TDefaultArgs);

		basic_string_functor(const TFunc& _func, TDefaultArgs... _defaultArgs) : m_pFunc(_func), m_defaultArgs(std::make_tuple(_defaultArgs...))
		{}

		virtual std::string operator ()(std::vector<std::string>& strArgs) const
		{
			// Validate argument count
			if (strArgs.size() < nrOfArgs - nrOfDefaultArgs)
			{
				return "Error: Too few arguments(" + lexical_cast<std::string>(strArgs.size()) +
					"). The minimum is " + lexical_cast<std::string>(nrOfArgs - nrOfDefaultArgs) + ".";
			}
			if (strArgs.size() > nrOfArgs)
			{
				return "Error: Too many arguments(" + lexical_cast<std::string>(strArgs.size()) +
					"). The maximum is " + lexical_cast<std::string>(nrOfArgs)+".";
			}

			TArgs args;
			try
			{
				fill_default_args(std::integral_constant < std::size_t, nrOfDefaultArgs - 1 > {}, args);
				parse(std::integral_constant < std::size_t, nrOfArgs - 1 > {}, args, strArgs);
			}
			catch (bad_argument_conversion e)
			{
				return "Error: Argument(" + lexical_cast<std::string>(e.index) + ") type conversion failed. Expected type " + e.typeName + ".";
			}
			function_caller<traits::return_type> caller;
			return caller(IntegerSequenceGenerator<std::tuple_size<decltype(args)>::value>::type(), args, m_pFunc);
		}

	private:
		// parse function converts N.th string argument to the appropriate type and recursively calls itself with N-1 until N = 0 -> the tuple has been filled
		template<std::size_t N, typename... Ts>
		void parse(std::integral_constant<std::size_t, N>, std::tuple<Ts...>& _args, const std::vector<std::string>& strArgs) const
		{
			using TypeN = std::remove_const<typename std::remove_reference<typename traits::arg<N>::type>::type>::type;

			if (N < strArgs.size())
			{
				try
				{
					std::get<N>(_args) = lexical_cast<TypeN>(strArgs[N]);
				}
				catch (bad_lexical_cast)
				{
					throw bad_argument_conversion(N, typeid(TypeN).name());
				}
			}
			parse(std::integral_constant < std::size_t, N - 1 > {}, _args, strArgs);
		}

		template<typename... Ts>
		void parse(std::integral_constant<std::size_t, 0>, std::tuple<Ts...>& _args, const std::vector<std::string>& strArgs) const
		{
			using TypeN = std::remove_const<typename std::remove_reference<typename traits::arg<0>::type>::type>::type;

			if (strArgs.size())
			{
				try
				{
					std::get<0>(_args) = lexical_cast<TypeN>(strArgs[0]);
				}
				catch (bad_lexical_cast)
				{
					throw bad_argument_conversion(0, typeid(TypeN).name());
				}
			}
		}

		template<typename... Ts>
		void parse(std::integral_constant<std::size_t, -1>, std::tuple<Ts...>& info, const std::vector<std::string>& strArgs) const
		{
			// the function is not expecting any arguments
		}

		template<size_t N, typename... Ts>
		void fill_default_args(std::integral_constant < std::size_t, N >, std::tuple<Ts...>& _args) const
		{
			//std::get<nrOfArgs - N - 1>(_args) = std::get<N>(m_defaultArgs);
			std::get<nrOfArgs - nrOfDefaultArgs + N>(_args) = std::get<N>(m_defaultArgs);
			fill_default_args(std::integral_constant < std::size_t, N - 1 > {}, _args);
		}

		template<typename... Ts>
		void fill_default_args(std::integral_constant < std::size_t, 0 >, std::tuple<Ts...>& _args) const
		{
			//std::get<nrOfArgs - 1>(_args) = std::get<0>(m_defaultArgs);
			std::get<nrOfArgs - nrOfDefaultArgs>(_args) = std::get<0>(m_defaultArgs);
		}

		template<typename... Ts>
		void fill_default_args(std::integral_constant < std::size_t, -1 >, std::tuple<Ts...>& _args) const
		{

		}

		// Need to nest the actual function call inside a struct to allow partial specialization for functions that return void 
		template<typename return_type >
		struct function_caller	// default function_caller
		{
			template<typename _TFunc, std::size_t... N>
			std::string operator()(IntegerSequence<N...>, TArgs _args, _TFunc _pFunc)
			{
				return lexical_cast<std::string>(_pFunc(std::get<N>(_args)...));
			}
		};


		template<>
		struct function_caller < void > // function_caller specialized for void return_type
		{
			template<typename _TFunc, std::size_t... N>
			std::string operator()(IntegerSequence<N...>, TArgs _args, _TFunc _pFunc)
			{
				_pFunc(std::get<N>(_args)...);
				return ""; // "void"
			}
		};

		TFunc						m_pFunc;
		std::tuple<TDefaultArgs...> m_defaultArgs;
	};

	template<typename TFunc, typename... TDefaultArgs>
	auto make_string_functor(TFunc pFunc, TDefaultArgs... defaultArgs) -> basic_string_functor<TFunc, TDefaultArgs...>
	{
		return basic_string_functor<TFunc, TDefaultArgs...>(pFunc, defaultArgs...);
	}

	// faster in most cases but requires compile time function pointer and no polymorphism
	namespace experimental
	{
		template<typename TFunc>
		class string_functor{};

		template<typename R, typename... Args>
		class string_functor<R(*)(Args...)> 
		{
		public:
			using traits = bs::function_traits<R(*)(Args...)>;


			static auto args_from_strings_impl(std::integral_constant<size_t, sizeof...(Args)>, int argc, const char* argv[], std::tuple<Args...>& ret)
			{
				return ret;
			}

			template<size_t N>
			static	auto args_from_strings_impl(std::integral_constant<size_t, N>, int argc, const char* argv[], std::tuple<Args...>& ret)
			{
				using TypeN = std::remove_const<typename std::remove_reference<typename traits::arg<0>::type>::type>::type;
				std::get<N>(ret) = lexical_cast<TypeN>(argv[N]);
				return args_from_strings_impl(std::integral_constant<size_t, N+1>(), argc, argv, ret);
			}

			static auto args_from_strings(int argc, const char* argv[])
			{
				return args_from_strings_impl(std::integral_constant<size_t, 0>(), argc, argv, std::tuple<Args...>());
			}


			template<R(*func)(Args...)>
			static std::string invoke(int argc, const char* argv[])
			{
				return invoke_impl<func>(IntegerSequenceGenerator<sizeof...(Args)>::type(), args_from_strings(argc, argv));

			}

			template<R(*func)(Args...), size_t... N>
			static std::string invoke_impl(IntegerSequence<N...>, const std::tuple<Args...>& args)
			{
				return lexical_cast<std::string>(func(std::get<N>(args)...));
			}
		};


		template<typename TFunc, TFunc func>
		auto make_string_functor()
		{
			return &string_functor<TFunc>::invoke<func>;
		}
	}
}