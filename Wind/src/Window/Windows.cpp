#include "Windows.h"
#include "WindowContainer.h"
#include "Application.h"

#include <tchar.h>
#include <strsafe.h>

//extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

void GetErrorAndExit(LPTSTR lpszFunction)
{
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0, NULL);

    // Display the error message and exit the process

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
        (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
    StringCchPrintf((LPTSTR)lpDisplayBuf,
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"),
        lpszFunction, dw, lpMsgBuf);
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
    ExitProcess(dw);
}

namespace Core
{
    int resize = 0;

	LRESULT CALLBACK Windows::windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
        LRESULT result = NULL;
        WindowContainer* container = WindowContainer::GetContainer();
        Window* window = container->GetWindowFromHandle((void*)hwnd);
        if (window == nullptr)
        {
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }

        //if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
        //{
        //    return true;
        //}

		switch (msg)
		{
            
        case WM_CLOSE:				// Windows is destroyed and only make any necessary cleanup
		case WM_DESTROY:			// Windows is not started to destroy yet
		{
			//WindowData* data = (WindowData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			//WindowCloseEvent e;
			//data->EventFn(e);

            Application::GetApplication().Close();
            container->erase((void*)hwnd);
            break;
		}
        /*
        //case WM_INPUT:
        //{
        //    UINT dataSize;
        //    GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER));
        //    
        //    if (dataSize > 0)
        //    {
        //        Unique<BYTE[]> rawData = CreateUnique<BYTE[]>(dataSize);
        //        if(GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawData.get(), & dataSize, sizeof(RAWINPUTHEADER)) == dataSize)
        //        {
        //            RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawData.get());
        //            if (raw->header.dwType == RIM_TYPEMOUSE)
        //            {
        //                WindowData* data = (WindowData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        //                MouseMovedEvent e((float)LOWORD(lParam), (float)HIWORD(lParam));
        //                data->EventFn(e);
        //            }
        //        }
        //    }
        //
        //    return DefWindowProc(hwnd, msg, wParam, lParam);
        //}
        case WM_SIZE :
        {
            WindowData* data = (WindowData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            data->width = (UINT)LOWORD(lParam);
            data->height = (UINT)HIWORD(lParam);

            WindowResizeEvent e(data->width, data->height);
            data->EventFn(e);
        }
            break;
        case WM_KEYUP:
        {
            WindowData* data = (WindowData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            KeyReleasedEvent e(static_cast<KeyCode>(wParam));
            data->EventFn(e);
            break;
        }
        case WM_KEYDOWN :
        {
            WindowData* data = (WindowData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            int repeatCount = (lParam & 0xffff);
            KeyPressedEvent e(static_cast<KeyCode>(wParam), repeatCount);
            data->EventFn(e);
            break;
        }
        case WM_MOUSEMOVE:
        {
            WindowData* data = (WindowData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            //MouseMovedEvent e((float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));
            MouseMovedEvent e((float)LOWORD(lParam), (float)HIWORD(lParam));
            data->EventFn(e);
            break;
        }
        case WM_MOUSEWHEEL:
        {
            WindowData* data = (WindowData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            MouseScrolledEvent e((float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA);
            data->EventFn(e);
            break;
        }
        case WM_LBUTTONDOWN:
        {
            WindowData* data = (WindowData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            MouseButtonPressedEvent e(static_cast<MouseCode>(VK_LBUTTON));
            data->EventFn(e);
            break;
        }
        case WM_LBUTTONUP:
        {
            WindowData* data = (WindowData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            MouseButtonReleasedEvent e(static_cast<MouseCode>(VK_LBUTTON));
            data->EventFn(e);
            break;
        }
        case WM_DPICHANGED:
            if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
            {
                const RECT* suggested_rect = (RECT*)lParam;
                ::SetWindowPos(hwnd, NULL, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
            }
            break;
            */
		default:
			result = DefWindowProc(hwnd, msg, wParam, lParam);
            break;
		}
        
