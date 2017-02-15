#pragma once
#include <functional> // function traits
#include <tuple>
#include <sys/stat.h> // file exists

/*
 * Random utilities
 */
 namespace bs
 {
	 inline bool file_exists(const std::string& filePath)
	 {
		struct stat buffer;
		return (stat(filePath.c_str(), &buffer) == 0);
	 }

	// unpacking types packed in a tuple
	template<template<typename...> class F, typename T>
	struct unwrap_into;

	template<template<typename...> class F, typename... Ts>
	struct unwrap_into<F, std::tuple<Ts...>>
	{
		typedef F<Ts...> type;
	};

	template<typename R, typename ... Ts>
	struct unwrap_into_stdfunction;

	template<typename R, typename... Ts>
	struct unwrap_into_stdfunction<R, std::tuple<Ts...>>
	{
		typedef std::function<R(Ts...)> type;
	};

	template<size_t ... S> struct IntegerSequence {};

	template<size_t  N, size_t ...S> struct IntegerSequenceGenerator : IntegerSequenceGenerator<N - 1, N - 1, S...> {};

	template<size_t ...S> struct IntegerSequenceGenerator<0, S...>{ using type = IntegerSequence<S...>; };

	// converts lambda to std::function
	template<typename TCallable, typename R, typename... Args>
	auto lambda(TCallable& _callable, R(TCallable::*mf)(Args...) const)
	{
		std::function<R(Args...)> retVal = _callable;
		return retVal;
	}

	template<typename TCallable>
	auto lambda(TCallable _callable)
	{
		return lambda(_callable, &TCallable::operator());
	}
	
	// removes reference from each type in tuple
	template <typename... T>
	struct tuple_refrence_remover
	{
		using type = std::tuple < typename std::remove_reference<T>::type... >;
	};

	template <typename... T>
	struct tuple_refrence_remover< std::tuple<T...> > : public tuple_refrence_remover<T...>
	{};

	// removes const from each type in tuple
	template <typename... T>
	struct tuple_const_remover
	{
		using type = std::tuple < typename std::remove_const<T>::type... >;
	};

	template <typename... T>
	struct tuple_const_remover< std::tuple<T...> > : public tuple_const_remover<T...>
	{};
	
	// get tuple element index by type
	template <class T, class Tuple>
	struct tuple_index;

	template <class T, class... Types>
	struct tuple_index<T, std::tuple<T, Types...>> {
		static const std::size_t value = 0;
	};

	template <class T, class U, class... Types>
	struct tuple_index<T, std::tuple<U, Types...>> {
		static const std::size_t value = 1 + tuple_index<T, std::tuple<Types...>>::value;
	};

	template <class... Args>
	struct type_list
	{
		// total number of types
		const static size_t count = sizeof...(Args);

		// get the type at index N
		template <std::size_t N>
		using type = typename std::tuple_element<N, std::tuple<Args...>>::type;

		// get index of type T
		template<typename T>
		using index = tuple_index<T, std::tuple<Args...>>;
	};
	
	// Append a type to a type_list
	template<typename TL, typename T> struct append_type {};

	template<typename T, typename... Args>
	struct append_type<type_list<Args...>, T>
	{
		using type = type_list<Args..., T>;
	};
}