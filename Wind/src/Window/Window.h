#pragma once

#include <string>
#include <iostream>
#include <functional>

namespace Core
{
	struct WindowProperties
	{
	public :
		uint32_t width;
		uint32_t height;
		std::string title;
		bool Vsync;

		WindowProperties(std::string_view Title = "Cndr Engine", uint32_t Width = 1280, uint32_t Height = 720, bool VSync = false)
			:title(Title), width(Width), height(Height), Vsync(VSync)
		{}
	};

	class Window
	{
	public :

		using EventCallbackFn = std::function<void()>;

		virtual ~Window() {}
		virtual void OnUpdate() = 0;
		virtual void ShutDown() = 0;
		virtual void* GetWindow() = 0;
		virtual uint32_t GetHeight() = 0;
		virtual uint32_t GetWidth() = 0;

		virtual void SetTitle(const std::string title) = 0;
		virtual void SetIcon(const char* filename) = 0;
		virtual void* LoadIconFile(const std::string& icon) = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

		static std::unique_ptr<Window> Create(const WindowProperties& prop = WindowProperties());
	};
}