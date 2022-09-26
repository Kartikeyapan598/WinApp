#pragma once

#include <string>

#include "Core.h"

namespace Core
{
	class Application
	{
	public :
		Application() = delete;
		Application(std::string_view title);
		~Application();

		void Init();
		void Close();
		void OnUpdate();
		void Run();
		bool IsRunning() { return m_running; }

		void OnEvent();

		static Application& GetApplication() { return *m_applicationInstance; }
	private :
		bool m_running = true;
		static inline Application* m_applicationInstance = NULL;
	};
}