		return result;
	}

    Windows::Windows(const WindowProperties props)
    {
        if (!m_Instance)
        {
            m_Instance = this;
        }
        Init(props);
        //RAWINPUTDEVICE rid;
        //rid.usUsagePage = 0x01;
        //rid.usUsage = 0x02;
        //rid.dwFlags = 0;
        //rid.hwndTarget = NULL;
        //if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == false)
        //{
        //    std::cout << "Raw Input Registration Failed\n";
        //}
        //m_Data.m_rawInputInit = true;
    }

    Windows::~Windows()
    {
        WindowContainer* container = WindowContainer::GetContainer();
        if (container->size() == 0)
        {

            std::string classTitle = WindowContainer::GetContainer()->GetWindowClassTitle();

#ifdef UNICODE
            LPCWSTR wclassTitle = std::wstring(classTitle.begin(), classTitle.end()).c_str();
#endif

            int res = UnregisterClass(wclassTitle, GetModuleHandle(NULL));
            std::cout << "Window UnReg " << res << '\n';
        }
        m_Instance = NULL;
        delete m_Instance;
    }

    void Windows::SetTitle(const std::string title)
    {
        m_Data.title = title;

#if defined UNICODE
        std::wstring wTitle = std::wstring(m_Data.title.begin(), m_Data.title.end());
        LPCWSTR Title = wTitle.c_str();
        SetWindowText(m_hwnd, wTitle.c_str());
#elif defined ANSI
        SetWindowText(m_hwnd, _T(title.c_str()));
#endif
    }

    void Windows::SetIcon(const char* filename)
    {
#ifdef UNICODE

#else
        auto _hIcon = (HICON)LoadImage(nullptr, filename, IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
        auto _hIconSmall = (HICON)LoadImage(nullptr, filename, IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
        SendMessageA(m_hwnd, WM_SETICON, ICON_BIG, (LPARAM)_hIcon);
        SendMessageA(m_hwnd, WM_SETICON, ICON_SMALL, (LPARAM)_hIcon);
#endif
    }

    bool Windows::Init(const WindowProperties props)
    {
        m_Data.height = props.height;
        m_Data.width = props.width;
        m_Data.vsync = props.Vsync;
        m_Data.title = props.title;

        HICON hIcon = static_cast<HICON>(::LoadImage(NULL, MAKEINTRESOURCE(IDI_WARNING), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR | LR_SHARED | LR_DEFAULTSIZE));

        WindowContainer* container = WindowContainer::GetContainer();

#ifdef UNICODE
        std::wstring windowTitle = std::wstring(m_Data.title.begin(), m_Data.title.end());
        std::string cName = container->GetWindowClassTitle();
        std::wstring windowClassName = std::wstring(cName.begin(), cName.end());
#else
        auto Title = props.Title;
#endif

        if (container->size() == 0)
        {
            WNDCLASSEX wc;
            ZeroMemory(&wc, sizeof(wc));

            wc.cbClsExtra = NULL;
            wc.cbSize = sizeof(WNDCLASSEX);
            wc.cbWndExtra = NULL;
            wc.hbrBackground = CreateSolidBrush(RGB(55, 55, 55));
            wc.hCursor = LoadCursor(NULL, IDC_ARROW);
            //wc.hIcon =  (HICON)LoadImageW(NULL, L"C:\\dev\\Cndr\\Cndr\\resrc\\fire_1.png", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
            //wc.hIconSm = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(ID_ICON), IMAGE_ICON, 16, 16, 0);
            wc.hIcon = hIcon;
            wc.hIconSm = hIcon;
            wc.hInstance = container->GetHinstance();
            wc.lpszClassName = windowClassName.c_str();
            wc.lpszMenuName = NULL;
            wc.style = CS_CLASSDC;
            wc.lpfnWndProc = &windowProc;

            if (!::RegisterClassEx(&wc))
            {
                return false;
            }
        }
        std::cout << "Window Creation\n";

        RECT rect;
        rect.top = 0;
        rect.left = 0;
        rect.right = rect.left + m_Data.width;
        rect.bottom = rect.top + m_Data.height;

        AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

        //m_hwnd = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, wc.lpszClassName, Title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, m_Data.width, m_Data.height,
        //    NULL, NULL, wc.hInstance, this);

        m_hwnd = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, windowClassName.c_str(), windowTitle.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT
            , rect.right - rect.left, rect.bottom - rect.top,
            NULL, NULL, container->GetHinstance(), this);

        //m_hwnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, wc.lpszClassName, Title, (WS_BORDER), CW_USEDEFAULT, CW_USEDEFAULT, 1000, 720, NULL, NULL, wc.hInstance, this); //WS_BORDER removes border
        //SetWindowLong(m_hwnd, GWL_STYLE, 0);
        //SetWindowLong(m_hwnd, GWL_STYLE, WS_BORDER);


        if (!m_hwnd)
        {
            //CR_CORE_ERROR("m_hwnd value error");
            wchar_t error[11] = { 'H', 'W', 'N', 'D', ' ','E', 'r', 'r', 'o','r' };//"HWND Error";
            GetErrorAndExit(error);

            std::cout << "m_hwnd value error\n";            // Error here in last Run
            return false;
        }

        SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)&m_Data);
        ShowWindow(m_hwnd, SW_SHOW);
        UpdateWindow(m_hwnd);
        return true;
    }

    void* Windows::LoadIconFile(const std::string& icon)
    {
#ifdef UNICODE
        std::wstring path = std::wstring(icon.begin(), icon.end());
        LPCWSTR _icon = path.c_str();
#else
        const char* _icon = icon.c_str();
#endif 
        HICON hicon = (HICON)LoadImage(
            NULL,				// Handle Instance must be NULL when loading from a files
            _icon,		// Icon File
            IMAGE_ICON,			// Specifies that the file is an icon
            0,					// Width of the image (we'll specify default later on)
            0,					// Height of the image
            LR_LOADFROMFILE |	// Load a file (as opposed to a resource)
            LR_DEFAULTSIZE |	// Default metrics based on the type (IMAGE_ICON, 32x32)
            LR_SHARED			// Let the system release the handle when it's no longer used
        );
        HDC hdcd = GetDC(m_hwnd);
        DrawIconEx(hdcd, 100, 200, hicon, 72, 78, 0, NULL, DI_NORMAL);
        return hicon;
    }

    void Windows::Broadcast()
    {
        MSG msg = { 0 };

        while (PeekMessage(&msg, m_hwnd, 0, 0, PM_REMOVE) > 0)
        {
            //TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    void Windows::OnUpdate()
    {
        Broadcast();
    }

    void Windows::ShutDown()
    {
    }

    uint32_t Windows::GetHeight()
    {
        return m_Data.height;
    }

    uint32_t Windows::GetWidth()
    {
        return m_Data.width;
    }
}