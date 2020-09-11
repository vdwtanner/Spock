#include "../TestApplication.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
#include <exception>

#include "Common/SpockException.h"
#include "Common/Logger/LoggerChain.h"
#include "Common/Logger/VisualStudioLogger.h"
#include "Common/Logger/CoutLogger.h"

namespace Spock::Testbed
{
    using namespace Common;
    using namespace vkCore;

    TestApplication::~TestApplication() {
        renderer.reset();
        window.reset();
        glfwTerminate();
    }

    const void TestApplication::Init() {
        InitLogger();
        try {
            InitWindow();
            InitRenderer();
        } catch (SpockException& e) {
            LOG_ERROR(e);
            exit(1);
        }
        
    }

    const void TestApplication::InitLogger() {
        auto logger = std::make_unique<LoggerChain>();
        logger->AddLogger(std::make_unique<VisualStudioLogger>());
        logger->AddLogger(std::make_unique<CoutLogger>());
        LoggerProvider::Initialize(std::move(logger));
    }

    const void TestApplication::InitWindow() {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        window = std::unique_ptr<GLFWwindow, DestroyGlfwWindow>(glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr));
    }

    const void TestApplication::InitRenderer() {
        auto loader = std::make_shared<vkCore::LoaderImpl>();
        renderer = std::make_unique<GlfwRenderer>(loader, window.get());
        renderer->Init();
    }

    const void TestApplication::Run() {
        try {
            while (!glfwWindowShouldClose(window.get())) {
                glfwPollEvents();
            }
        } catch (SpockException e) {
            LOG_ERROR(e);
        }
    }
}

