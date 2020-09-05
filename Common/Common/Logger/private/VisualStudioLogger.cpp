#include "Common/Logger/VisualStudioLogger.h"
#include "Common/StringUtils.h"

#if defined _WIN32
#include <debugapi.h>

namespace Spock::Common
{
	void VisualStudioLogger::Info(std::string str) {
		OutputDebugString(StringUtils::StringToWideString("[INFO] " + str + "\n").c_str());
	}

	void VisualStudioLogger::Info(std::wstring wstr) {
		OutputDebugString((L"[INFO] " + wstr + L"\n").c_str());
	}

	void VisualStudioLogger::Warn(std::string str) {
		OutputDebugString(StringUtils::StringToWideString("[WARN] " + str + "\n").c_str());
	}

	void VisualStudioLogger::Error(std::string str) {
		OutputDebugString(StringUtils::StringToWideString("[ERROR] " + str + "\n").c_str());
	}

	void VisualStudioLogger::Assert(std::string str) {
		OutputDebugString(StringUtils::StringToWideString("[ASSERT] " + str + "\n").c_str());
	}
}
#else 
namespace Spock::Common
{
	void VisualStudioLogger::Info(std::string str) {
		cout << str;
	}

	void VisualStudioLogger::Info(std::wstring wstr) {
		cout << str;
	}

	void VisualStudioLogger::Warn(std::string str) {
		cout << str;
	}

	void VisualStudioLogger::Error(std::string str) {
		cout << str;
	}

	void VisualStudioLogger::Assert(std::string str) {
		cout << str;
	}
}
#endif