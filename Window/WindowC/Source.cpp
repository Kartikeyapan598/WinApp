#include <windows.h>

HWND ghMainWnd = 0;

bool InitWindowsApp();//HINSTANCE instanceHandle, int show);

int Run();

void OnUpdate()
{

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//int /*WINAPI*/ main(int argv, char** argc)//HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd)
//{
//
//	if (!InitWindowsApp())//hInstance, nShowCmd))
//		return 0;
//
//	return Run();
//}
bool InitWindowsApp()//HINSTANCE instanceHandle, int show)
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = NULL;//instanceHandle;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = CreateSolidBrush(RGB(80, 0, 80));
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"BasicWndClass";

	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass FAILED", 0, 0);
		return false;
	}

	ghMainWnd = CreateWindow(
		L"BasicWndClass", // Registered WNDCLASS //instance to use.
		L"Edtr", // window title
		WS_OVERLAPPEDWINDOW, // style flags
		CW_USEDEFAULT, // x-coordinate
		CW_USEDEFAULT, // y-coordinate
		1280, // width
		768, // height
		0, // parent window
		0, // menu handle
		NULL,//instanceHandle, // app instance
		0); // extra creation parameters
	if (ghMainWnd == 0)
	{
		MessageBox(0, L"CreateWindow FAILED", 0, 0);
		return false;
	}

	ShowWindow(ghMainWnd, SW_SHOW);
	UpdateWindow(ghMainWnd);
	return true;
}
int Run()
{
	MSG msg = { 0 };

	BOOL bRet = 1;
	while ((bRet = GetMessage(&msg, 0, 0, 0)) != 0)
	{
		if (bRet == -1)
		{
			MessageBox(0, L"GetMessage FAILED", L"Error", MB_OK);
			break;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	OnUpdate();

	return (int)msg.wParam;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	switch (msg)
	{

	case WM_LBUTTONDOWN:
		MessageBox(0, L"Edtr", L"Cndr", MB_OK);
		return 0;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			DestroyWindow(ghMainWnd);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}