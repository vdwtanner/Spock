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
		static Optional<T> ofNullable(const T* pValue) requires NonPointerType<T> {
			return pValue == nullptr ? Optional<T>() : Optional<T>(*pValue);
		}

		Optional<T>(const Optional<T>& other) = default;
		Optional<T>(Optional<T>&& other) = default;
		const void operator=(const Optional<T>& rhs);
		const void operator=(const T& rhs);
		Optional<T>& operator=(Optional<T>&& rhs) = default;
		auto operator<=>(const Optional<T>& rhs) const { 
			if (!present && !rhs.present) { return std::strong_ordering::equal; }
			if (present && !rhs.present) { return std::strong_ordering::greater; }
			if (!present && rhs.present) { return std::strong_ordering::less; }
			return value <=> rhs.value;
		}
		bool operator==(const Optional<T>& rhs) const { 
			if (!present && !rhs.present) { return true; }
			if (present ^ rhs.present) { return false; }
			return value == rhs.value;
		}
		auto operator<=>(const T& rhs) const {
			if (!present) { return std::strong_ordering::less; }
			return value <=> rhs;
		}
		bool operator==(const T& rhs) const {
			if (!present) { return false; }
			return value == rhs;
		}

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
		T value;
		bool present;

		Optional<T>() : present(false), value() {}
		Optional<T>(T value) : value(value), present(true) {}
		Optional<T>(T value, bool present) : value(value), present(present) {}
	};
}

//include the definition so that the templates are instantiated and linked properly
#include "private/Optional.inl"