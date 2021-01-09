#pragma once
#include <Windows.h>

class __declspec(dllexport) WindowC
{
public :
	WindowC() {}
	int Run();
	void OnUpdate();
	bool InitWindowsApp();
	//static WindowC getWindow() { return ; }

private :
	HWND m_hwnd;
};