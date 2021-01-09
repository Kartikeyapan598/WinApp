#pragma once
#include "WindowC/WindowC.h"
int main(int argc,char **argv)
{
	WindowC* m_Window = new WindowC();

	if (m_Window->InitWindowsApp())
		return 0;
	return m_Window->Run();
}