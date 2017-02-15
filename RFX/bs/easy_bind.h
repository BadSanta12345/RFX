#pragma once

/*
 * Defines a bind function similar to std::bind but without the need or ability to use std::placeholders.
 */

namespace bs
{
	template <typename C, typename R, typename... Args>
	std::function<R(Args...)> easy_bind(R(C::*mf)(Args...), C* obj)
	{
		return [=](Args... args) { return (obj->*mf)(args...); };
	}

	template <typename C, typename R, typename... Args>
	std::function<R(Args...)> easy_bind(R(C::*mf)(Args...) const, C* obj)
	{
		return [=](Args... args) { return (obj->*mf)(args...); };
	}

	/*template <typename C, typename R, typename... Args>
	std::function<R(Args...)> easy_bind(R(C::*mf)(Args...), C& obj)
	{
		return [=](Args... args) { return (obj.*mf)(args...); };
	}

	template <typename C, typename R, typename... Args>
	std::function<R(Args...)> easy_bind(R(C::*mf)(Args...) const, C& obj)
	{
		return [=](Args... args) { return (obj.*mf)(args...); };
	}*/
}