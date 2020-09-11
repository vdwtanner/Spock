#pragma once

#include "vkCore/Loader.h" 

#ifdef VK_USE_PLATFORM_WIN32_KHR
#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_EXPOSE_NATIVE_WIN32
#endif

#include <GLFW/glfw3.h>
#include "GLFW/glfw3native.h"

namespace Spock::Testbed
{
	struct DestroyGlfwWindow
	{
		void operator()(GLFWwindow* window) {
			if (window != nullptr) {
				glfwDestroyWindow(window);
			}
		}
	};
}