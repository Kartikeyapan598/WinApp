
#pragma once
#include <iostream>
#include <Windows.h>

// Child Window
#define ID_FIRSTCHILD  100
//

#define IDM_FILE_NEW 1
#define IDM_FILE_OPEN 2
#define IDM_FILE_QUIT 3
#define IDM_FILE_ABOUT 4

#define IDM_WINDOW_RENDER_START 5
#define IDM_WINDOW_RENDER_CLOSE 6

#define ASSERT(x, y, ...) { if (x == y) { return true; }  else {std::cout<<__VA_ARGS__<<"\n"; return false;} }

static HWND m_hwnd;
static HWND m_Rhwnd;
static HMENU m_hmenu;
static bool m_Running;

// Window Properties
static unsigned int m_Render_Width = 720;
static unsigned int m_Render_Height = 720;
static unsigned int RefreshRate = 60;

unsigned int GetRenderWidth()
{
	return m_Render_Width;
}

unsigned int GetRenderHeight()
{
	return m_Render_Height;
}

void menuFunct(HWND hwnd)
{
	std::cout << "Menu Function Called !" << "\n";
	HMENU hMenubar;
	HMENU hMenu;
	HMENU WindowMenu;

	hMenubar = CreateMenu();
	hMenu = CreateMenu();
	WindowMenu = CreateMenu();


	AppendMenuW(hMenu, MF_STRING, IDM_FILE_NEW, L"&New");
	AppendMenuW(hMenu, MF_STRING, IDM_FILE_OPEN, L"&Open");
	AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
	AppendMenuW(hMenu, MF_STRING, IDM_FILE_QUIT, L"&Quit");

	AppendMenuW(WindowMenu, MF_STRING, IDM_WINDOW_RENDER_START, L"Start Render");
	AppendMenuW(WindowMenu, MF_STRING, IDM_WINDOW_RENDER_CLOSE, L"Close Render");

	AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenu, L"&File");
	AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)WindowMenu, L"&Window");
	AppendMenuW(hMenubar, MF_STRING, IDM_FILE_ABOUT, L"&About");
	SetMenu(hwnd, hMenubar);
	std::cout << "Menu Function ended !" << "\n";
}
LRESULT CALLBACK ChildProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

LRESULT CALLBACK CommandProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	std::cout << "Command Proc Called !" ;
	switch (LOWORD(wparam))
	{
	case IDM_FILE_NEW:
	case IDM_FILE_OPEN:
		std::cout << "Command Proc IDM_FILE_NEW and IDM_FILE_OPEN switch cases !" << "\n";
		MessageBeep(MB_ICONINFORMATION);
		break;
	case IDM_FILE_QUIT:
		std::cout << "Command Proc IDM_FILE_QUIT switch case !" << "\n";
		m_Running = false;
		PostQuitMessage(0);
		DestroyWindow(hwnd);
		break;
	case IDM_FILE_ABOUT:
		MessageBox(0, L"All Rights Reserved with me ! Just Kidding! Play around", L"About", MB_OK);
		break;
	case IDM_WINDOW_RENDER_START:
		if (m_Rhwnd == NULL)
		{
		}
		break;
	case IDM_WINDOW_RENDER_CLOSE:
		if (m_Rhwnd != NULL)
		{
			ChildProc(m_Rhwnd, WM_DESTROY, wparam, lparam);
			break;
		}
	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	return 0;
}
LRESULT CALLBACK ChildProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_RBUTTONUP:
		MessageBox(0, L"ChildProc Called!", L"ChildProc", MB_OK);
		break;
	case WM_DESTROY:
		//PostQuitMessage(0);
		//DestroyWindow(m_Rhwnd);
		CloseWindow(m_Rhwnd);
		break;
	case WM_NCDESTROY:
		hwnd = NULL;
		break;
	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	return 0;
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	HMENU hMenu;
	POINT point;
	if (hwnd == m_Rhwnd)
	{
		return ChildProc(hwnd, msg, wparam, lparam);
	}
	
	switch (msg)
	{
	case WM_RBUTTONUP:
		point.x = LOWORD(lparam);
		point.y = HIWORD(lparam);

		hMenu = CreatePopupMenu();
		ClientToScreen(hwnd, &point);

		AppendMenuW(hMenu, MF_STRING, IDM_FILE_OPEN, L"&View");
		AppendMenuW(hMenu, MF_SEPARATOR, 0, L"Seperator");
		AppendMenuW(hMenu, MF_STRING, IDM_FILE_NEW, L"&Porperties");
		AppendMenuW(hMenu, MF_STRING, IDM_FILE_QUIT, L"&Quit");

		TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, point.x, point.y, 0, hwnd, NULL);
		DestroyMenu(hMenu);
		break;

	case WM_CREATE:
		menuFunct(hwnd);
		break;
	case WM_COMMAND:
		CommandProc(hwnd, msg, wparam, lparam);
		break;
	case WM_DESTROY:
		m_Running = false;
		PostQuitMessage(0);
		DestroyWindow(hwnd);
		break;															
	default:														
		return DefWindowProc(hwnd, msg, wparam, lparam);			
	}
	return 0;
}

bool Init()
{
	if (!m_Running)
	{
		m_Running = true;
	}
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));

	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = NULL;
	wc.hbrBackground = CreateSolidBrush(RGB(55, 55, 55)); //(HBRUSH)GetStockObject(DKGRAY_BRUSH);;  COLOR_ACTIVEBORDER
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); //(HICON)LoadImageW(nullptr, L"res\\paddle.ico", IMAGE_ICON, 64, 64, LR_LOADFROMFILE);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = NULL;
	wc.lpszClassName = L"Cndr";
	wc.lpszMenuName = L"";
	wc.style = NULL;
	wc.lpfnWndProc = &WindowProc;

	if (!::RegisterClassEx(&wc))
	{
		return false;
	}

	m_hwnd = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, wc.lpszClassName, L"Cndr Engine", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720,
		NULL, NULL, NULL, NULL);
	m_Rhwnd = ::CreateWindowEx(0, L"Cndr", L"RenderOut", WS_CHILD | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, m_Render_Width, m_Render_Height,
		m_hwnd, (HMENU)(int)(ID_FIRSTCHILD), NULL, NULL);
	if (!m_hwnd)
	{
		return false;
	}
	if (!m_Rhwnd)
	{
		return false;
	}
	ShowWindow(m_hwnd, SW_SHOW);
	ShowWindow(m_Rhwnd, SW_SHOWNORMAL);
	UpdateWindow(m_hwnd);
	UpdateWindow(m_Rhwnd);
	return true;
}

bool broadcast()
{
	MSG msg = { 0 };

	BOOL bRet = 1;
	std::cout << "Flag in Broadcast" << "\n";
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

	return (int)msg.wParam;
}
