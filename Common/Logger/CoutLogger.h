#pragma once

#include "Logger/Logger.h"

namespace Spock::Common
{
	class CoutLogger : public Logger
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