#pragma once
#include "Logger/Logger.h"
#include <vector>

namespace Spock::Common
{
    class LoggerChain : public Logger
    {
    public:
        LoggerChain();
        ~LoggerChain();
        void AddLogger(std::unique_ptr<Logger> logger);

        // Inherited via Logger
        virtual void Info(std::string str) override;
        virtual void Info(std::wstring wstr) override;
        virtual void Warn(std::string str) override;
        virtual void Error(std::string str) override;
        virtual void Assert(std::string str) override;
    private:
        std::vector<std::unique_ptr<Logger>> loggers;
    };
}

