#pragma once

#include <concepts>

namespace Spock::Common
{
	template<typename Arg, typename Functor, typename Result>
	concept Returns = requires(Arg a, Functor f) { {f(a)}->std::convertible_to<Result>; };

	template<typename Arg, typename ObjectRef, typename Functor, typename Result>
	concept ObjReturns = requires(Arg a, ObjectRef obj, Functor f) { {(obj->*f)(a)}->std::convertible_to<Result>; };

	template<typename T>
	concept NonPointerType =  !std::is_pointer_v<T>;
}