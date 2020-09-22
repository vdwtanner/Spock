#pragma once

#include <vector>
#include <filesystem>

namespace Spock::Common {

	class FileUtils
	{
	public:
		static std::vector<char> ReadFile(const std::filesystem::path& filepath);
	private:
	};
}