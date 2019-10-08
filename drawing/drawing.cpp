// drawing.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "drawing.h"

#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DRAWING, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DRAWING));

    MSG msg;

    // 主消息循环: 
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DRAWING));
    wcex.hCursor        = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_POINTER));
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DRAWING);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	HINSTANCE hInstance;
	static BOOLEAN lPress = FALSE; // 鼠标左键
	static POINT nowPos;

	//（1）win32双缓冲绘图: 定义设备描述表及位图句柄
	static HDC hMemDC;
	HBITMAP hBitmap;
	HBRUSH hWhiteBrush;
	static RECT rect;
    switch (message)
    {
	case WM_CREATE:
		//（2）win32双缓冲绘图: 创建一个与窗口矩形显示兼容的内存显示设备描述表
		hdc = GetDC(hWnd);
		hMemDC = CreateCompatibleDC(hdc);
		//（3）win32双缓冲绘图: 用hdc创建一个与窗口矩形显示兼容的位图
		GetClientRect(hWnd, &rect); // 窗口矩形
		hBitmap = CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
		ReleaseDC(hWnd, hdc);
		//（4）win32双缓冲绘图: 将位图hBitmap选入到内存显示设备hDCMem中, 
		// 只有选入了位图的内存显示设备才有地方绘图，画到指定的位图上
		SelectObject(hMemDC, hBitmap);
		//（5）win32双缓冲绘图: 用白色画笔将位图清除干净
		hWhiteBrush = CreateSolidBrush(RGB(255, 255, 255)); // 白色画刷
		FillRect(hMemDC, &rect, hWhiteBrush);
		break;
	case WM_SIZE:
		// TODO: 窗口大小改变时改变位图大小
		break;
	case WM_MOUSEMOVE:
		if (!lPress)
		{
			break;
		}
		// 这里没有 break。
	case WM_LBUTTONDOWN:
		lPress = TRUE; // 鼠标左键按住
		hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWL_HINSTANCE); // 获取应用程序实例句柄
		nowPos.x = GET_X_LPARAM(lParam) - 16;
		nowPos.y = GET_Y_LPARAM(lParam) - 16;

		hdc = GetDC(hWnd);
		//（6）win32双缓冲绘图: 绘图到 hMemDC
		// DrawIcon(hdc, nowPos.x, nowPos.y, LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)));
		DrawIcon(hMemDC, nowPos.x, nowPos.y, LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)));
		//（7）win32双缓冲绘图: 将内存中的图拷贝到窗口矩形上进行显示
		BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hMemDC, 0, 0, SRCCOPY);

		ReleaseDC(hWnd, hdc);
		break;
	case WM_LBUTTONUP:
		lPress = FALSE; // 鼠标左键松开
		nowPos.x = GET_X_LPARAM(lParam) - 16;
		nowPos.y = GET_Y_LPARAM(lParam) - 16;
		break;
	case WM_KEYDOWN:
		if (wParam == VK_UP || wParam == VK_DOWN || wParam == VK_LEFT || wParam == VK_RIGHT)
		{
			switch (wParam)
			{
			case VK_UP:
				nowPos.y -= 28;
				break;
			case VK_DOWN:
				nowPos.y += 28;
				break;
			case VK_LEFT:
				nowPos.x -= 28;
				break;
			case VK_RIGHT:
				nowPos.x += 28;
				break;
			}

			hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWL_HINSTANCE); // 获取应用程序实例句柄
			hdc = GetDC(hWnd);
			//（6）win32双缓冲绘图: 绘图到 hMemDC
			// DrawIcon(hdc, nowPos.x, nowPos.y, LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)));
			DrawIcon(hMemDC, nowPos.x, nowPos.y, LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)));
			//（7）win32双缓冲绘图: 将内存中的图拷贝到窗口矩形上进行显示
			BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hMemDC, 0, 0, SRCCOPY);
			ReleaseDC(hWnd, hdc);
		}
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择: 
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
