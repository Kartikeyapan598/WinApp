#include "WindowContainer.h"

namespace Core
{
	WindowContainer::WindowContainer()
		:m_hInstance(GetModuleHandle(0)), m_windowClass("Cndr Enginer Window Class")
	{
		m_container = this;
	}

	WindowContainer* WindowContainer::GetContainer()
	{
		if (m_container == NULL)
		{
			return new WindowContainer();
		}
		else return m_container;
	}

	LRESULT CALLBACK WindowContainer::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT result = NULL;
		Window* window = WindowContainer::GetContainer()->GetWindowFromHandle((void*)hwnd);
		if (window == nullptr)
		{
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}

		switch (msg)
		{
		case WM_CREATE:
			break;
		case WM_CLOSE:				// Windows is destroyed and only make any necessary cleanup
		case WM_DESTROY:			// Windows is not started to destroy yet
		case VK_F4:
		{
			DestroyWindow(hwnd);
			break;
		}
		case WM_MOVE :
			break;
		case WM_SIZE :
			//WindowData* data = (WindowData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			//data->width = (UINT)LOWORD(lParam);
			//data->height = (UINT)HIWORD(lParam);
			//
			//WindowResizeEvent e(data->width, data->height);
			//data->EventFn(e);
			break;
		case WM_DPICHANGED:
			break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		return result;
	}

	void WindowContainer::insert(Window* window, void* hwnd)
	{
		m_handleMap[hwnd] = window;
	}

	void WindowContainer::erase(void* handle)
	{
		if (m_handleMap.find(handle) == m_handleMap.end())
		{
			std::cout << "Window Not Found in HandleMap\n";
			return;
		}
		else
		{
			m_handleMap.erase(handle);
		}
	}

	Window* WindowContainer::GetWindowFromHandle(void* handle)
	{
		if (m_handleMap.find(handle) == m_handleMap.end())
		{
			return NULL;
		}
		return m_handleMap[handle];
	}

	void WindowContainer::Broadcast()
	{
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}