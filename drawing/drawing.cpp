// drawing.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "drawing.h"

#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������

// �˴���ģ���а����ĺ�����ǰ������: 
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

    // TODO: �ڴ˷��ô��롣

    // ��ʼ��ȫ���ַ���
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DRAWING, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ִ��Ӧ�ó����ʼ��: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DRAWING));

    MSG msg;

    // ����Ϣѭ��: 
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
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
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
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

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
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND  - ����Ӧ�ó���˵�
//  WM_PAINT    - ����������
//  WM_DESTROY  - �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	HINSTANCE hInstance;
	static BOOLEAN lPress = FALSE; // ������
	static POINT nowPos;

	//��1��win32˫�����ͼ: �����豸������λͼ���
	static HDC hMemDC;
	HBITMAP hBitmap;
	HBRUSH hWhiteBrush;
	static RECT rect;
    switch (message)
    {
	case WM_CREATE:
		//��2��win32˫�����ͼ: ����һ���봰�ھ�����ʾ���ݵ��ڴ���ʾ�豸������
		hdc = GetDC(hWnd);
		hMemDC = CreateCompatibleDC(hdc);
		//��3��win32˫�����ͼ: ��hdc����һ���봰�ھ�����ʾ���ݵ�λͼ
		GetClientRect(hWnd, &rect); // ���ھ���
		hBitmap = CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
		ReleaseDC(hWnd, hdc);
		//��4��win32˫�����ͼ: ��λͼhBitmapѡ�뵽�ڴ���ʾ�豸hDCMem��, 
		// ֻ��ѡ����λͼ���ڴ���ʾ�豸���еط���ͼ������ָ����λͼ��
		SelectObject(hMemDC, hBitmap);
		//��5��win32˫�����ͼ: �ð�ɫ���ʽ�λͼ����ɾ�
		hWhiteBrush = CreateSolidBrush(RGB(255, 255, 255)); // ��ɫ��ˢ
		FillRect(hMemDC, &rect, hWhiteBrush);
		break;
	case WM_SIZE:
		// TODO: ���ڴ�С�ı�ʱ�ı�λͼ��С
		break;
	case WM_MOUSEMOVE:
		if (!lPress)
		{
			break;
		}
		// ����û�� break��
	case WM_LBUTTONDOWN:
		lPress = TRUE; // ��������ס
		hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWL_HINSTANCE); // ��ȡӦ�ó���ʵ�����
		nowPos.x = GET_X_LPARAM(lParam) - 16;
		nowPos.y = GET_Y_LPARAM(lParam) - 16;

		hdc = GetDC(hWnd);
		//��6��win32˫�����ͼ: ��ͼ�� hMemDC
		// DrawIcon(hdc, nowPos.x, nowPos.y, LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)));
		DrawIcon(hMemDC, nowPos.x, nowPos.y, LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)));
		//��7��win32˫�����ͼ: ���ڴ��е�ͼ���������ھ����Ͻ�����ʾ
		BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hMemDC, 0, 0, SRCCOPY);

		ReleaseDC(hWnd, hdc);
		break;
	case WM_LBUTTONUP:
		lPress = FALSE; // �������ɿ�
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

			hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWL_HINSTANCE); // ��ȡӦ�ó���ʵ�����
			hdc = GetDC(hWnd);
			//��6��win32˫�����ͼ: ��ͼ�� hMemDC
			// DrawIcon(hdc, nowPos.x, nowPos.y, LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)));
			DrawIcon(hMemDC, nowPos.x, nowPos.y, LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)));
			//��7��win32˫�����ͼ: ���ڴ��е�ͼ���������ھ����Ͻ�����ʾ
			BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hMemDC, 0, 0, SRCCOPY);
			ReleaseDC(hWnd, hdc);
		}
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // �����˵�ѡ��: 
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
            // TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...
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

// �����ڡ������Ϣ�������
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
