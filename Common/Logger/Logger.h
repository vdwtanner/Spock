#pragma once
#include <string>
#include <memory>
#include "SpockException.h"

namespace Spock::Common
{
	class Logger
	{
	public:
		virtual void Info(std::string str) = 0;
		virtual void Info(std::wstring wstr) = 0;
		virtual void Warn(std::string str) = 0;
		virtual void Error(std::string str) = 0;
		virtual void Assert(std::string str) = 0;
	};

	class LoggerProvider
	{
	public:
		static std::unique_ptr<Logger>& GetLogger();
		static void Initialize(std::unique_ptr<Logger> logger);
		static void SetLogger(std::unique_ptr<Logger> logger);
	private:
		static LoggerProvider* instance;

		LoggerProvider(std::unique_ptr<Logger>&& logger);
		std::unique_ptr<Logger> logger;
	};

#define RELEASE_ASSERT(expr) if(!(expr)) Spock::Common::_release_assert(#expr, __FILE__, __LINE__)
	inline void _release_assert(const std::string expression, const std::string file, int line) {
		fprintf(stderr, "Assertion '%s' failed, file '%s' line '%d'.",
			expression.c_str(), file.c_str(), line);
		abort();
	}

#if defined _DEBUG
#define ASSERT_USAGE(expr, msg) if(!(expr)) Spock::Common::_assertUsage(#expr, msg)
	inline void _assertUsage(const std::string expr, const std::string msg) {
		fprintf(stderr, "Assertion '%s' failed: %s", expr.c_str(), msg.c_str());
		abort();
	}

#define LOG_ASSERT(expr) if(!(expr)) Spock::Common::_log_assert(#expr, __FILE__, __LINE__)
	inline void _log_assert(const std::string expression, const std::string file, int line) {
		std::string message = "Assertion '" + expression +
			"' failed, file '" + file +
			"' line '" + std::to_string(line) + "'.";

		LoggerProvider::GetLogger()->Assert(message);
		_release_assert(expression, file, line);
	}

#define LOG_INFO(msg) Spock::Common::_log_info(msg)
	inline void _log_info(const char* message) {
		LoggerProvider::GetLogger()->Info(message);
	}

#define LOG_WARN(msg) Spock::Common::_log_warn(msg)
	inline void _log_warn(const char* message) {
		LoggerProvider::GetLogger()->Warn(message);
	}

#define LOG_ERROR(msg) Spock::Common::_log_error(msg)
	inline void _log_error(const char* message) {
		LoggerProvider::GetLogger()->Error(message);
	}
	inline void _log_error(const Spock::Common::SpockException exception) {
		LoggerProvider::GetLogger()->Error(exception.GenerateErrorReport());
	}
	inline void _log_error(const std::exception exception) {
		LoggerProvider::GetLogger()->Error(exception.what());
	}
#else
#define LOG_ASSERT(expr)
#define ASSERT_USAGE(expr, msg)
#define LOG_INFO(msg)
#define LOG_WARN(msg)
#define LOG_ERROR(msg)
#endif
}