#pragma once

#include <memory>

#include "GlfwHelper.h"

#include "GlfwRenderer.h"
#include "Common/Logger/Logger.h"

namespace Spock::Testbed
{
	class TestApplication
	{
	public:
		~TestApplication();
		const void Init();
		const void Run();
	private:
		const int WINDOW_HEIGHT = 600;
		const int WINDOW_WIDTH = 800;
		std::unique_ptr<GLFWwindow, DestroyGlfwWindow> window;
		std::unique_ptr<GlfwRenderer> renderer;

		const void InitLogger();
		const void InitWindow();
		const void InitRenderer();
	};
}
