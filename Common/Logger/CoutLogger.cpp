#include "CoutLogger.h"
#include <iostream>

void Spock::Common::CoutLogger::Info(std::string str) {
	std::cout << str << std::endl;
}

void Spock::Common::CoutLogger::Info(std::wstring wstr) {
	std::cout << "CoutLogger DOES NOT SUPPORT WIDE STRING LOGGING" << std::endl;
}

void Spock::Common::CoutLogger::Warn(std::string str) {
	std::cout << str << std::endl;
}

void Spock::Common::CoutLogger::Error(std::string str) {
	std::cout << str << std::endl;
}

void Spock::Common::CoutLogger::Assert(std::string str) {
	std::cout << str << std::endl;
}
