// TestBed.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <exception>
#include "SpockException.h"
#include "Logger/LoggerChain.h"
#include "Logger/VisualStudioLogger.h"
#include "Logger/CoutLogger.h"
#include "vkCore/Loader.h"

using namespace Spock;
using namespace Common;

void BadThings() {
    try {
        THROW_EXCEPTION(SpockException, "test");
    } catch (const SpockException& e) {
        THROW_EXCEPTION(SpockException, "Wrapper", new SpockException(e));
    }
}

int main()
{
    auto logger = std::make_unique<LoggerChain>();
    logger->AddLogger(std::make_unique<VisualStudioLogger>());
    logger->AddLogger(std::make_unique<CoutLogger>());
    LoggerProvider::Initialize(std::move(logger));
    vkCore::Loader loader = vkCore::Loader();
    
    try {
        loader.LoadVulkanLibrary();
        BadThings();
    } catch (const Common::SpockException& e) {
        LOG_ERROR(e);
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
