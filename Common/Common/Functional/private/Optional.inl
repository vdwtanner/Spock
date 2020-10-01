//see..\Optional.h" for declaration
#include "Common/Exceptions/SpockException.h"
#include "..\Optional.h"

namespace Spock::Common
{
	template<typename T>
	const void Optional<T>::operator=(const Optional<T>& rhs) {
		value = rhs.value;
		present = rhs.present;
	}
	template<typename T>
	const void Optional<T>::operator=(const T& rhs) {
		value = rhs;
		present = true;
	}

	template<typename T>
	const bool Optional<T>::IsPresent() const {
		return present;
	}

	template<typename T>
	const bool Optional<T>::IsEmpty() const {
		return !present;
	}

	template<typename T>
	const T Optional<T>::GetOr(T other) const {
		return present ? value : other;
	}

	template<typename T>
	const T Optional<T>::Get() const {
		if (!present) {
			THROW_EXCEPTION(SpockException, "Get() called when no value present!");
		}
		return value;
	}

	template<typename T>
	inline const T& Optional<T>::GetRefOr(T& other) const {
		return present ? value : other;
	}

	template<typename T>
	inline const T& Optional<T>::GetRef() const {
		if (!present) {
			THROW_EXCEPTION(SpockException, "Get() called when no value present!");
		}
		return value;
	}

	template<typename T>
	template<typename Functor>
	const void Optional<T>::Apply(Functor function) const {
		if (present) {
			function(value);
		}
	}

	template<typename T>
	template<typename ObjectRef, typename Functor>
	const void Optional<T>::Apply(ObjectRef ref, Functor function) const {
		if (present) {
			(ref->*function)(value);
		}
	}

	template<typename T>
	template<typename Functor>
	inline const void Optional<T>::IfEmpty(Functor function) const {
		if (!present) {
			function();
		}
	}

	template<typename T>
	template<typename ObjectRef, typename Functor>
	inline const void Optional<T>::IfEmpty(ObjectRef ref, Functor function) const {
		if (!present) {
			(ref->*function)();
		}
	}

	template<typename T>
	template<typename Result, typename Functor>
	requires Returns<T, Functor, Result>
	const Optional<Result> Optional<T>::Map(Functor function) const {
		if (present) {
			return Optional<Result>::of(function(value));
		}
		return Optional<Result>::empty();
	}

	template<typename T>
	template<typename Result, typename ObjectRef, typename Functor>
	requires ObjReturns<T, ObjectRef, Functor, Result>
	const Optional<Result> Optional<T>::Map(ObjectRef ref, Functor function) const {
		if (present) {
			return Optional<Result>::of((ref->*function)(value));
		}
		return Optional<Result>::empty();
	}
}