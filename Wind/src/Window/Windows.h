#pragma once

#include "Window.h"

#include <Windows.h>

namespace Core
{
	class Windows : public Window
	{
	public :
		Windows() = delete;
		Windows(const WindowProperties props);
		~Windows();

		bool Init(const WindowProperties props);
		void Broadcast();
		// Inherited via Window
		virtual void OnUpdate() override;
		virtual void ShutDown() override;
		virtual void* GetWindow() override { return m_hwnd; }
		virtual uint32_t GetHeight() override;
		virtual uint32_t GetWidth() override;

		virtual void SetTitle(const std::string title) override;
		virtual void SetIcon(const char* filename) override;
		virtual void* LoadIconFile(const std::string& icon);

		virtual void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventFn = callback; }

		static LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	private :
		HWND m_hwnd;
		Window* m_Instance;

		struct WindowData
		{
			bool vsync;
			uint32_t height;
			uint32_t width;
			std::string_view title;
			bool m_rawInputInit = false;
			EventCallbackFn EventFn;
		};
		WindowData m_Data;

		std::unordered_map<void*, HWND> m_handleMap;
	};
}

