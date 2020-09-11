#pragma once

#include <memory>

#include "Surface.h"
#include "VulkanInstance.h"

namespace Spock::vkCore
{
	struct WindowParameters
	{
	#ifdef VK_USE_PLATFORM_WIN32_KHR
		HINSTANCE hInstance;
		HWND hWnd;
	#elif defined VK_USE_PLATFORM_XLIB_KHR
		Display* display;
		Window window;
	#elif defined VK_USE_PLATFORM_XCB_KHR
		xcb_connection_t* connection;
		xcb_window_t window;
	#endif 

	};

	class SurfaceFactory
	{
	public:
		virtual std::unique_ptr<Surface> CreateSurface(const WindowParameters windowParameters) = 0;
	};

	class SurfaceFactoryImpl : public SurfaceFactory
	{
	public:
		SurfaceFactoryImpl(VulkanInstance* instance);
		std::unique_ptr<Surface> CreateSurface(const WindowParameters windowParameters) override;
	private:
		const VulkanInstance* instance;

		const VkResult PortedCreateSurface(const WindowParameters windowParameters, VkSurfaceKHR& surface);
	};
}


