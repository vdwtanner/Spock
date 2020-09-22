#pragma once

#include <string>
#include <filesystem>
#include "SpockException.h"

namespace Spock::Common
{
	class FileNotFoundException : public SpockException
	{
	public:
		FileNotFoundException(std::string fileName, int lineNumber, const std::filesystem::path& path, const SpockException* cause = nullptr);
		FileNotFoundException(std::string fileName, int lineNumber, const std::string path, const SpockException* cause = nullptr);
	};
}