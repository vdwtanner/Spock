#pragma once
#ifndef __wtypes_h__
#include <wtypes.h>
#endif

#ifndef __WINDEF_
#include <windef.h>
#endif

#include <string>

namespace Spock::Common
{
	class StringUtils
	{
	public:
		static std::wstring StringToWideString(const std::string);

		static constexpr const char* GetBaseFileName(const char* filepath) {
			const char* file = filepath;
			while (*filepath) {
				char c = *filepath++;
				if (c == '/' || c=='\\') {
					file = filepath;
				}
			}
			return file;
		}
	};
}

namespace notstd
{
	namespace adl_helper
	{
		using std::to_string;

		template<class T>
		std::string as_string(T&& t) {
			return to_string(std::forward<T>(t));
		}
	}
	template<class T>
	std::string to_string(T&& t) {
		return adl_helper::as_string(std::forward<T>(t));
	}
}