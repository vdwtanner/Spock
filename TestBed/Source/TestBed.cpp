// TestBed.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <exception>
#include "Common/SpockException.h"
#include "Common/Logger/LoggerChain.h"
#include "Common/Logger/VisualStudioLogger.h"
#include "Common/Logger/CoutLogger.h"
#include "vkCore/Loader.h"
#include "vkCore/VulkanInstance.h"
#include "vkCore/VulkanInstanceFactory.h"

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
    auto loader = std::make_shared<vkCore::LoaderImpl>();

    auto instanceFactory = vkCore::VulkanInstanceFactoryImpl(loader);

    try {
        loader->LoadVulkanLibrary();
        loader->LoadExportedFunctions();
        loader->LoadGlobalFunctions();
        loader->DiscoverAvailableExtensions();

        auto instance = instanceFactory.CreateVulkanInstance("TestBed", Version(0, 0, 1));
        
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
