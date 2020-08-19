#include "LoggerChain.h"
#include <cstdarg>

namespace Spock::Common
{
	LoggerChain::LoggerChain() {
		loggers = std::vector<std::unique_ptr<Logger>>();
	}

	LoggerChain::~LoggerChain() {
		for (auto& logger : loggers) {
			logger.release();
		}
	}

	void LoggerChain::AddLogger(std::unique_ptr<Logger> logger) {
		loggers.push_back(std::move(logger));
	}

	void LoggerChain::Info(std::string str) {
		for (const auto& logger : loggers) {
			logger->Info(str);
		}
	}
	void LoggerChain::Info(std::wstring wstr) {
		for (const auto& logger : loggers) {
			logger->Info(wstr);
		}
	}
	void LoggerChain::Warn(std::string str) {
		for (const auto& logger : loggers) {
			logger->Warn(str);
		}
	}
	void LoggerChain::Error(std::string str) {
		for (const auto& logger : loggers) {
			logger->Error(str);
		}
	}
	void LoggerChain::Assert(std::string str) {
		for (const auto& logger : loggers) {
			logger->Error(str);
		}
	}
}