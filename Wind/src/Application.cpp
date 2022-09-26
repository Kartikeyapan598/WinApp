#include "Application.h"
#include "Window/WindowContainer.h"

namespace Core
{
	Application::Application(std::string_view title)
	{
		if (m_applicationInstance != NULL)
		{
			return;
		}
		m_applicationInstance = this;
		{
			WindowContainer* container = WindowContainer::GetContainer();
			auto window = Window::Create(title);
			//window->SetEventCallback(BINDFN(Application::OnEvent));
			container->insert(window.get(), window->GetWindow());

		}
		Init();
	}

	Application::~Application()
	{
		m_applicationInstance = NULL;
		delete m_applicationInstance;
	}

	void Application::Init()
	{
		m_running = true;
	}

	void Application::Close()
	{
		m_running = false;
	}

	void Application::OnUpdate()
	{
	}

	void Application::OnEvent()
	{
		Close();
	}

	void Application::Run()
	{
		WindowContainer* container = WindowContainer::GetContainer();
		while (m_running)
		{
			OnUpdate();
			container->Broadcast();
		}
	}
}