#pragma once
#include <tuple>
#include <functional>

namespace bs
{
	template<typename T>
	struct function_traits;

	template<typename R, typename... Args>
	struct function_traits<R(*)(Args...)> : public function_traits<R(Args...)>
	{};


	template<typename R, typename... Args>
	struct function_traits<R(__stdcall *)(Args...)> : public function_traits<R(Args...)>
	{};

	template<typename R, typename... Args>
	struct function_traits<R(__thiscall *)(Args...)> : public function_traits<R(Args...)>
	{};

	template<typename TClass, typename R, typename... Args>
	struct function_traits<R(__thiscall TClass::*)(Args...)> : public function_traits<R(TClass&, Args...)>
	{};

	/*template<typename TClass, typename R, typename... Args>
	struct function_traits<std::_Pmf_wrap<R(__thiscall TClass::*)(Args...), R, TClass, Args...>> : public function_traits<R(Args...)>
	{};*/

	template<typename R, typename... Args>
	struct function_traits<std::function<R(Args...)>> : public function_traits<R(Args...)>
	{};

	template<typename R, typename ...Args>
	struct function_traits<R(Args...)>
	{
		static const size_t arity = sizeof...(Args);

		using return_type = R;
		using tuple_arguments_type = std::tuple<Args...>;

		template<size_t N>
		using arg = typename std::tuple_element<N, tuple_arguments_type>;
		/*template<size_t N>
		struct arg
		{
			static_assert(N < arity, "error: invalid parameter index.");
			using type = typename std::tuple_element<N, tuple_arguments_type>::type;
		};*/
	};
}