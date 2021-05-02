#pragma once
#include <iostream>
#include <Windows.h>

#define IDM_FILE_NEW 1
#define IDM_FILE_OPEN 2
#define IDM_FILE_QUIT 3
#define IDM_FILE_ABOUT 4

static HWND m_hwnd;
static HMENU m_hmenu;

void menuFunct(HWND hwnd)
{
	HMENU hMenubar;
	HMENU hMenu;

	hMenubar = CreateMenu();
	hMenu = CreateMenu();

	AppendMenuW(hMenu, MF_STRING, IDM_FILE_NEW, L"&New");
	AppendMenuW(hMenu, MF_STRING, IDM_FILE_OPEN, L"&Open");
	AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
	AppendMenuW(hMenu, MF_STRING, IDM_FILE_QUIT, L"&Quit");

	AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenu, L"&File");
	AppendMenuW(hMenubar, MF_STRING, IDM_FILE_ABOUT, L"&About");
	SetMenu(hwnd, hMenubar);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam,LPARAM lparam)
{
	switch (msg)
	{
	case WM_CREATE:
		menuFunct(hwnd);
		break;
	case WM_COMMAND:
		switch (LOWORD(wparam)) {

		case IDM_FILE_NEW:
		case IDM_FILE_OPEN:

			MessageBeep(MB_ICONINFORMATION);
			break;

		case IDM_FILE_QUIT:

			SendMessage(hwnd, WM_CLOSE, 0, 0);
			break;
	case IDM_FILE_ABOUT:
		MessageBox(0, L"All Rights Reserved with me ! Just Kidding! Play around", L"About", MB_OK);
		break;
		}
		return DefWindowProc(hwnd, msg, wparam, lparam);
	case WM_DESTROY :
		PostQuitMessage(0);
		DestroyWindow(hwnd);
		break;
	default :
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
}

bool Init()
{
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
	if (!m_hwnd)
	{
		return false;
	}
	ShowWindow(m_hwnd, SW_SHOW);
	UpdateWindow(m_hwnd);
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