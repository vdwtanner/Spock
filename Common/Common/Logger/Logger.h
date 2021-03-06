#pragma once

#ifdef _WIN32
#ifndef __wtypes_h__
#include <wtypes.h>
#endif

#ifndef __WINDEF_
#include <windef.h>
#endif
#endif

#include <string>
#include <memory>
#include "Common/Exceptions/SpockException.h"

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

#ifdef _WIN32
	#define PAUSE() system("pause");
#else
#define PAUSE() system("read");
#endif // _WIN32

#define RELEASE_ASSERT(expr) if(!(expr)) Spock::Common::_release_assert(#expr, __FILE__, __LINE__)
	inline void _release_assert(const std::string expression, const std::string file, int line) {
		fprintf(stderr, "Assertion '%s' failed, file '%s' line '%d'.",
			expression.c_str(), file.c_str(), line);
		abort();
	}

#define LOG_ERROR(msg) Spock::Common::_log_error(msg)
	inline void _log_error(const char* message) {
		LoggerProvider::GetLogger()->Error(message);
	}
	inline void _log_error(const std::string message) {
		LoggerProvider::GetLogger()->Error(message);
	}
	inline void _log_error(const Spock::Common::SpockException exception) {
		LoggerProvider::GetLogger()->Error(exception.GenerateErrorReport());
	}
	inline void _log_error(const std::exception exception) {
		LoggerProvider::GetLogger()->Error(exception.what());
	}

#if defined _DEBUG
#define ASSERT_USAGE(expr, msg) if(!(expr)) Spock::Common::_assertUsage(#expr, msg, __FILE__, __LINE__)
	inline void _assertUsage(const char* expr, const char* msg, const char* file, int line) {
		const char* filename = Spock::Common::StringUtils::GetBaseFileName(file);
		fprintf(stderr, "Assertion '%s' failed at %s:%d:: %s", expr, filename, line, msg);
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
	inline void _log_info(const std::string message) {
		LoggerProvider::GetLogger()->Info(message);
	}

#define LOG_WARN(msg) Spock::Common::_log_warn(msg)
	inline void _log_warn(const char* message) {
		LoggerProvider::GetLogger()->Warn(message);
	}
	inline void _log_warn(const std::string message) {
		LoggerProvider::GetLogger()->Warn(message);
	}
#else
#define LOG_ASSERT(expr)
#define ASSERT_USAGE(expr, msg)
#define LOG_INFO(msg)
#define LOG_WARN(msg)
#endif

#define ABORT(msg) Spock::Common::_abort(msg)
	inline void _abort(const char* msg) {
		LOG_ERROR(msg);
		PAUSE();
		abort();
	}
	inline void _abort(Spock::Common::SpockException exception) {
		LOG_ERROR(exception);
		PAUSE();
		abort();
	}
}