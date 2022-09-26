#include <memory>

#include <Windows.h>

#include "Window.h"
#include "Windows.h"

namespace Core
{
	std::unique_ptr<Window> Window::Create(const WindowProperties& prop)
	{
#ifdef _WIN32
		return std::make_unique<Windows>(prop);
#endif // FLAMEW

	}
}