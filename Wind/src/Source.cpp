
#include "Wind.h"
#include "Render.h"
int main()
{
	Init();
	RenderInit(m_Render_Width, m_Render_Height,m_Rhwnd);
	while(m_Running)
	{
		broadcast();
	}

	// Clear Resources
	ClearResources();
	return 0;
}