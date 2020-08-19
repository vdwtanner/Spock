#pragma once

#include <string>
#include <memory>
#include "Preconditions.h"
#include "StringUtils.h"

namespace Spock::Common
{
	class SpockException : public std::exception
	{
	public:
		SpockException(std::string fileName, int lineNumber, std::string what, const SpockException* cause=nullptr);
		SpockException(const SpockException& other);
		~SpockException() = default;

		std::string GenerateErrorReport() const;
	private:
		const std::string fileName;
		const int lineNumber;
		const SpockException* cause;

		virtual std::string InternalGenerateErrorReport() const;
	};

#define THROW_EXCEPTION(type, ...) DERIVED_FROM(type, SpockException); \
	throw Spock::Common::_mk_exception<type>(Spock::Common::StringUtils::GetBaseFileName(__FILE__), ## __LINE__, ## __VA_ARGS__);

	//This should not be called directly. Use THROW_EXCEPTION macro instead
	template <class T, class... Args>
	T _mk_exception(Args&&... args) {
		return T(std::forward<Args>(args)...);
	}
}