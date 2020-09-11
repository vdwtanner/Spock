#pragma once

#include <functional>

#include "Concepts.h"

namespace Spock::Common
{
	template<typename T>
	class Optional
	{
	public:
		static Optional<T> empty() requires NonPointerType<T> {
			return Optional<T>();
		}
		static Optional<T> of(T value) requires NonPointerType<T> {
			return Optional<T>(value);
		}
		static Optional<T> ofNullable(T* pValue) requires NonPointerType<T> {
			return pValue == nullptr ? Optional<T>() : Optional<T>(*pValue);
		}

		Optional<T>(const Optional<T>& other) = default;
		Optional<T>(Optional<T>&& other) = default;
		const Optional<T> operator=(const Optional<T>& rhs);
		const Optional<T> operator=(const T& rhs);
		Optional<T>& operator=(Optional<T>&& rhs) = default;

		const bool IsPresent() const;
		const bool IsEmpty() const;
		const T GetOr(T other) const;
		const T Get() const;

		template<typename Functor>
		const void Apply(Functor function) const;

		template<typename ObjectRef, typename Functor>
		const void Apply(ObjectRef ref, Functor function) const;

		template<typename Result, typename Functor>
		requires Returns<T, Functor, Result>
			const Optional<Result> Map(Functor function) const;

		template<typename Result, typename ObjectRef, typename Functor>
		requires ObjReturns<T, ObjectRef, Functor, Result>
			const Optional<Result> Map(ObjectRef ref, Functor function) const;
	private:
		const T value;
		const bool present;

		Optional<T>() : present(false), value() {}
		Optional<T>(T value) : value(value), present(true) {}
		Optional<T>(T value, bool present) : value(value), present(present) {}
	};
}

//include the definition so that the templates are instantiated and linked properly
#include "private/Optional.inl"