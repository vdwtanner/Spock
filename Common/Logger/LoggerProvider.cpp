#include "Logger/Logger.h"

namespace Spock::Common
{
	LoggerProvider* LoggerProvider::instance;

	std::unique_ptr<Logger>& LoggerProvider::GetLogger() {
		ASSERT_USAGE(instance != nullptr, "LoggerProvider must be initialized before calling GetLogger().");
		//#pragma warning(disable:6011) //We abort if instance is null in debug mode, but we don't make the check in release mode
		return LoggerProvider::instance->logger;
		//#pragma warning(default:6011)
	}

	void LoggerProvider::Initialize(std::unique_ptr<Logger> logger) {
		ASSERT_USAGE(instance == nullptr, "LoggerProvider already initialized.");
		LoggerProvider::instance = new LoggerProvider(std::move(logger));
	}

	void LoggerProvider::SetLogger(std::unique_ptr<Logger> logger) {
		ASSERT_USAGE(logger != nullptr, "Cannot call SetLogger with a nullptr.");
		LoggerProvider::instance->logger = std::move(logger);
	}

	LoggerProvider::LoggerProvider(std::unique_ptr<Logger>&& log) {
		ASSERT_USAGE(log != nullptr, "Cannot initialize LoggerProvider with a null Logger.");
		logger = std::move(log);
	}
}