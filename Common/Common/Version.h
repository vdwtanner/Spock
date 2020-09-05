#pragma once
#include <compare>
#include <sstream>

#include "StringUtils.h"

namespace Spock::Common
{
	struct Version
	{
		friend std::string to_string(const Version& self) {
			std::stringstream ss;
			ss << "Version(" << self.major << ", " << self.minor << ", " << self.patch << ")";
			return ss.str();
		}

	public:
		const int major;
		const int minor;
		const int patch;

		Version(int major, int minor, int patch) : major(major), minor(minor), patch(patch) {}

		auto operator<=>(const Version& other) const {
			if (major != other.major) {
				return major <=> other.major;
			} else if (minor != other.minor) {
				return minor <=> other.minor;
			} else {
				return patch <=> other.patch;
			}
		}
		bool operator==(const Version& other) const {
			return major == other.major &&
				minor == other.minor &&
				patch == other.patch;
		}
	};
}