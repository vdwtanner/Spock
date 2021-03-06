#include "Common/Exceptions/SpockException.h"
#include <sstream>

namespace Spock::Common
{
	SpockException::SpockException(std::string fileName, int lineNumber, std::string what, const SpockException* cause) :
		SpockException(fileName, lineNumber, what, "SpockException", cause) {
	}

	SpockException::SpockException(const SpockException& other) 
		: SpockException(other.fileName, other.lineNumber, other.what(), other.name, other.cause) {
	}

	SpockException::SpockException(std::string fileName, int lineNumber, std::string what, std::string name, const SpockException* cause) :
		std::exception(what.c_str()), name(name), fileName(fileName), lineNumber(lineNumber), cause(cause) {
	}

	std::string SpockException::InternalGenerateErrorReport() const {
		auto ss = std::stringstream();
		ss << fileName << ":" << lineNumber << " - " << name << ":: " << what();
		return ss.str();
	}

	std::string SpockException::GenerateErrorReport() const {
		std::stringstream ss = std::stringstream();
		const SpockException* current = this;
		int level = 0;
		while (current != nullptr) {
			ss << std::string(level++, '\t') << current->InternalGenerateErrorReport() << std::endl;
			current = current->cause;
		}
		return ss.str();
	}


}
