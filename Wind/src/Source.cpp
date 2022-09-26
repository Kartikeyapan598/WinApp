#include "Wind.h"
#include "Render.h"
#include "Application.h"

int main()
{
	Core::Application* app = new Core::Application("WinApp");

	//Init();
	//RenderInit(m_Render_Width, m_Render_Height,m_Rhwnd);
	
	app->Run();

	delete app;
	// Clear Resources
	//ClearResources();
	return 0;
}