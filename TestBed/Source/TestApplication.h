#pragma once

#include <memory>

#include "TestRenderer.h"

struct GLFWwindow;
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
		GLFWwindow* window;
		std::unique_ptr<TestRenderer> renderer;

		const void InitLogger();
		const void InitWindow();
		const void InitRenderer();
	};
}
