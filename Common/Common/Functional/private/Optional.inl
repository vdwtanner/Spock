//see..\Optional.h" for declaration
#include "Common/SpockException.h"

namespace Spock::Common
{
	template<typename T>
	const Optional<T> Optional<T>::operator=(const Optional<T>& rhs) {
		return Optional<T>(rhs.value, rhs.present);
	}
	template<typename T>
	const Optional<T> Optional<T>::operator=(const T& rhs) {
		return Optional<T>::of(rhs);
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