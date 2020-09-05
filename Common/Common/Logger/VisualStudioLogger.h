#pragma once

#include "Logger.h"

namespace Spock::Common
{
	class VisualStudioLogger : public Logger
	{
	public:
		// Inherited via Logger
		virtual void Info(std::string str) override;
		virtual void Info(std::wstring wstr) override;
		virtual void Warn(std::string str) override;
		virtual void Error(std::string str) override;
		virtual void Assert(std::string str) override;
	};
}
