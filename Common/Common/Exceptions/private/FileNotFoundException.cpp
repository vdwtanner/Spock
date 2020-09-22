#include "../FileNotFoundException.h"
#include "..\FileNotFoundException.h"

namespace Spock::Common
{
	FileNotFoundException::FileNotFoundException(std::string fileName, int lineNumber, const std::filesystem::path& path, const SpockException* cause) 
	: SpockException(fileName, lineNumber, path.string() + " does not exist", "FileNotFoundException", cause) {
	}

	FileNotFoundException::FileNotFoundException(std::string fileName, int lineNumber, const std::string path, const SpockException* cause) 
	: SpockException(fileName, lineNumber, path + " does not exist", "FileNotFoundException", cause) {
	}
}